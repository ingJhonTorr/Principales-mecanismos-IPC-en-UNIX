#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

sem_t mySemaphore;

void *thread_function(void *arg) {
    // Funcion que ejecutará el thread
    printf("Thread esperando...\n");
    sem_wait(&mySemaphore);
    printf("Thread ejecutandose!\n");
    sem_post(&mySemaphore);
    return NULL;
}

int main() {
    // Se crea el semaforo
    sem_init(&mySemaphore, 0, 1);
    // Se crea un thread
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, NULL);
    // Main thread
    sleep(3); // Espera un momento
    sem_post(&mySemaphore); // Libera el semáforo
    // Se espera a que el thread termine su ejecución
    pthread_join(tid, NULL);
    // libera cualquier memoria o recursos asignados al semáforo
    sem_destroy(&mySemaphore);
    return 0;
}
