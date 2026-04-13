#include <stdio.h>
#include <string.h>
#include <time.h>
#include "persistencia.h"

/* ── Guardar estado completo en archivo de texto ─────────── */
void guardar_estado(Proceso p[], int n, Metricas *m,
                    ArchivoCompartido *af, const char *algoritmo) {
    FILE *f = fopen(ARCHIVO_ESTADO, "w");
    if (!f) {
        printf("[Error] No se pudo guardar el estado.\n");
        return;
    }

    time_t ahora = time(NULL);
    fprintf(f, "# Estado guardado: %s", ctime(&ahora));
    fprintf(f, "ALGORITMO:%s\n", algoritmo);
    fprintf(f, "PROCESOS:%d\n", n);

    for (int i = 0; i < n; i++) {
        fprintf(f, "P:%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                p[i].id, p[i].llegada, p[i].rafaga,
                p[i].restante, p[i].prioridad,
                p[i].finalizacion, p[i].espera,
                p[i].retorno, p[i].tipo);
    }

    fprintf(f, "METRICAS:%.2f,%.2f,%.2f,%.2f\n",
            m->promedio_espera, m->promedio_retorno,
            m->utilizacion_cpu, m->espera_archivo);

    /* Guardar contenido del archivo compartido */
    pthread_mutex_lock(&af->mutex);
    fprintf(f, "ARCHIVO:%d,%d\n", af->cantidad, af->capacidad);
    for (int i = 0; i < af->cantidad; i++)
        fprintf(f, "ITEM:%s\n", af->items[i]);
    pthread_mutex_unlock(&af->mutex);

    fclose(f);
    printf("[Persistencia] Estado guardado en '%s'\n", ARCHIVO_ESTADO);
}

/* ── Cargar estado desde archivo ─────────────────────────── */
int cargar_estado(Proceso p[], int *n, Metricas *m,
                  char *algoritmo_out) {
    FILE *f = fopen(ARCHIVO_ESTADO, "r");
    if (!f) {
        printf("[Persistencia] No existe estado previo guardado.\n");
        return 0;
    }

    char linea[512];
    *n = 0;
    int idx = 0;

    while (fgets(linea, sizeof(linea), f)) {
        if (linea[0] == '#') continue;

        if (strncmp(linea, "ALGORITMO:", 10) == 0)
            sscanf(linea + 10, "%49s", algoritmo_out);

        else if (strncmp(linea, "PROCESOS:", 9) == 0)
            sscanf(linea + 9, "%d", n);

        else if (strncmp(linea, "P:", 2) == 0) {
            sscanf(linea + 2, "%d,%d,%d,%d,%d,%d,%d,%d,%11s",
                   &p[idx].id,          &p[idx].llegada,
                   &p[idx].rafaga,      &p[idx].restante,
                   &p[idx].prioridad,   &p[idx].finalizacion,
                   &p[idx].espera,      &p[idx].retorno,
                   p[idx].tipo);
            idx++;
        }
        else if (strncmp(linea, "METRICAS:", 9) == 0)
            sscanf(linea + 9, "%f,%f,%f,%f",
                   &m->promedio_espera, &m->promedio_retorno,
                   &m->utilizacion_cpu, &m->espera_archivo);
    }

    fclose(f);
    printf("[Persistencia] Cargados %d procesos. Algoritmo: %s\n",
           *n, algoritmo_out);
    return 1;
}

/* ── Mostrar reporte de metricas ─────────────────────────── */
void mostrar_reporte(Proceso p[], int n, Metricas *m,
                     const char *algoritmo) {
    printf("\n+==========================================+\n");
    printf("|          REPORTE DE METRICAS             |\n");
    printf("|  Algoritmo: %-28s|\n", algoritmo);
    printf("+==========================================+\n");
    printf("  Promedio de espera  : %.2f unidades\n",
           m->promedio_espera);
    printf("  Promedio de retorno : %.2f unidades\n",
           m->promedio_retorno);
    printf("  Utilizacion de CPU  : %.1f%%\n",
           m->utilizacion_cpu);
    printf("  Espera por archivo  : %.6f segundos\n",
           m->espera_archivo);

    printf("\n  %-5s %-12s %-8s %-8s %-8s %-8s\n",
           "PID", "Tipo", "Llegada", "Rafaga", "Espera", "Retorno");
    printf("  --------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("  %-5d %-12s %-8d %-8d %-8d %-8d\n",
               p[i].id, p[i].tipo, p[i].llegada,
               p[i].rafaga, p[i].espera, p[i].retorno);
    }
}
