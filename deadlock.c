#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t resource1;
pthread_mutex_t resource2;

void* thread1(void* arg) {
    pthread_mutex_lock(&resource1);
    printf("Thread 1: Bloqueó Recurso 1\n");
    sleep(1);

    printf("Thread 1: Intentando bloquear Recurso 2...\n");
    pthread_mutex_lock(&resource2);
    printf("Thread 1: Bloqueó Recurso 2\n");

    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&resource2);
    printf("Thread 2: Bloqueó Recurso 2\n");
    sleep(1);

    printf("Thread 2: Intentando bloquear Recurso 1...\n");
    pthread_mutex_lock(&resource1);
    printf("Thread 2: Bloqueó Recurso 1\n");

    pthread_mutex_unlock(&resource1);
    pthread_mutex_unlock(&resource2);
    return NULL;
}

void detect_deadlock() {
    int lock1 = pthread_mutex_trylock(&resource1);
    int lock2 = pthread_mutex_trylock(&resource2);

    if (lock1 != 0 && lock2 != 0) {
        printf("Detectado interbloqueo (deadlock): Ambos recursos están bloqueados.\n");
        printf("Rompiendo el interbloqueo...\n");
        pthread_mutex_unlock(&resource1);
        pthread_mutex_unlock(&resource2);
    } else {
        printf("No se detecta interbloqueo.\n");
    }

    if (lock1 == 0) pthread_mutex_unlock(&resource1);
    if (lock2 == 0) pthread_mutex_unlock(&resource2);
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    sleep(2); // Esperar un poco para que ocurra el deadlock
    detect_deadlock();

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&resource1);
    pthread_mutex_destroy(&resource2);

    return 0;
}
