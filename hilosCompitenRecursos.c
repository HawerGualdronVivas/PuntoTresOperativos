#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Definimos dos recursos compartidos como mutex
pthread_mutex_t resource1;
pthread_mutex_t resource2;

// Función del primer hilo
void* thread1(void* arg) {
    pthread_mutex_lock(&resource1);
    printf("Thread 1: Bloqueó Recurso 1\n");
    sleep(1); // Simula un retraso al intentar acceder al segundo recurso

    printf("Thread 1: Intentando bloquear Recurso 2...\n");
    pthread_mutex_lock(&resource2);
    printf("Thread 1: Bloqueó Recurso 2\n");

    // Liberamos los recursos
    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    return NULL;
}

// Función del segundo hilo
void* thread2(void* arg) {
    pthread_mutex_lock(&resource2);
    printf("Thread 2: Bloqueó Recurso 2\n");
    sleep(1); // Simula un retraso al intentar acceder al primer recurso

    printf("Thread 2: Intentando bloquear Recurso 1...\n");
    pthread_mutex_lock(&resource1);
    printf("Thread 2: Bloqueó Recurso 1\n");

    // Liberamos los recursos
    pthread_mutex_unlock(&resource1);
    pthread_mutex_unlock(&resource2);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Inicializamos los recursos (mutex)
    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    // Creamos los hilos
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    // Esperamos a que los hilos terminen
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destruimos los recursos
    pthread_mutex_destroy(&resource1);
    pthread_mutex_destroy(&resource2);

    return 0;
}
