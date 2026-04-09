#include <stdio.h>

struct Proceso {
    int id;
    int rafaga;
};

void sjn(struct Proceso p[], int n) {

    // Ordenar por menor rafaga
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[j].rafaga < p[i].rafaga) {
                struct Proceso temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("Ejecucion SJN:\n");

    int tiempo = 0;

    for (int i = 0; i < n; i++) {
        printf("Proceso %d ejecuta (%d)\n", p[i].id, p[i].rafaga);
        tiempo += p[i].rafaga;
    }
}

int main() {
    struct Proceso procesos[] = {
        {1, 5},
        {2, 3},
        {3, 7}
    };

    sjn(procesos, 3);

    return 0;
}
