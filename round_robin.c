#include <stdio.h>
#include "proceso.h"

void ejecutar_rr(Proceso p[], int n, int quantum, Metricas *m) {
    for (int i = 0; i < n; i++)
        p[i].restante = p[i].rafaga;

    printf("\n+----------------------------------------------+\n");
    printf("|       Algoritmo Round Robin  (Q=%2d)         |\n", quantum);
    printf("+----------------------------------------------+\n");

    int tiempo = 0, completados = 0;
    float total_espera = 0, total_retorno = 0;

    while (completados < n) {
        int ejecutado = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].llegada <= tiempo && p[i].restante > 0) {
                ejecutado = 1;
                if (p[i].restante > quantum) {
                    printf("Tiempo %2d: Proceso %d ejecuta %d unidades\n",
                           tiempo, p[i].id, quantum);
                    tiempo += quantum;
                    p[i].restante -= quantum;
                } else {
                    printf("Tiempo %2d: Proceso %d termina\n",
                           tiempo, p[i].id);
                    tiempo += p[i].restante;
                    p[i].restante    = 0;
                    p[i].finalizacion = tiempo;
                    completados++;
                }
            }
        }
        if (!ejecutado) tiempo++;
    }

    printf("\n%-5s %-10s %-8s %-8s %-8s\n",
           "PID", "Llegada", "Rafaga", "Espera", "Retorno");
    printf("---------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        p[i].retorno = p[i].finalizacion - p[i].llegada;
        p[i].espera  = p[i].retorno - p[i].rafaga;
        total_espera  += p[i].espera;
        total_retorno += p[i].retorno;
        printf("%-5d %-10d %-8d %-8d %-8d\n",
               p[i].id, p[i].llegada, p[i].rafaga,
               p[i].espera, p[i].retorno);
    }

    m->promedio_espera  = total_espera  / n;
    m->promedio_retorno = total_retorno / n;
    m->utilizacion_cpu  = 100.0f;
    m->espera_archivo   = 0;

    printf("---------------------------------------------\n");
    printf("Promedio espera: %.2f | Promedio retorno: %.2f\n",
           m->promedio_espera, m->promedio_retorno);
}
