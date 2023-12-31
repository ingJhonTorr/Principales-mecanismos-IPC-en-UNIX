<h1 align=center>Principales mecanismos IPC en UNIX</h1>
Los Mecanismos de Comunicación entre Procesos (IPC), abreviatura de Inter-Process Communication, son herramientas y técnicas fundamentales que dotan de capacidad a los procesos en sistemas Unix para establecer conexiones y compartir recursos de manera eficiente y colaborativa. La capacidad de comunicarse y compartir recursos es esencial para la construcción de aplicaciones y sistemas complejos, lo que convierte a los IPC en una parte esencial del lenguaje C y los sistemas Unix.

<br><h2 align=center>Signals - Señales</h2>

Las señales en Unix son eventos asincrónicos que permiten la comunicación y el control entre procesos y el sistema operativo. Son notificaciones que un proceso envía o recibe para manejar eventos o situaciones específicas. Se presentan tres ejemplos en C que utilizan señales:

#### **Captura de Ctrl+C**
Un ejemplo simple es la captura de la señal SIGINT (generada al presionar Ctrl+C en el teclado) para realizar acciones antes de que un programa se cierre.

#### **Código:**
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

#### **Comunicación Padre-Hijo**
Las señales también se utilizan para la comunicación entre un proceso padre y uno, o varios procesos hijos. En el siguiente ejemplo, el padre envía una señal SIGUSR1 al hijo para notificar un evento.
#### **Código:**
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

#### **Comunicación entre Hilos**
Las señales también se pueden utilizar para la comunicación entre hilos dentro de un proceso. En el siguiente ejemplo, un hilo envía una señal a otro hilo para sincronizar su trabajo.
#### **Código:**
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

<br><h2 align=center>Semaphores - Semáforos</h2>
Los semáforos en sistemas Unix ayudan a controlar cuándo un programa puede acceder a ciertas áreas o recursos compartidos, evitando bloqueos y condiciones de carrera. También pueden ayudar a los programas a comunicarse entre sí, asegurándose que esperen su turno antes de continuar. En resumen, son herramientas clave para mantener el orden y la cooperación entre programas en sistemas multitarea.


#### **Control de Acceso con Semáforos**
Este código en C utiliza semáforos para coordinar la ejecución entre un hilo principal y un hilo secundario en un entorno multi-hilo. El semáforo permite que el hilo secundario ejecute su código solo después de que el hilo principal lo desbloquee, lo que ilustra la sincronización y el control de acceso a recursos compartidos en programas concurrentes.

#### **Código:**
```c
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
```

#### **Sincronización de Hilos con Semáforos**
Este código demuestra el uso de semáforos para coordinar dos hilos en un entorno multi-hilo. Se crea un semáforo para controlar el acceso a una sección de código compartida por los dos hilos. Cada hilo espera su turno mediante `sem_wait`, ejecuta su sección de código y luego finaliza con `sem_post`. La sincronización garantiza que los hilos impriman su salida en un orden específico y evita posibles conflictos durante la ejecución concurrente.

#### **Código:**
```c
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
```


#### **Incremento Concurrente con Semáforos**
Este ejemplo utiliza semáforos para coordinar el incremento concurrente de la variable `counter` por múltiples hilos. Se crean cuatro hilos, cada uno aumenta el valor de la variable en un millón mediante un ciclo for; los semáforos garantizan que los hilos accedan a la variable de manera sincronizada. Al final, se muestra el valor total de `counter`.

#### **Código:**
```c
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
```

<h2 align=center>Pipes - Tuberías</h2>
En sistemas Unix, Las tuberías son canales de comunicación unidireccional y permiten que dos programas se comuniquen entre sí de manera eficiente y sincronizada. Las tuberías son útiles para pasar información de un programa a otro, lo que permite la colaboración y el intercambio de datos de manera fluida. Además, las tuberías pueden utilizarse para la entrada y salida estándar, permitiendo que la salida de un programa se convierta en la entrada de otro, esto es esencial en la construcción de flujos de trabajo complejos y la automatización de tareas. A continuación, se muestran dos ejemplos en C que ilustran el funcionamiento de las tuberías.

#### **Comunicación Unidireccional entre Procesos con Tuberías**
Este código muestra cómo dos procesos, un padre y un hijo, se comunican utilizando tuberías. El proceso padre envía un mensaje al proceso hijo a través de una tubería, y el proceso hijo lee y muestra el mensaje recibido por la salida estandar.

#### **Código:**
```c
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
```

#### **Comunicación Bidireccional con dos Tuberías**
En este ejemplo, se implementa la comunicación bidireccional entre un proceso padre y un proceso hijo utilizando dos tuberías. Cada tubería se utiliza para enviar mensajes en una dirección específica, lo que permite una comunicación fluida en ambas direcciones. El proceso padre y el hijo pueden intercambiar mensajes de manera sincronizada y cuando cualquiera de los procesos escribe "SALIR" como mensaje, el programa termina, permitiendo una salida controlada.

#### **Código:**
```c
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
```
