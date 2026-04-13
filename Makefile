CC      = gcc
CFLAGS  = -Wall -Wextra -g
LDFLAGS = -lpthread

SRCS = main.c fcfs.c sjn.c round_robin.c prioridades.c \
       archivo_compartido.c persistencia.c

TARGET = simulador

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o estado_simulacion.dat

run: all
	./$(TARGET)

.PHONY: all clean run
