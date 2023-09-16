#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t hilo1, hilo2;

void *funcion_hilo1(void *arg) {
    sleep(2);
    printf("Hilo 1: Enviando señal al Hilo 2...\n");
    pthread_kill(hilo2, SIGUSR1);
    return NULL;
}

void *funcion_hilo2(void *arg) {
    signal(SIGUSR1, SIG_IGN); // Ignorar la señal SIGUSR1 por defecto
    printf("Hilo 2: Esperando señal...\n");
    sleep(2); // Esperar a una señal
    printf("Hilo 2: Señal recibida. Continuando...\n");
    return NULL;
}

int main() {
    pthread_create(&hilo1, NULL, funcion_hilo1, NULL);
    pthread_create(&hilo2, NULL, funcion_hilo2, NULL);
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    return 0;
}