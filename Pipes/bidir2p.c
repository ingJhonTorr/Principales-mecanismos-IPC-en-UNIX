#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe_fd[2];
    int pipe_fd2[2];
    pid_t pid;
    char buffer[256];

    if (pipe(pipe_fd) < 0 || pipe(pipe_fd2) < 0) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { // Proceso padre
        close(pipe_fd[0]); // Cerramos el extremo de lectura de la primera tubería
        close(pipe_fd2[1]); // Cerramos el extremo de escritura de la segunda tubería
        while (1) {
            printf("Escriba un mensaje para enviar al proceso hijo: ");
            fgets(buffer, 256, stdin);
            write(pipe_fd[1], buffer, strlen(buffer) + 1); // Escribimos en la primera tubería

            if (strcmp(buffer, "SALIR\n") == 0) {
                printf("La comunicación ha terminado!");
                break; // Terminar si se ingresa "SALIR"
            }

            read(pipe_fd2[0], buffer, 256); // Leemos de la segunda tubería
            printf("El proceso padre ha recibido el mensaje del proceso hijo: %s", buffer);
        }
        close(pipe_fd[1]); // Cerramos el extremo de escritura de la primera tubería
        close(pipe_fd2[0]); // Cerramos el extremo de lectura de la segunda tubería
    }
    else { // Proceso hijo
        close(pipe_fd[1]); // Cerramos el extremo de escritura de la primera tubería
        close(pipe_fd2[0]); // Cerramos el extremo de lectura de la segunda tubería
        while (1) {
            read(pipe_fd[0], buffer, 256); // Leemos de la primera tubería

            if (strcmp(buffer, "SALIR\n") == 0) {
                printf("La comunicación ha terminado!");
                break; // Terminar si se ingresa "SALIR"
            }

            printf("El proceso hijo ha recibido el mensaje del proceso padre: %s", buffer);
            printf("Escriba un mensaje para enviar al proceso padre: ");
            fgets(buffer, 256, stdin);
            write(pipe_fd2[1], buffer, strlen(buffer) + 1); // Escribimos en la segunda tubería
        }
        close(pipe_fd[0]); // Cerramos el extremo de lectura de la primera tubería
        close(pipe_fd2[1]); // Cerramos el extremo de escritura de la segunda tubería
    }

    return 0;
}
