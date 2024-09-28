#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// Búfer compartido y contadores
int buffer[BUFFER_SIZE];
int count = 0;

// Definir semáforos y mutex
sem_t vacios;    // Espacios disponibles en el búfer
sem_t llenos;    // Elementos disponibles para consumir
pthread_mutex_t mutex;  // Exclusión mutua para el acceso al búfer

// Función del productor
void* productor(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        item = i;  // Producir un nuevo ítem
        
        // Esperar a que haya espacio vacío en el búfer
        sem_wait(&vacios);
        // Proteger el acceso exclusivo al búfer
        pthread_mutex_lock(&mutex);
        
        // Colocar el ítem en el búfer
        buffer[count] = item;
        count++;
        printf("Productor produjo: %d\n", item);
        
        // Liberar el mutex
        pthread_mutex_unlock(&mutex);
        // Indicar que hay un elemento disponible para consumir
        sem_post(&llenos);
        
        sleep(1);  // Simular el tiempo de producción
    }
    return NULL;
}

// Función del consumidor
void* consumidor(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        // Esperar a que haya ítems disponibles en el búfer
        sem_wait(&llenos);
        // Proteger el acceso exclusivo al búfer
        pthread_mutex_lock(&mutex);
        
        // Retirar el ítem del búfer
        count--;
        item = buffer[count];
        printf("Consumidor consumió: %d\n", item);
        
        // Liberar el mutex
        pthread_mutex_unlock(&mutex);
        // Indicar que hay un espacio vacío en el búfer
        sem_post(&vacios);
        
        sleep(1);  // Simular el tiempo de consumo
    }
    return NULL;
}

int main() {
    // Inicializar los semáforos
    sem_init(&vacios, 0, BUFFER_SIZE);  // El búfer está inicialmente vacío
    sem_init(&llenos, 0, 0);            // No hay ítems para consumir inicialmente
    pthread_mutex_init(&mutex, NULL);
    
    // Crear hilos para productor y consumidor
    pthread_t prod, cons;
    pthread_create(&prod, NULL, productor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);
    
    // Esperar a que ambos hilos terminen
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    // Destruir semáforos y mutex
    sem_destroy(&vacios);
    sem_destroy(&llenos);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
