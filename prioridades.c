#include <stdio.h>
#include "proceso.h"

/* ── Prioridades SIN desalojo ───────────────────────────── */
void ejecutar_prioridades(Proceso p[], int n, Metricas *m) {
    /* Ordenar: menor número = mayor prioridad */
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].prioridad < p[i].prioridad) {
                Proceso tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
        }
    }

    printf("\n+------------------------------------------------------+\n");
    printf("|      Prioridades SIN desalojo                        |\n");
    printf("+------------------------------------------------------+\n");
    printf("%-5s %-10s %-10s %-8s %-8s %-8s\n",
           "PID","Prioridad","Llegada","Rafaga","Espera","Retorno");
    printf("------------------------------------------------------\n");

    int tiempo = 0;
    float total_espera = 0, total_retorno = 0;

    for (int i = 0; i < n; i++) {
        if (tiempo < p[i].llegada)
            tiempo = p[i].llegada;

        p[i].espera       = tiempo - p[i].llegada;
        p[i].retorno      = p[i].espera + p[i].rafaga;
        p[i].finalizacion = tiempo + p[i].rafaga;

        printf("%-5d %-10d %-10d %-8d %-8d %-8d\n",
               p[i].id, p[i].prioridad, p[i].llegada,
               p[i].rafaga, p[i].espera, p[i].retorno);

        total_espera  += p[i].espera;
        total_retorno += p[i].retorno;
        tiempo += p[i].rafaga;
    }

    m->promedio_espera  = total_espera / n;
    m->promedio_retorno = total_retorno / n;
    m->utilizacion_cpu  = 100.0f;
    m->espera_archivo   = 0;

    printf("------------------------------------------------------\n");
    printf("Promedio espera: %.2f | Promedio retorno: %.2f\n",
           m->promedio_espera, m->promedio_retorno);
}

/* ── Prioridades CON desalojo (Preemptive) ─────────────── */
void ejecutar_prioridades_desalojo(Proceso p[], int n, Metricas *m) {
    for (int i = 0; i < n; i++)
        p[i].restante = p[i].rafaga;

    printf("\n+------------------------------------------------------+\n");
    printf("|      Prioridades CON desalojo (Preemptive)           |\n");
    printf("+------------------------------------------------------+\n");

    int tiempo = 0, completados = 0;
    float total_espera = 0, total_retorno = 0;

    while (completados < n) {
        int mejor = -1;

        /* Buscar proceso disponible con mayor prioridad */
        for (int i = 0; i < n; i++) {
            if (p[i].llegada <= tiempo && p[i].restante > 0) {
                if (mejor == -1 || p[i].prioridad < p[mejor].prioridad) {
                    mejor = i;
                }
            }
        }

        /* Si no hay procesos disponibles */
        if (mejor == -1) {
            tiempo++;
            continue;
        }

        printf("Tiempo %2d: Proceso %d (Prioridad %d) ejecuta 1 unidad\n",
               tiempo, p[mejor].id, p[mejor].prioridad);

        p[mejor].restante--;
        tiempo++;

        /* Si termina */
        if (p[mejor].restante == 0) {
            p[mejor].finalizacion = tiempo;
            p[mejor].retorno      = tiempo - p[mejor].llegada;
            p[mejor].espera       = p[mejor].retorno - p[mejor].rafaga;

            total_espera  += p[mejor].espera;
            total_retorno += p[mejor].retorno;

            completados++;

            printf("         Proceso %d termino. Espera: %d | Retorno: %d\n",
                   p[mejor].id, p[mejor].espera, p[mejor].retorno);
        }
    }

    m->promedio_espera  = total_espera / n;
    m->promedio_retorno = total_retorno / n;
    m->utilizacion_cpu  = 100.0f;
    m->espera_archivo   = 0;

    printf("\nPromedio espera: %.2f | Promedio retorno: %.2f\n",
           m->promedio_espera, m->promedio_retorno);
}
