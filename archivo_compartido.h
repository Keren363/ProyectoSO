#ifndef ARCHIVO_COMPARTIDO_H
#define ARCHIVO_COMPARTIDO_H

#include <pthread.h>
#include <semaphore.h>

#define MAX_CAPACIDAD 100
#define MAX_ITEM_LEN  256

/* Archivo compartido entre productores y consumidores */
typedef struct {
    char   items[MAX_CAPACIDAD][MAX_ITEM_LEN];
    int    cantidad;
    int    capacidad;
    double tiempo_espera_total;
    pthread_mutex_t mutex;
    sem_t           espacios_libres;
    sem_t           items_disponibles;
} ArchivoCompartido;

void   inicializar_archivo(ArchivoCompartido *af, int capacidad);
void   destruir_archivo(ArchivoCompartido *af);
int    productor_escribir(ArchivoCompartido *af, int proceso_id,
                          const char *operacion);
int    consumidor_leer(ArchivoCompartido *af, int proceso_id,
                       char *resultado_out);
void   mostrar_archivo(const ArchivoCompartido *af);
double calcular_operacion(const char *item);

#endif
