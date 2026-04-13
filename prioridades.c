#include <stdio.h>
#include "proceso.h"

void ejecutar_prioridades(Proceso p[], int n, Metricas *m) {
    /* Ordenar: menor numero = mayor prioridad */
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].prioridad < p[i].prioridad) {
                Proceso tmp = p[i]; p[i] = p[j]; p[j] = tmp;
            }
        }
    }

    printf("\n+------------------------------------------------------+\n");
    printf("|           Algoritmo por Prioridades                  |\n");
    printf("+------------------------------------------------------+\n");
    printf("%-5s %-10s %-10s %-8s %-8s %-8s\n",
           "PID", "Prioridad", "Llegada", "Rafaga", "Espera", "Retorno");
    printf("------------------------------------------------------\n");

    int tiempo = 0;
    float total_espera = 0, total_retorno = 0;

    for (int i = 0; i < n; i++) {
        if (tiempo < p[i].llegada)
            tiempo = p[i].llegada;

        p[i].espera      = tiempo - p[i].llegada;
        p[i].retorno     = p[i].espera + p[i].rafaga;
        p[i].finalizacion = tiempo + p[i].rafaga;

        printf("%-5d %-10d %-10d %-8d %-8d %-8d\n",
               p[i].id, p[i].prioridad, p[i].llegada,
               p[i].rafaga, p[i].espera, p[i].retorno);

        total_espera  += p[i].espera;
        total_retorno += p[i].retorno;
        tiempo += p[i].rafaga;
    }

    m->promedio_espera  = total_espera  / n;
    m->promedio_retorno = total_retorno / n;
    m->utilizacion_cpu  = 100.0f;
    m->espera_archivo   = 0;

    printf("------------------------------------------------------\n");
    printf("Promedio espera: %.2f | Promedio retorno: %.2f\n",
           m->promedio_espera, m->promedio_retorno);
}
