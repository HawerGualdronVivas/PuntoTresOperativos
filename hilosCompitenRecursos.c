#include <pthread.h> // Biblioteca para crear, gestionar y sincronizar hilos y mutex en C.
#include <stdio.h> // Biblioteca estándar de entrada/salida.
#include <unistd.h> // Para la función sleep().


// Declaramos dos recursos compartidos como mutex.
// Estos representan los recursos que los hilos intentarán bloquear.
pthread_mutex_t resource1;
pthread_mutex_t resource2;

// Función ejecutada por el primer hilo
void* thread1(void* arg) {
    // Bloqueamos el primer recurso
    pthread_mutex_lock(&resource1);
    printf("Thread 1: Bloqueó Recurso 1\n");

    // Retraso de 1 segundo antes de intentar bloquear el segundo recurso.
    // Esto aumenta la probabilidad de un deadlock al dar tiempo al segundo hilo de bloquear el otro recurso.
    sleep(1);

    printf("Thread 1: Intentando bloquear Recurso 2...\n");

    // Intentamos bloquear el segundo recurso. Aquí puede ocurrir el interbloqueo
    // si el segundo hilo ya tiene este recurso bloqueado.
    pthread_mutex_lock(&resource2);
    printf("Thread 1: Bloqueó Recurso 2\n");

    // Liberamos los recursos que hemos bloqueado.
    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    // Finalizamos la ejecución del hilo.
    return NULL;
}

// Función ejecutada por el segundo hilo
void* thread2(void* arg) {
    // Bloqueamos el segundo recurso
    pthread_mutex_lock(&resource2);
    printf("Thread 2: Bloqueó Recurso 2\n");

    // Simulamos un retraso de 1 segundo antes de intentar bloquear el primer recurso.
    // Esto aumenta la probabilidad de un deadlock.
    sleep(1);

    printf("Thread 2: Intentando bloquear Recurso 1...\n");
    // Intentamos bloquear el primer recurso. Aquí puede ocurrir el interbloqueo
    // si el primer hilo ya tiene este recurso bloqueado.
    pthread_mutex_lock(&resource1);
    printf("Thread 2: Bloqueó Recurso 1\n");

    // Liberamos los recursos que hemos bloqueado.
    pthread_mutex_unlock(&resource1);
    pthread_mutex_unlock(&resource2);

    // Finalizamos la ejecución del hilo.
    return NULL;
}
int main() {
    pthread_t t1, t2; // Declaramos dos identificadores de hilo

    // Inicializamos los recursos compartidos (mutex).
    // Esto prepara los mutex para que los hilos puedan bloquearlos.
    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    // Creamos los hilos.
    // Cada hilo ejecutará una función diferente (thread1 o thread2).
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    // Esperamos a que los hilos terminen su ejecución.
    // Esto asegura que el programa principal no termine antes que los hilos.
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destruimos los recursos (mutex).
    // Esto limpia los mutex, liberando los recursos del sistema.
    pthread_mutex_destroy(&resource1);
    pthread_mutex_destroy(&resource2);  
    return 0; // Finalizamos el programa.
}
