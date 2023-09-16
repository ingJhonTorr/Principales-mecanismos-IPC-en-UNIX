#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4 // Cantidad de Threads
sem_t mutex;
int counter;

void *thread(void *arg)
{
    int i;
    sem_wait(&mutex);
    for (i = 0; i < 1000000; i++)
        counter++;
    sem_post(&mutex);
}

int main()
{
    pthread_t threads[NTHREADS];
    int i;
    sem_init(&mutex, 0, 1);

    for (i = 0; i < NTHREADS; i++)
        pthread_create(&threads[i], NULL, thread, NULL);

    for (i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Counter value: %d\n", counter);
    sem_destroy(&mutex);
    return 0;
}