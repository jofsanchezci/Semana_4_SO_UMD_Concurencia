
# Concurrencia en Sistemas Operativos

## Introducción

El concepto de **concurrencia en sistemas operativos** es clave para entender cómo se gestionan múltiples tareas o procesos de manera que parezcan ejecutarse simultáneamente. Aunque la concurrencia no siempre implica que las tareas se ejecuten al mismo tiempo (como en un sistema con un solo núcleo de CPU), se refiere a la **intercalación de tareas** para optimizar el uso de los recursos del sistema, como la CPU, la memoria y otros periféricos.

## Técnicas para Manejar la Concurrencia

1. **Multitarea (Multitasking)**: Permite que varios procesos se ejecuten "simultáneamente" mediante la **interrupción y cambio de contexto** entre ellos. Esto se logra compartiendo el tiempo de CPU entre las diferentes tareas, también conocido como **time-sharing**. Cada proceso recibe un intervalo de tiempo (quantum), y al finalizar este tiempo, se le da paso a otro proceso.

2. **Multiprogramación (Multiprogramming)**: Esta técnica permite que varios programas se encuentren **cargados en la memoria al mismo tiempo**, y la CPU cambia de uno a otro cuando un proceso está esperando un recurso (por ejemplo, la finalización de una operación de entrada/salida). Aquí, el objetivo es maximizar el uso de la CPU evitando que quede inactiva.

3. **Multiprocesamiento (Multiprocessing)**: En este caso, existen **varias CPUs** o núcleos en un sistema que ejecutan múltiples procesos en paralelo de forma real, aprovechando los **recursos hardware** disponibles para realizar tareas concurrentes, incrementando así el rendimiento.

## Problemas de la Concurrencia

La concurrencia puede llevar a diversos problemas cuando varios procesos compiten por los mismos recursos del sistema. Dos de los problemas más comunes son:

- **Condiciones de carrera (Race Conditions)**: Ocurren cuando dos o más procesos acceden y modifican recursos compartidos sin un control adecuado, lo que puede llevar a resultados inesperados o inconsistentes. Por ejemplo, si dos procesos intentan escribir simultáneamente en una misma variable, los resultados pueden ser impredecibles.
  
- **Exclusión mutua (Mutual Exclusion)**: Se refiere a la necesidad de garantizar que cuando un proceso está accediendo a un recurso compartido, ningún otro proceso pueda hacerlo hasta que el primero haya terminado. Si no se maneja bien, esto puede llevar a problemas como el **bloqueo mutuo (deadlock)**, donde varios procesos se bloquean entre sí esperando recursos.

## Mecanismos para Controlar la Concurrencia

Para prevenir los problemas de concurrencia, los sistemas operativos emplean **mecanismos de sincronización**, tales como:

1. **Semáforos**: Son variables que se utilizan para controlar el acceso a recursos compartidos. Pueden estar en un estado de "señal" o "espera", lo que indica si un recurso está disponible o no. Los semáforos ayudan a gestionar la exclusión mutua y la coordinación de procesos.

2. **Mutex (Mutual Exclusion Locks)**: Son similares a los semáforos, pero están diseñados específicamente para garantizar que solo un proceso pueda acceder a un recurso crítico a la vez. Los mutex son esenciales para evitar las condiciones de carrera.

3. **Monitores**: Proporcionan una abstracción de alto nivel para manejar la concurrencia. Un monitor es una estructura que permite que solo un proceso esté ejecutando una sección crítica de código a la vez, lo que evita conflictos al acceder a recursos compartidos.


# Uso de Semáforos en C

Este es un ejemplo de implementación en C utilizando **semáforos** para controlar el acceso a un recurso compartido entre varios hilos.

## Descripción

En este ejemplo, dos hilos intentan acceder y modificar una variable compartida. El semáforo asegura que sólo un hilo acceda a la sección crítica (el recurso compartido) a la vez, evitando **condiciones de carrera**.

### Conceptos clave:

