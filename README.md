# Sistema de Planificacion de Procesos

**Curso:** BIS11 Sistemas Operativos  
**Universidad:** Latina de Costa Rica  
**Anio:** 2026

## Compilar
```
make
```

## Ejecutar
```
./simulador
```

## Limpiar binarios
```
make clean
```

## Estructura del proyecto
```
ProyectoSO/
├── proceso.h              <- Estructura unificada de proceso y metricas
├── fcfs.c                 <- Algoritmo FCFS
├── sjn.c                  <- Algoritmo SJN
├── round_robin.c          <- Algoritmo Round Robin
├── prioridades.c          <- Algoritmo por Prioridades
├── archivo_compartido.h   <- Header del archivo compartido
├── archivo_compartido.c   <- Productores, consumidores y semaforos
├── persistencia.h         <- Header de persistencia
├── persistencia.c         <- Guardar y cargar estado
├── main.c                 <- Menu principal
├── Makefile               <- Compilacion
└── README.md              <- Este archivo
```

## Algoritmos implementados
- **FCFS** - First Come First Served
- **SJN**  - Shortest Job Next
- **Round Robin** - Quantum configurable por el usuario
- **Prioridades** - Menor numero = mayor prioridad

## Funcionalidades
- Archivo compartido con productores y consumidores
- Sincronizacion con semaforos y mutex (POSIX)
- Calculos matematicos: SUMA, MULT, FACTORIAL, PROMEDIO
- Persistencia: guardar y cargar estado de simulacion
- Metricas: promedio de espera, retorno, utilizacion CPU
