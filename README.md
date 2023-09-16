<h1 align=center>Principales-mecanismos-IPC-en-UNIX</h1>
Los Mecanismos de Comunicación entre Procesos (IPC), abreviatura de Inter-Process Communication, son herramientas y técnicas fundamentales que dotan de capacidad a los procesos en sistemas Unix para establecer conexiones y compartir recursos de manera eficiente y colaborativa. La capacidad de comunicarse y compartir recursos es esencial para la construcción de aplicaciones y sistemas complejos, lo que convierte a los IPC en una parte esencial del lenguaje C y los sistemas Unix.

<h2 align=center>Signals - Señales</h2>

Las señales en Unix son eventos asincrónicos que permiten la comunicación y el control entre procesos y el sistema operativo. Son notificaciones que un proceso envía o recibe para manejar eventos o situaciones específicas. Se presentan tres ejemplos en C que utilizan señales:

### **Captura de Ctrl+C:**
Un ejemplo simple es la captura de la señal SIGINT (generada al presionar Ctrl+C en el teclado) para realizar acciones antes de que un programa se cierre.

#### **Código en C:**
```c
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
```

### **Comunicación Padre-Hijo:**
Las señales también se utilizan para la comunicación entre un proceso padre y uno, o varios procesos hijos. En el siguiente ejemplo, el padre envía una señal SIGUSR1 al hijo para notificar un evento.
#### **Código en C:**
```c
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
```

### **Comunicación entre Hilos:**
Las señales también se pueden utilizar para la comunicación entre hilos dentro de un proceso. En el siguiente ejemplo, un hilo envía una señal a otro hilo para sincronizar su trabajo.
#### **Código en C:**
```c
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
```

<h2 align=center>Semaphores - Semáforos</h2>
