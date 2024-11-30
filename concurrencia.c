#include <pthread.h>  // Biblioteca para manejo de hilos
#include <semaphore.h> // Biblioteca para semáforos
#include <stdio.h>    // Biblioteca estándar para entrada y salida
#include <stdlib.h>   // Biblioteca estándar para funciones generales
#include <unistd.h>   // Para funciones como sleep

// Definición de constantes
#define NUM_HILOS 10      // Número total de hilos a crear
#define ITERACIONES 1000  // Número de iteraciones que cada hilo ejecutará

// Variables globales
int contador_global = 0;   // Recurso compartido entre los hilos
pthread_mutex_t mutex;     // Mutex para proteger el acceso al recurso compartido
sem_t semaforo;            // Semáforo binario para coordinar la salida de mensajes

// Función que ejecutarán los hilos
void* hilo_trabajo(void* arg) {
    int id = *((int*)arg);  // Recuperar el ID del hilo desde el argumento

    for (int i = 0; i < ITERACIONES; i++) {
        // Bloquear el acceso al recurso compartido utilizando el mutex
        pthread_mutex_lock(&mutex);

        // Sección crítica: incrementar el contador global
        contador_global++;

        // Desbloquear el mutex para permitir el acceso a otros hilos
        pthread_mutex_unlock(&mutex);

        // Coordinación mediante el semáforo
        sem_wait(&semaforo);  // Bloquea si el semáforo está en 0
        printf("Hilo %d ejecutó iteración %d.\n", id, i + 1);  // Imprime información
        sem_post(&semaforo);  // Incrementa el semáforo, liberando acceso para otro hilo
    }

    return NULL;  // Finaliza la ejecución del hilo
}

int main() {
    pthread_t hilos[NUM_HILOS];  // Array para almacenar los identificadores de los hilos
    int ids[NUM_HILOS];          // Array para asignar IDs únicos a cada hilo

    // Inicialización del mutex
    pthread_mutex_init(&mutex, NULL);

    // Inicialización del semáforo binario con valor inicial 1
    sem_init(&semaforo, 0, 1);

    // Crear los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;  // Asignar un ID único a cada hilo
        pthread_create(&hilos[i], NULL, hilo_trabajo, &ids[i]);  // Crear el hilo y ejecutar "hilo_trabajo"
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);  // Bloquear el programa principal hasta que cada hilo termine
    }

    // Mostrar el valor final del contador global
    printf("Valor final del contador global: %d\n", contador_global);

    // Destruir el mutex y el semáforo para liberar recursos
    pthread_mutex_destroy(&mutex);  // Elimina el mutex
    sem_destroy(&semaforo);         // Elimina el semáforo

    return 0;  // Finaliza la ejecución del programa
}
