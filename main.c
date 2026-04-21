#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proceso.h"
#include "archivo_compartido.h"
#include "persistencia.h"

/* ── Ingresar procesos por consola ───────────────────────── */
int ingresar_procesos(Proceso p[]) {
    int n = 0;
    printf("\nCuantos procesos desea ingresar? (max %d): ", MAX_PROCESOS);
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PROCESOS) {
        printf("Usando 3 procesos de ejemplo.\n");
        Proceso ej[] = {
            {1, 0, 5, 5, 2, 0, 0, 0, "productor"},
            {2, 1, 3, 3, 1, 0, 0, 0, "consumidor"},
            {3, 2, 7, 7, 3, 0, 0, 0, "productor"}
        };
        for (int i = 0; i < 3; i++) p[i] = ej[i];
        return 3;
    }

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("\n  Proceso %d:\n", i + 1);
        printf("    Tiempo llegada : "); scanf("%d", &p[i].llegada);
        printf("    Rafaga (CPU)   : "); scanf("%d", &p[i].rafaga);
        printf("    Prioridad      : "); scanf("%d", &p[i].prioridad);
        printf("    Tipo (1=productor / 2=consumidor): ");
        int t; scanf("%d", &t);
        strcpy(p[i].tipo, t == 1 ? "productor" : "consumidor");
        p[i].restante    = p[i].rafaga;
        p[i].finalizacion = 0;
        p[i].espera      = 0;
        p[i].retorno     = 0;
    }
    return n;
}

/* ── Demo archivo compartido ─────────────────────────────── */
void demo_archivo(Proceso p[], int n, Metricas *m) {
    int cap = 0;
    printf("\nCapacidad del archivo compartido: ");
    scanf("%d", &cap);
    if (cap <= 0) cap = 5;

    ArchivoCompartido af;
    inicializar_archivo(&af, cap);

    const char *ops[] = {
        "SUMA:3+7",      "MULT:6*8",       "FACTORIAL:5",
        "PROMEDIO:2,4,6,8", "SUMA:10+25", "MULT:4*9",
        "FACTORIAL:4",   "PROMEDIO:10,20,30"
    };
    int nops = 8, op_idx = 0;
    char resultado[MAX_ITEM_LEN];

    printf("\n--- Simulacion productores / consumidores ---\n");
    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].tipo, "productor") == 0 && op_idx < nops)
            productor_escribir(&af, p[i].id, ops[op_idx++]);
        else if (strcmp(p[i].tipo, "consumidor") == 0 && af.cantidad > 0)
            consumidor_leer(&af, p[i].id, resultado);
    }

    mostrar_archivo(&af);
    m->espera_archivo = (float)af.tiempo_espera_total;
    destruir_archivo(&af);
}

/* ── Menu principal ──────────────────────────────────────── */
int main(void) {
    Proceso  procesos[MAX_PROCESOS];
    Proceso  copia[MAX_PROCESOS];
    Metricas metricas   = {0};
    int      n          = 0;
    char     algoritmo[50] = "Ninguno";
    int      opcion;

    printf("\n+==========================================+\n");
    printf("|  Sistema de Planificacion de Procesos   |\n");
    printf("|  BIS11 Sistemas Operativos - 2026       |\n");
    printf("+==========================================+\n");

    do {
        printf("\n+----------------------------------+\n");
        printf("|         MENU PRINCIPAL           |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Ingresar procesos            |\n");
        printf("|  2. Ejecutar FCFS                |\n");
        printf("|  3. Ejecutar SJN                 |\n");
        printf("|  4. Ejecutar Round Robin         |\n");
        printf("|  5. Prioridades SIN desalojo     |\n");
        printf("|  6. Archivo compartido (demo)    |\n");
        printf("|  7. Ver reporte de metricas      |\n");
        printf("|  8. Guardar estado               |\n");
        printf("| 10. Prioridades CON desalojo     |\n");
        printf("|  0. Salir                        |\n");
        printf("+----------------------------------+\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {

        case 1:
            n = ingresar_procesos(procesos);
            printf("[OK] %d procesos ingresados.\n", n);
            break;

        case 2:
            if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
            for (int i = 0; i < n; i++) copia[i] = procesos[i];
            ejecutar_fcfs(copia, n, &metricas);
            strcpy(algoritmo, "FCFS");
            for (int i = 0; i < n; i++) procesos[i] = copia[i];
            break;

        case 3:
            if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
            for (int i = 0; i < n; i++) copia[i] = procesos[i];
            ejecutar_sjn(copia, n, &metricas);
            strcpy(algoritmo, "SJN");
            for (int i = 0; i < n; i++) procesos[i] = copia[i];
            break;

        case 4: {
            if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
            int q = 2;
            printf("Quantum (ej: 2): ");
            scanf("%d", &q);
            for (int i = 0; i < n; i++) copia[i] = procesos[i];
            ejecutar_rr(copia, n, q, &metricas);
            strcpy(algoritmo, "RoundRobin");
            for (int i = 0; i < n; i++) procesos[i] = copia[i];
            break;
        }

        case 5:
          if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
          for (int i = 0; i < n; i++) copia[i] = procesos[i];
          ejecutar_prioridades(copia, n, &metricas);
          strcpy(algoritmo, "Prioridades-SinDesalojo");
          for (int i = 0; i < n; i++) procesos[i] = copia[i];
          break;

        case 6:
            if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
            demo_archivo(procesos, n, &metricas);
            break;

        case 7:
            if (!n) { printf("[!] No hay datos todavia.\n"); break; }
            mostrar_reporte(procesos, n, &metricas, algoritmo);
            break;

        case 8: {
            if (!n) { printf("[!] No hay datos que guardar.\n"); break; }
            ArchivoCompartido af_tmp;
            inicializar_archivo(&af_tmp, 10);
            guardar_estado(procesos, n, &metricas, &af_tmp, algoritmo);
            destruir_archivo(&af_tmp);
            break;
        }

        case 9: {
            char alg_cargado[50] = "";
            int  n_cargado = 0;
            if (cargar_estado(procesos, &n_cargado, &metricas, alg_cargado)) {
                n = n_cargado;
                strcpy(algoritmo, alg_cargado);
            }
            break;
        }
     
   case 10:
    if (!n) { printf("[!] Primero ingrese procesos.\n"); break; }
    for (int i = 0; i < n; i++) copia[i] = procesos[i];
    ejecutar_prioridades_desalojo(copia, n, &metricas);
    strcpy(algoritmo, "Prioridades-ConDesalojo");
    for (int i = 0; i < n; i++) procesos[i] = copia[i];
    break;

        case 0:
            printf("Hasta luego!\n");
            break;

        default:
            printf("[!] Opcion invalida.\n");
        }

    } while (opcion != 0);

    return 0;
}
