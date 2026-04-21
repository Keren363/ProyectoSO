#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "archivo_compartido.h"

/* ── Inicializar archivo compartido ──────────────────────── */
void inicializar_archivo(ArchivoCompartido *af, int capacidad) {
    if (capacidad > MAX_CAPACIDAD) capacidad = MAX_CAPACIDAD;
    if (capacidad < 1)             capacidad = 1;

    af->cantidad             = 0;
    af->capacidad            = capacidad;
    af->tiempo_espera_total  = 0.0;

    pthread_mutex_init(&af->mutex, NULL);
    sem_init(&af->espacios_libres,   0, capacidad);
    sem_init(&af->items_disponibles, 0, 0);

    printf("[Archivo] Inicializado con capacidad %d\n", capacidad);
}

/* ── Destruir recursos ───────────────────────────────────── */
void destruir_archivo(ArchivoCompartido *af) {
    pthread_mutex_destroy(&af->mutex);
    sem_destroy(&af->espacios_libres);
    sem_destroy(&af->items_disponibles);
}

/* ── Calcular resultado de una operacion ─────────────────── */
/* Formatos: SUMA:3+7  MULT:4*9  FACTORIAL:5  PROMEDIO:2,4,6 */
double calcular_operacion(const char *item) {
    char tipo[20], datos[200];

    if (sscanf(item, "%19[^:]:%199s", tipo, datos) != 2)
        return 0.0;

    if (strcmp(tipo, "SUMA") == 0) {
        int a = 0, b = 0;
        sscanf(datos, "%d+%d", &a, &b);
        return (double)(a + b);
    }
    if (strcmp(tipo, "MULT") == 0) {
        int a = 0, b = 0;
        sscanf(datos, "%d*%d", &a, &b);
        return (double)(a * b);
    }
    if (strcmp(tipo, "FACTORIAL") == 0) {
        int n = 0;
        sscanf(datos, "%d", &n);
        long long r = 1;
        for (int i = 2; i <= n; i++) r *= i;
        return (double)r;
    }
    if (strcmp(tipo, "PROMEDIO") == 0) {
        double suma = 0.0;
        int count   = 0;
        char *copy  = strdup(datos);
        char *tok   = strtok(copy, ",");
        while (tok) {
            suma += atof(tok);
            count++;
            tok = strtok(NULL, ",");
        }
        free(copy);
        return count > 0 ? suma / count : 0.0;
    }
    return 0.0;
}

/* ── Productor: escribe una operacion ────────────────────── */
int productor_escribir(ArchivoCompartido *af, int proceso_id,
                       const char *operacion) {
    struct timespec ini, fin;
    clock_gettime(CLOCK_MONOTONIC, &ini);

    sem_wait(&af->espacios_libres);         /* esperar espacio */

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double espera = (fin.tv_sec  - ini.tv_sec) +
                    (fin.tv_nsec - ini.tv_nsec) / 1e9;

    pthread_mutex_lock(&af->mutex);

    strncpy(af->items[af->cantidad], operacion, MAX_ITEM_LEN - 1);
    af->items[af->cantidad][MAX_ITEM_LEN - 1] = '\0';
    af->cantidad++;
    af->tiempo_espera_total += espera;

    printf("[Productor %d] Escribio: %-25s (items: %d/%d)\n",
           proceso_id, operacion, af->cantidad, af->capacidad);

    pthread_mutex_unlock(&af->mutex);
    sem_post(&af->items_disponibles);       /* notificar consumidor */
    return 1;
}

/* ── Consumidor: lee y ejecuta una operacion ─────────────── */
int consumidor_leer(ArchivoCompartido *af, int proceso_id,
                    char *resultado_out) {
    struct timespec ini, fin;
    clock_gettime(CLOCK_MONOTONIC, &ini);

    sem_wait(&af->items_disponibles);       /* esperar item */

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double espera = (fin.tv_sec  - ini.tv_sec) +
                    (fin.tv_nsec - ini.tv_nsec) / 1e9;

    pthread_mutex_lock(&af->mutex);

    char operacion[MAX_ITEM_LEN];
    strncpy(operacion, af->items[0], MAX_ITEM_LEN);

    /* Desplazar items a la izquierda */
    for (int i = 0; i < af->cantidad - 1; i++)
        strncpy(af->items[i], af->items[i + 1], MAX_ITEM_LEN);
    af->cantidad--;
    af->tiempo_espera_total += espera;

    double resultado = calcular_operacion(operacion);
    snprintf(resultado_out, MAX_ITEM_LEN,
             "%s = %.2f", operacion, resultado);

    printf("[Consumidor %d] Leyo: %-25s => %.2f  (items: %d/%d)\n",
           proceso_id, operacion, resultado,
           af->cantidad, af->capacidad);

    pthread_mutex_unlock(&af->mutex);
    sem_post(&af->espacios_libres);         /* liberar espacio */
    return 1;
}

/* ── Mostrar contenido actual del archivo ────────────────── */
void mostrar_archivo(const ArchivoCompartido *af) {
    printf("\n[Archivo Compartido]  Items: %d/%d\n",
           af->cantidad, af->capacidad);
    printf("----------------------------\n");
    if (af->cantidad == 0) {
        printf("  (vacio)\n");
    } else {
        for (int i = 0; i < af->cantidad; i++)
            printf("  [%d] %s\n", i + 1, af->items[i]);
    }
    printf("----------------------------\n");
    printf("Tiempo total en espera por archivo: %.6f s\n",
           af->tiempo_espera_total);
}
