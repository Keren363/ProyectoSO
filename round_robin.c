#include <stdio.h>

struct Proceso {
    int id;
    int llegada;
    int rafaga;
    int restante;
    int finalizacion;
};

void roundRobin(struct Proceso p[], int n, int quantum) {
    int tiempo = 0;
    int completados = 0;

    while (completados < n) {
        int ejecutado = 0;

        for (int i = 0; i < n; i++) {
            if (p[i].llegada <= tiempo && p[i].restante > 0) {

                ejecutado = 1;

                if (p[i].restante > quantum) {
                    printf("Tiempo %d: Proceso %d ejecuta %d\n", tiempo, p[i].id, quantum);
                    tiempo += quantum;
                    p[i].restante -= quantum;
                } else {
                    printf("Tiempo %d: Proceso %d termina\n", tiempo, p[i].id);
                    tiempo += p[i].restante;
                    p[i].restante = 0;
                    p[i].finalizacion = tiempo;
                    completados++;
                }
            }
        }

        if (!ejecutado) tiempo++;
    }

    printf("\nResultados:\n");

    for (int i = 0; i < n; i++) {
        int turnaround = p[i].finalizacion - p[i].llegada;
        int espera = turnaround - p[i].rafaga;

        printf("Proceso %d -> Espera: %d | Retorno: %d\n",
               p[i].id, espera, turnaround);
    }
}

int main() {
    struct Proceso procesos[] = {
        {1, 0, 5, 5, 0},
        {2, 1, 3, 3, 0},
        {3, 2, 7, 7, 0}
    };

    int n = 3;
    int quantum = 2;

    roundRobin(procesos, n, quantum);

    return 0;
}
