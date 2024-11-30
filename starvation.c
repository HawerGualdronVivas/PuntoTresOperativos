#include <pthread.h> // Biblioteca para manejo de hilos
#include <stdio.h>   // Biblioteca estándar para entrada y salida
#include <stdlib.h>  // Biblioteca estándar de C para funciones como malloc, exit, etc.
#include <unistd.h>  // Para funciones como sleep
#include <time.h>    // Para manejar tiempos y cálculos de espera

// Definimos constantes para el programa
#define NUM_HILOS 5          // Número total de hilos a crear
#define TIEMPO_MAX_ESPERA 3  // Tiempo máximo en segundos para evitar inanición

// Variables globales
pthread_mutex_t recurso;        // Mutex para sincronizar el acceso al recurso compartido
int tiempos_espera[NUM_HILOS];  // Array que almacena el tiempo de espera de cada hilo
int prioridades[NUM_HILOS];     // Array que almacena la prioridad de cada hilo (0 = baja, 1 = alta)

// Función que simula el trabajo de cada hilo
void* hilo(void* arg) {
    int id = *((int*)arg);  // Convertimos el argumento para obtener el ID del hilo

    // Simulamos un retraso inicial para hilos de baja prioridad
    if (prioridades[id] == 0) {
        printf("Hilo %d (baja prioridad) tiene un retraso inicial.\n", id);
        sleep(2);  // Retraso para los hilos de baja prioridad
    }

    while (1) {  // Ciclo infinito hasta que el hilo acceda al recurso
        // Verificamos si el hilo ha esperado demasiado tiempo
        if (tiempos_espera[id] > TIEMPO_MAX_ESPERA) {
            prioridades[id] = 1;  // Incrementamos la prioridad del hilo
            printf("Hilo %d (baja prioridad) está siendo favorecido.\n", id);
        }

        // Intentar bloquear el mutex para acceder al recurso
        if (pthread_mutex_trylock(&recurso) == 0) {
            // Acceso exitoso al recurso
            printf("Hilo %d accedió al recurso.\n", id);
            sleep(1);  // Simulamos el tiempo que el hilo usa el recurso
            pthread_mutex_unlock(&recurso);  // Liberamos el recurso
            break;  // Salimos del ciclo porque el hilo ya accedió al recurso
        } else {
            // No se pudo acceder al recurso, incrementamos el tiempo de espera
            tiempos_espera[id]++;
            printf("Hilo %d está esperando... Tiempo de espera: %d segundos\n", id, tiempos_espera[id]);
            sleep(1);  // Simulamos el tiempo de espera antes de reintentar
        }
    }

    return NULL;  // Finalizamos la ejecución del hilo
}

int main() {
    pthread_t hilos[NUM_HILOS];  // Array para almacenar los identificadores de los hilos
    int ids[NUM_HILOS];          // Array para pasar los IDs a cada hilo

    // Inicializamos el mutex del recurso compartido
    pthread_mutex_init(&recurso, NULL);

    // Inicializamos las prioridades y los tiempos de espera de los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        prioridades[i] = (i % 2 == 0) ? 1 : 0;  // Hilos pares: alta prioridad, impares: baja prioridad
        tiempos_espera[i] = 0;  // Inicializamos los tiempos de espera a 0
    }

    // Creamos los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;  // Asignamos el ID del hilo
        pthread_create(&hilos[i], NULL, hilo, &ids[i]);  // Creamos el hilo y ejecutamos la función "hilo"
    }

    // Esperamos a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);  // Bloqueamos el programa hasta que cada hilo termine
    }

    // Destruimos el mutex al finalizar
    pthread_mutex_destroy(&recurso);

    return 0;  // Terminamos el programa principal
}
