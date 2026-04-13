# Documentación - Sistema de Planificación de Procesos

## Estructuras Principales

### 1. **Proceso** (proceso.h)
Estructura que representa un proceso en el sistema.

**Atributos:**
- `id`: Identificador único del proceso
- `llegada`: Tiempo de llegada del proceso (unidades de tiempo)
- `rafaga`: Tiempo de ráfaga de CPU requerido
- `restante`: Tiempo de ráfaga restante (usado en Round Robin)
- `prioridad`: Nivel de prioridad (número menor = mayor prioridad)
- `finalizacion`: Tiempo en que el proceso finaliza
- `espera`: Tiempo que el proceso estuvo esperando
- `retorno`: Tiempo de retorno (espera + ráfaga)
- `tipo`: Clasificación del proceso ("productor" o "consumidor")

**Constantes:**
- `MAX_PROCESOS = 50`: Máximo número de procesos permitidos

---

### 2. **Metricas** (proceso.h)
Estructura que almacena las métricas de desempeño de un algoritmo de planificación.

**Atributos:**
- `promedio_espera`: Promedio de tiempo de espera de todos los procesos
- `promedio_retorno`: Promedio de tiempo de retorno de todos los procesos
- `utilizacion_cpu`: Porcentaje de utilización del CPU
- `espera_archivo`: Tiempo total que procesos esperaron acceso al archivo compartido

---

### 3. **ArchivoCompartido** (archivo_compartido.h)
Estructura sincronizada para comunicación productor-consumidor con buffer compartido.

**Atributos:**
- `items[][]`: Buffer que almacena operaciones (máx. 100 items, 256 caracteres cada uno)
- `cantidad`: Número actual de ítems en el buffer
- `capacidad`: Capacidad máxima del buffer
- `tiempo_espera_total`: Acumulador de tiempo de espera
- `mutex`: Mutex pthread para sincronización
- `espacios_libres`: Semáforo para espacios disponibles en el buffer
- `items_disponibles`: Semáforo para ítems listos para leer

---

## Módulos de Funcionalidad

### **archivo_compartido.c** - Sincronización Productor-Consumidor

| Función | Descripción |
|---------|-------------|
| `inicializar_archivo()` | Inicializa buffer sincronizado con mutex y semáforos |
| `destruir_archivo()` | Libera recursos (mutex y semáforos) |
| `productor_escribir()` | Escribe operación en buffer (espera espacio disponible) |
| `consumidor_leer()` | Lee y ejecuta operación del buffer (en orden FIFO) |
| `calcular_operacion()` | Evalúa operaciones: SUMA, MULT, FACTORIAL, PROMEDIO |
| `mostrar_archivo()` | Imprime estado actual del buffer compartido |

**Operaciones soportadas:**
- `SUMA:3+7` → 10
- `MULT:6*8` → 48
- `FACTORIAL:5` → 120
- `PROMEDIO:2,4,6` → 4.0

---

### **persistencia.c** - Guardado y Carga de Estado

| Función | Descripción |
|---------|-------------|
| `guardar_estado()` | Persiste procesos, métricas y algoritmo a archivo binario |
| `cargar_estado()` | Lee estado previamente guardado desde archivo |
| `mostrar_reporte()` | Imprime reporte detallado con todas las métricas |

---

## Algoritmos de Planificación

### **fcfs.c** - First Come First Served
- **Descripción:** Procesos se ejecutan en orden de llegada
- **Ordenamiento:** Por tiempo de llegada
- **Complejidad:** O(n)
- **Función:** `ejecutar_fcfs(Proceso p[], int n, Metricas *m)`

### **sjn.c** - Shortest Job Next
- **Descripción:** Procesos se ejecutan por tiempo de ráfaga menor primero
- **Ordenamiento:** Por ráfaga ascendente (bubble sort)
- **Complejidad:** O(n²)
- **Función:** `ejecutar_sjn(Proceso p[], int n, Metricas *m)`

### **round_robin.c** - Round Robin
- **Descripción:** Procesos comparten CPU en rodadas con tiempo fijo (quantum)
- **Características:** Ejecuta en tiempo real, reinicia procesos que no finalizan
- **Parámetro:** `quantum` (tiempo máximo por turno)
- **Complejidad:** O(n * número_de_rodadas)
- **Función:** `ejecutar_rr(Proceso p[], int n, int quantum, Metricas *m)`

### **prioridades.c** - Priority Scheduling
- **Descripción:** Procesos se ejecutan según nivel de prioridad (menor número = mayor prioridad)
- **Ordenamiento:** Por número de prioridad ascendente
- **Complejidad:** O(n²)
- **Función:** `ejecutar_prioridades(Proceso p[], int n, Metricas *m)`

---

## Flujo Principal (main.c)

**Funciones principales:**
- `ingresar_procesos()`: Lee procesos del usuario o carga ejemplos por defecto
- `demo_archivo()`: Simula interacción productor-consumidor
- `main()`: Menú interactivo con 10 opciones

**Opciones del menú:**
1. Ingresar procesos
2. Ejecutar FCFS
3. Ejecutar SJN
4. Ejecutar Round Robin (requiere quantum)
5. Ejecutar Prioridades
6. Demostración archivo compartido
7. Ver reporte de métricas
8. Guardar estado
9. Cargar estado
0. Salir

---

## Constantes del Sistema

| Constante | Valor | Ubicación |
|-----------|-------|-----------|
| `MAX_PROCESOS` | 50 | proceso.h |
| `MAX_CAPACIDAD` | 100 | archivo_compartido.h |
| `MAX_ITEM_LEN` | 256 | archivo_compartido.h |
| `ARCHIVO_ESTADO` | "estado_simulacion.dat" | persistencia.h |

---

## Flujo de Datos

```
main.c
  ├─→ ingresar_procesos() → Proceso[]
  ├─→ ejecutar_fcfs/sjn/rr/prioridades() → Metricas
  ├─→ demo_archivo() → ArchivoCompartido
  │    ├─→ productor_escribir()
  │    └─→ consumidor_leer()
  └─→ persistencia:guardar/cargar_estado()
```

---

## Compilación

```makefile
gcc -pthread -o simulador *.c
```

Requiere: pthread (sincronización)
