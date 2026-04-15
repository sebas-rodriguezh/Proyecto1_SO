# Nombre del ejecutable
TARGET = terminal

# Compilador y banderas
CC     = gcc
CFLAGS = -Wall -Wextra -I./include -pthread

# Todos los .c del proyecto
SRC = main.c \
      src/log.c \
      src/metricas.c \
      src/recursos.c \
      src/camion.c \
      src/planificador.c \
      src/planificador_fifo.c \
      src/planificador_rr.c \
      src/terminal.c

# Regla principal
all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

# Limpiar ejecutable y objetos
clean:
	rm -f $(TARGET)