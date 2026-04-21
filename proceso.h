#ifndef PROCESO_H
#define PROCESO_H

#define MAX_PROCESOS 50

/* Estructura unificada de proceso */
typedef struct {
    int  id;
    int  llegada;
    int  rafaga;
    int  restante;
    int  prioridad;
    int  finalizacion;
    int  espera;
    int  retorno;
    char tipo[12];    /* "productor" o "consumidor" */
} Proceso;

/* Metricas de un algoritmo de planificacion */
typedef struct {
    float promedio_espera;
    float promedio_retorno;
    float utilizacion_cpu;
    float espera_archivo;
} Metricas;

/* Declaraciones de algoritmos */
void ejecutar_fcfs(Proceso p[], int n, Metricas *m);
void ejecutar_sjn(Proceso p[], int n, Metricas *m);
void ejecutar_rr(Proceso p[], int n, int quantum, Metricas *m);
void ejecutar_prioridades(Proceso p[], int n, Metricas *m);
void ejecutar_prioridades_desalojo(Proceso p[], int n, Metricas *m);

#endif