- **Semáforos**: Son mecanismos de sincronización que permiten coordinar el acceso concurrente a recursos compartidos. Un semáforo puede tener un valor que indica cuántos hilos pueden acceder al recurso. En este caso, el semáforo se inicializa con un valor de 1, lo que significa que sólo un hilo puede acceder al recurso en un momento dado.
- **Sección Crítica**: Es la porción del código donde se accede al recurso compartido y donde necesitamos exclusión mutua para evitar condiciones de carrera.

## Código:

```c
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
    printf("Hilo %d accediendo al recurso compartido.
", id_hilo);
    recurso_compartido++;
    printf("Recurso compartido: %d
", recurso_compartido);

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
```

## Explicación del Código:

1. **`sem_init(&semaforo, 0, 1);`**: Inicializa el semáforo con un valor de 1, lo que significa que un hilo puede acceder a la sección crítica a la vez.
   
2. **`sem_wait(&semaforo);`**: Esta función hace que el hilo espere hasta que el semáforo esté disponible (si su valor es mayor que 0), y luego decrementa el valor del semáforo. Esto bloquea otros hilos hasta que el semáforo sea liberado.

3. **`sem_post(&semaforo);`**: Libera el semáforo incrementando su valor, lo que permite que otros hilos accedan a la sección crítica.

4. Los hilos creados con `pthread_create` ejecutan la función `incrementar_recurso`, que accede al recurso compartido protegido por el semáforo.

## Compilación y Ejecución:

Para compilar este programa en un sistema basado en Linux, puedes utilizar el siguiente comando:

```bash
gcc -o semaforo_example semaforo_example.c -lpthread
```

Luego, puedes ejecutar el programa con:

```bash
./semaforo_example
```

Este programa evitará que dos hilos accedan simultáneamente al recurso compartido, solucionando así el problema de la **condición de carrera**.


# Uso de Mutex en C

Este es un ejemplo de implementación en C utilizando **mutex** (bloqueos de exclusión mutua) para controlar el acceso a un recurso compartido entre varios hilos y evitar **condiciones de carrera**.

## Descripción

En este ejemplo, dos hilos intentan acceder y modificar una variable compartida. El mutex asegura que sólo un hilo acceda a la sección crítica (el recurso compartido) a la vez, evitando problemas de concurrencia.

### Conceptos clave:

- **Mutex (Mutual Exclusion)**: Un mutex es un mecanismo de sincronización que permite la exclusión mutua, es decir, asegura que solo un hilo pueda ejecutar una sección crítica a la vez. Esto es útil para evitar condiciones de carrera en las que varios hilos acceden a la vez a recursos compartidos.
- **Sección Crítica**: Es la parte del código donde se accede a un recurso compartido que debe protegerse mediante sincronización.

## Código:

```c
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
```

## Explicación del Código:

1. **`pthread_mutex_init(&mutex, NULL);`**: Inicializa el mutex que se usará para controlar el acceso a la sección crítica.

2. **`pthread_mutex_lock(&mutex);`**: Bloquea el mutex antes de entrar en la sección crítica, impidiendo que otros hilos entren mientras el recurso está siendo utilizado por el hilo actual.

3. **Sección crítica**: En este ejemplo, la sección crítica es la parte del código donde el hilo accede y modifica la variable `recurso_compartido`.

4. **`pthread_mutex_unlock(&mutex);`**: Libera el mutex después de que el hilo termina de trabajar en la sección crítica, permitiendo que otros hilos accedan a ella.

5. **`pthread_mutex_destroy(&mutex);`**: Destruye el mutex después de que todos los hilos han terminado, liberando cualquier recurso asociado con el mutex.

## Compilación y Ejecución:

Para compilar este programa en un sistema basado en Linux, puedes utilizar el siguiente comando:

```bash
gcc -o mutex_example mutex_example.c -lpthread
```

Luego, puedes ejecutar el programa con:

```bash
./mutex_example
```

Este programa asegura que sólo un hilo acceda a la sección crítica a la vez, evitando así condiciones de carrera y otros problemas de concurrencia.


## Conclusión

En resumen, la **concurrencia en sistemas operativos** es esencial para mejorar la eficiencia y el rendimiento de un sistema. Sin embargo, también introduce desafíos, y es fundamental emplear mecanismos adecuados de sincronización para evitar problemas como las condiciones de carrera o la exclusión mutua.
