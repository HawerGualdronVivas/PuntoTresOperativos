#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_HILOS 10  // Número de hilos
#define ITERACIONES 1000  // Número de iteraciones por hilo

int contador_global = 0;  // Recurso compartido
pthread_mutex_t mutex;    // Mutex para proteger el recurso compartido
sem_t semaforo;           // Semáforo binario para sincronización

// Función para simular el trabajo de los hilos
void* hilo_trabajo(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < ITERACIONES; i++) {
        // Bloquear acceso al recurso usando el mutex
        pthread_mutex_lock(&mutex);

        // Incrementar el contador global (sección crítica)
        contador_global++;

        // Desbloquear el mutex
        pthread_mutex_unlock(&mutex);

        // Usar el semáforo para coordinar
        sem_wait(&semaforo);  // Decrementar el semáforo
        printf("Hilo %d ejecutó iteración %d.\n", id, i + 1);
        sem_post(&semaforo);  // Incrementar el semáforo
    }

    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Inicializar el mutex
    pthread_mutex_init(&mutex, NULL);

    // Inicializar el semáforo binario con valor 1
    sem_init(&semaforo, 0, 1);

    // Crear los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, hilo_trabajo, &ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Mostrar el valor final del contador
    printf("Valor final del contador global: %d\n", contador_global);

    // Destruir el mutex y el semáforo
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaforo);

    return 0;
}
