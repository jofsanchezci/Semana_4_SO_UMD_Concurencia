#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Variable compartida
int recurso_compartido = 0;

// Definición del mutex
pthread_mutex_t mutex;

// Función que será ejecutada por los hilos
void* incrementar_recurso(void* arg) {
    // Bloquear el mutex (impide que otros hilos accedan a la sección crítica)
    pthread_mutex_lock(&mutex);

    // Sección crítica
    int id_hilo = *((int*)arg);
    printf("Hilo %d accediendo al recurso compartido.\n", id_hilo);
    recurso_compartido++;
    printf("Recurso compartido: %d\n", recurso_compartido);

    // Simulamos trabajo
    sleep(1);

    // Liberar el mutex (permite que otros hilos accedan a la sección crítica)
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // Inicializamos el mutex
    pthread_mutex_init(&mutex, NULL);

    // Definimos dos hilos
    pthread_t hilos[2];
    int id_hilos[2] = {1, 2};

    // Crear los hilos
    pthread_create(&hilos[0], NULL, incrementar_recurso, &id_hilos[0]);
    pthread_create(&hilos[1], NULL, incrementar_recurso, &id_hilos[1]);

    // Esperar a que los hilos terminen
    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    printf("Proceso completado. Valor final del recurso compartido: %d\n", recurso_compartido);

    return 0;
}
