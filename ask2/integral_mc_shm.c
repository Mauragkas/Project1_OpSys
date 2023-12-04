/*
1093513 Christodoulopoylos Efstathios Panagiotis
1093514 Christodoulou Nikolaos
1097445 Basilopoulos Basilios
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

double get_wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

inline double f(double x) {
    return sin(cos(x));
}

double *initialize_shared_memory(int num_processes) {
    return mmap(NULL, num_processes * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}

void calculate_integral(int process_index, double a, double h, unsigned long points_per_process, double *shared_results) {
    double res = 0.0;
    for (unsigned long j = 0; j < points_per_process; j++) {
        double xi = a + (process_index * points_per_process + j) * h;
        res += f(xi);
    }
    shared_results[process_index] = res * h;
}

double aggregate_results(double *shared_results, int num_processes) {
    double total_result = 0;
    for (int i = 0; i < num_processes; i++) {
        total_result += shared_results[i];
    }
    return total_result;
}

int main(int argc, char *argv[]) {
    double a = 0.0, b = 1.0;
    unsigned long n = 24e7;
    double h = (b - a) / n;
    double ref = 0.73864299803689018;
    int num_processes = 4; // Default number of processes
    pid_t pid;
    double start_time, end_time;

    if (argc == 2) {
        num_processes = atoi(argv[1]);
        if (num_processes <= 0) {
            fprintf(stderr, "Number of processes must be positive.\n");
            return 1;
        }
    }

    unsigned long points_per_process = n / num_processes;
    double *shared_results = initialize_shared_memory(num_processes);

    start_time = get_wtime(); // Start timer

    for (int i = 0; i < num_processes; i++) {
        pid = fork();
        if (pid == 0) {
            calculate_integral(i, a, h, points_per_process, shared_results);
            exit(0);
        } else if (pid < 0) {
            fprintf(stderr, "Fork failed.\n");
            return 1;
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    double total_result = aggregate_results(shared_results, num_processes);
    end_time = get_wtime(); // Stop timer

    printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n",
           total_result, fabs(total_result - ref), fabs(total_result - ref) / ref, end_time - start_time);

    munmap(shared_results, num_processes * sizeof(double));

    return 0;
}
