#include <stdio.h>

struct Proceso {
    int id;
    int llegada;
    int rafaga;
};

void fcfs(struct Proceso p[], int n) {
    int tiempo = 0;

    printf("Ejecucion FCFS:\n");

    for (int i = 0; i < n; i++) {

        if (tiempo < p[i].llegada)
            tiempo = p[i].llegada;

        int espera = tiempo - p[i].llegada;
        int retorno = espera + p[i].rafaga;

        printf("Proceso %d -> Espera: %d | Retorno: %d\n",
               p[i].id, espera, retorno);

        tiempo += p[i].rafaga;
    }
}

int main() {
    struct Proceso procesos[] = {
        {1, 0, 5},
        {2, 1, 3},
        {3, 2, 7}
    };

    fcfs(procesos, 3);

    return 0;
}
