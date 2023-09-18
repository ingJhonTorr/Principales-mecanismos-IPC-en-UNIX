#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Proceso hijo
        close(pipe_fd[1]); // Cerramos el extremo de escritura de la tubería
        char buffer[100];
        read(pipe_fd[0], buffer, sizeof(buffer));
        printf("Hijo: Mensaje recibido: %s\n", buffer);
        close(pipe_fd[0]);
    } else {
        // Proceso padre
        close(pipe_fd[0]); // Cerramos el extremo de lectura de la tubería
        const char *mensaje = "Hola desde el padre!";
        write(pipe_fd[1], mensaje, strlen(mensaje) + 1);
        close(pipe_fd[1]);
    }

    return 0;
}
