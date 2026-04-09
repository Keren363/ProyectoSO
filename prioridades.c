#include <stdio.h>

struct Proceso {
    int id;
    int prioridad;
    int rafaga;
};

void prioridades(struct Proceso p[], int n) {

    // Ordenar por prioridad (menor número = mayor prioridad)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].prioridad < p[i].prioridad) {
                struct Proceso temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("Orden de ejecucion por prioridad:\n");

    for (int i = 0; i < n; i++) {
        printf("Proceso %d (Prioridad %d)\n", p[i].id, p[i].prioridad);
    }
}

int main() {
    struct Proceso procesos[] = {
        {1, 2, 5},
        {2, 1, 3},
        {3, 3, 4}
    };

    prioridades(procesos, 3);

    return 0;
}
