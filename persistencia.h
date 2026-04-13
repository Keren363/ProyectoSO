#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "proceso.h"
#include "archivo_compartido.h"

#define ARCHIVO_ESTADO "estado_simulacion.dat"

void guardar_estado(Proceso p[], int n, Metricas *m,
                    ArchivoCompartido *af, const char *algoritmo);
int  cargar_estado(Proceso p[], int *n, Metricas *m,
                   char *algoritmo_out);
void mostrar_reporte(Proceso p[], int n, Metricas *m,
                     const char *algoritmo);

#endif
