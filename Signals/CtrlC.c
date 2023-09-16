#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void manejador(int sig) {
    printf("Se recibió la señal SIGINT (Ctrl+C).\n");
    exit(0);
}

int main() {
    signal(SIGINT, manejador); // Configurar el manejador de señales
    while (1) {
        sleep(1);
    }
    return 0;
}

