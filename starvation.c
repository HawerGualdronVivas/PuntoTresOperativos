#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_HILOS 5  // Número total de hilos
#define TIEMPO_MAX_ESPERA 3  // Tiempo máximo de espera en segundos para evitar inanición

pthread_mutex_t recurso;  // Recurso compartido
int tiempos_espera[NUM_HILOS];  // Array para registrar el tiempo de espera de cada hilo
int prioridades[NUM_HILOS];  // Array para las "prioridades" de los hilos

// Función que simula el trabajo de un hilo
void* hilo(void* arg) {
    int id = *((int*)arg);  // Obtener ID del hilo

    // Retrasar los hilos de baja prioridad
    if (prioridades[id] == 0) {
        printf("Hilo %d (baja prioridad) tiene un retraso inicial.\n", id);
        sleep(2);  // Retraso para hilos de baja prioridad
    }

    while (1) {
        // Verifica si el hilo está esperando demasiado tiempo
        if (tiempos_espera[id] > TIEMPO_MAX_ESPERA) {
            prioridades[id] = 1;  // Aumentamos la prioridad
            printf("Hilo %d (baja prioridad) está siendo favorecido.\n", id);
        }

        // Intentar acceder al recurso
        if (pthread_mutex_trylock(&recurso) == 0) {
            // Simula el trabajo con el recurso
            printf("Hilo %d accedió al recurso.\n", id);
            sleep(1);  // Simula tiempo de uso del recurso
            pthread_mutex_unlock(&recurso);
            break;  // Sale del ciclo después de acceder al recurso
        } else {
            // Incrementa el tiempo de espera si no puede acceder al recurso
            tiempos_espera[id]++;
            printf("Hilo %d está esperando... Tiempo de espera: %d segundos\n", id, tiempos_espera[id]);
            sleep(1);  // Simula el tiempo de espera
        }
    }

    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    // Inicialización
    pthread_mutex_init(&recurso, NULL);

    // Inicializar prioridades (0 = baja, 1 = alta)
    for (int i = 0; i < NUM_HILOS; i++) {
        prioridades[i] = (i % 2 == 0) ? 1 : 0;  // Hilos pares: alta prioridad, impares: baja prioridad
        tiempos_espera[i] = 0;
    }

    // Crear hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, hilo, &ids[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Liberar recursos
    pthread_mutex_destroy(&recurso);

    return 0;
}
