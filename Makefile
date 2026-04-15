TARGET = proyecto_so

CC = gcc
CFLAGS = -Wall -Wextra -I./include -pthread

SRC = main.c src/log.c src/metricas.c src/recursos.c src/camion.c src/planificador.c src/planificador_fifo.c src/planificador_rr.c src/terminal.c

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)