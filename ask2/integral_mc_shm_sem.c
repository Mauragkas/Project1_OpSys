/*
1093513 Christodoulopoylos Efstathios Panagiotis
1093514 Christodoulou Nikolaos
1097445 Basilopoulos Basilios
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

double get_wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

inline double f(double x) {
    return sin(cos(x));
}

sem_t semaphore;
double result = 0.0; // Global variable to store the result

void *thread_function(void *arg) {
    unsigned long chunk_size = *((unsigned long*)arg);
    double local_result = 0.0;

    // Initialize random number generator with a unique seed for each thread
    srand(time(NULL) ^ (unsigned int)pthread_self()); 

    for (unsigned long i = 0; i < chunk_size; i++) {
        double xi = (double)rand() / RAND_MAX;
        local_result += f(xi);
    }

    sem_wait(&semaphore);  // Entering critical section
    result += local_result;
    sem_post(&semaphore);  // Exiting critical section

    return NULL;
}

int main(int argc, char * argv[]) {
  double a = 0.0;
  double b = 1.0;
  unsigned long n = 24e7;
  const double h = (b - a) / n;
  const double ref = 0.73864299803689018;

  int num_threads = 4;  // Default number of threads

  if (argc == 2) {
    num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
      fprintf(stderr, "Number of threads must be positive.\n");
      return 1;
    }
  }

  pthread_t threads[num_threads];
  unsigned long chunk_size = n / num_threads;

  sem_init(&semaphore, 0, 1);  // Initialize semaphore

  double start_time = get_wtime();  // Start timer

  //Create threads
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, thread_function, &chunk_size);
  }

  //Join threads
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  double end_time = get_wtime();  // Stop timer

  result *= h;

  printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n",
           result, fabs(result - ref), fabs(result - ref) / ref, end_time - start_time);

  sem_destroy(&semaphore);  // Destroy semaphore

  return 0;
}