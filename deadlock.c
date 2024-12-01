#include <pthread.h> // Biblioteca para crear, gestionar y sincronizar hilos y mutex en C.
#include <stdio.h> // Biblioteca estándar de entrada/salida.
#include <unistd.h> // Para la función sleep().
#include <time.h> // Biblioteca para manejar tiempos

pthread_mutex_t resource1; // Definimos un mutex que simula el Recurso 1. 
pthread_mutex_t resource2; // Definimos un mutex que simula el Recurso 2. 

// Función que será ejecutada por el primer hilo.
void* thread1(void* arg) {
    pthread_mutex_lock(&resource1);
    // El hilo bloquea el Recurso 1.
    printf("Thread 1: Bloqueó Recurso 1\n");

    sleep(1);
    // Simulamos un retraso de 1 segundo antes de intentar bloquear el siguiente recurso.
    // Esto aumenta la probabilidad de interbloqueo (deadlock).

    printf("Thread 1: Intentando bloquear Recurso 2...\n");
    pthread_mutex_lock(&resource2);
    // El hilo intenta bloquear el Recurso 2. Si el Recurso 2 está bloqueado por otro hilo, el programa entra en deadlock.
    printf("Thread 1: Bloqueó Recurso 2\n");

    // Liberamos ambos recursos para permitir que otros hilos los utilicen.
    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    return NULL;
}

// Función que será ejecutada por el segundo hilo.
void* thread2(void* arg) {
    pthread_mutex_lock(&resource2);
    // El hilo bloquea el Recurso 2.
    printf("Thread 2: Bloqueó Recurso 2\n");

    sleep(1);
    // Simulamos un retraso de 1 segundo antes de intentar bloquear el siguiente recurso.
    // Esto aumenta la probabilidad de interbloqueo (deadlock).

    printf("Thread 2: Intentando bloquear Recurso 1...\n");
    pthread_mutex_lock(&resource1);
    // El hilo intenta bloquear el Recurso 1. Si el Recurso 1 está bloqueado por otro hilo, el programa entra en deadlock.
    printf("Thread 2: Bloqueó Recurso 1\n");

    // Liberamos ambos recursos para permitir que otros hilos los utilicen.
    pthread_mutex_unlock(&resource1);
    pthread_mutex_unlock(&resource2);

    return NULL;
}

// Función para detectar y resolver interbloqueos (deadlocks).
void detect_deadlock() {
    clock_t start_time, end_time; // Variables para medir tiempo

    start_time = clock(); // Registramos el tiempo de inicio

    int lock1 = pthread_mutex_trylock(&resource1);
    int lock2 = pthread_mutex_trylock(&resource2);

    if (lock1 != 0 && lock2 != 0) {
        // Si ambos recursos están bloqueados, detectamos un interbloqueo.
        printf("Detectado interbloqueo (deadlock): Ambos recursos están bloqueados.\n");
        printf("Rompiendo el interbloqueo...\n");

        // Forzamos la liberación de los recursos bloqueados.
        pthread_mutex_unlock(&resource1);
        pthread_mutex_unlock(&resource2);
    } else {
        printf("No se detecta interbloqueo.\n");
    }

    // Si logramos bloquear algún recurso, lo desbloqueamos inmediatamente para no alterar el comportamiento normal.
    if (lock1 == 0) pthread_mutex_unlock(&resource1);
    if (lock2 == 0) pthread_mutex_unlock(&resource2);

    end_time = clock(); // Registramos el tiempo de finalización

    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Tiempo en segundos
    printf("Tiempo de resolución de deadlock: %.6f segundos\n", time_spent);
}

// Función principal.
int main() {
    pthread_t t1, t2; // Declaramos dos identificadores de hilos.

    pthread_mutex_init(&resource1, NULL); // Inicializamos el mutex para el Recurso 1.
    pthread_mutex_init(&resource2, NULL); // Inicializamos el mutex para el Recurso 2.

    pthread_create(&t1, NULL, thread1, NULL); // Creamos el primer hilo que ejecutará la función thread1.
    pthread_create(&t2, NULL, thread2, NULL); // Creamos el segundo hilo que ejecutará la función thread2.

    sleep(2); // Introducimos un retraso para dar tiempo a que ocurra el deadlock antes de detectarlo.
    detect_deadlock(); // Llamamos a la función que detecta y resuelve interbloqueos.

    pthread_join(t1, NULL); // Esperamos a que el primer hilo termine.
    pthread_join(t2, NULL); // Esperamos a que el segundo hilo termine.

    pthread_mutex_destroy(&resource1); // Destruimos el mutex para liberar los recursos del sistema.
    pthread_mutex_destroy(&resource2); // Destruimos el mutex para liberar los recursos del sistema.

    return 0;
}
