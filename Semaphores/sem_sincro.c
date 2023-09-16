#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;

void* thread(void* arg)
{
    sem_post(&mutex);
    char * c1 = (char *)arg;
    printf("\n%s",c1);
    sleep(3);
    //signal
    printf("\nTerminando Thread 1...\n");
    sem_wait(&mutex);
}

void* thread2(void* arg)
{
    sem_wait(&mutex);
    char * c2 = (char *)arg;
    printf("\n%s",c2);
    sleep(3);
    //signal
    printf("\nTerminando Thread 2...\n");
}

int main()
{
    sem_init(&mutex, 0, 1);
    pthread_t t1,t2;
    char *cadena1 = "hola mundo Thread 1";
    char *cadena2 = "hola mundo Thread 2";
    printf("\nProceso Padre iniciando...\n");
    pthread_create(&t1,NULL,thread,(void*)cadena1);
    sleep(2);
    pthread_create(&t2,NULL,thread2,(void*)cadena2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("\nProceso Padre saliendo...\n");
    sem_destroy(&mutex);
    return 0;
}