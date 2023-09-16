#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void manejador(int sig) {
    printf("\nEl proceso hijo recibió una señal SIGUSR1.\n");
    exit(0);
}

int main() {
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        // Proceso hijo
        signal(SIGUSR1, manejador); // Configurar el manejador de señales
        printf("Soy el proceso Hijo ");
        while (1) {
            printf(".");
            sleep(1);
        }
    } else if (pid > 0) {
        // Proceso padre
        sleep(5); // Esperar un poco
        kill(pid, SIGUSR1); // Enviar señal SIGUSR1 al hijo
        wait(NULL); // Esperar a que el hijo termine
    }

    return 0;
}