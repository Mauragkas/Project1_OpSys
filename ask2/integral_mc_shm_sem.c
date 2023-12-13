/*
1093513 Christodoulopoylos Efstathios Panagiotis
1093514 Christodoulou Nikolaos
1097445 Basilopoulos Basilios
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

double get_wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

double f(double x) {
    return sin(cos(x));
}

void initialize_shared_resources(double **result, sem_t **semaphore) {
    *result = mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    **result = 0.0;

    *semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(*semaphore, 1, 1);
}

void calculate_integral(unsigned long chunk_size, double *result, sem_t *semaphore) {
    double local_result = 0.0;
    srand(time(NULL) ^ getpid());
    
    for (unsigned long i = 0; i < chunk_size; i++) {
        local_result += f((double)rand() / RAND_MAX);
    }

    sem_wait(semaphore);
    *result += local_result;
    sem_post(semaphore);
}

void cleanup_shared_resources(double *result, sem_t *semaphore) {
    sem_destroy(semaphore);
    munmap(semaphore, sizeof(sem_t));
    munmap(result, sizeof(double));
}

int main(int argc, char *argv[]) {
    unsigned long n = 24e7;  // Total number of points
    int num_processes = 4;   // Default number of processes
    double ref = 0.73864299803689018;

    if (argc == 2) {
        num_processes = atoi(argv[1]);
        if (num_processes <= 0) {
            fprintf(stderr, "Number of processes must be positive.\n");
            return 1;
        }
    }

    // Shared memory for result and semaphore
    double *result;
    sem_t *semaphore;
    initialize_shared_resources(&result, &semaphore);

    unsigned long chunk_size = n / num_processes;

    double start_time = get_wtime();

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            calculate_integral(chunk_size, result, semaphore);
            exit(0);
        }
    }

    // Parent process waits for all child processes
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    double end_time = get_wtime();

    double total_result = *result / n;
    printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n",
           total_result, fabs(total_result - ref), fabs(total_result - ref) / ref, end_time - start_time);

    // Clean up
    cleanup_shared_resources(result, semaphore);

    return 0;
}
