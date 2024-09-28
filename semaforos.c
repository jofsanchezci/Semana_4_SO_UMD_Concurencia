#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Variable compartida
int recurso_compartido = 0;

// Definición del semáforo
sem_t semaforo;

// Función que será ejecutada por los hilos
void* incrementar_recurso(void* arg) {
    // Espera el semáforo (decrementa el valor del semáforo)
    sem_wait(&semaforo);

    // Sección crítica
    int id_hilo = *((int*)arg);
    printf("Hilo %d accediendo al recurso compartido.\n", id_hilo);
    recurso_compartido++;
    printf("Recurso compartido: %d\n", recurso_compartido);

    // Simulamos trabajo
    sleep(1);

    // Liberar el semáforo (incrementa el valor del semáforo)
    sem_post(&semaforo);

    return NULL;
}

int main() {
    // Inicializamos el semáforo con valor 1 (sección crítica puede ser usada por un hilo a la vez)
    sem_init(&semaforo, 0, 1);

    // Definimos dos hilos
    pthread_t hilos[2];
    int id_hilos[2] = {1, 2};

    // Crear los hilos
    pthread_create(&hilos[0], NULL, incrementar_recurso, &id_hilos[0]);
    pthread_create(&hilos[1], NULL, incrementar_recurso, &id_hilos[1]);

    // Esperar a que los hilos terminen
    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);

    // Destruir el semáforo
    sem_destroy(&semaforo);

    printf("Proceso completado. Valor final del recurso compartido: %d\n", recurso_compartido);

    return 0;
}
