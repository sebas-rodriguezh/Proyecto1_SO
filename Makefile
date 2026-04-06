# Nombre del ejecutable final
TARGET = programa_modular

# Compilador y banderas
CC = gcc
CFLAGS = -I./include -pthread
SRC = main.c src/algo.c

# Regla principal: qué hacer cuando escribas "make"
all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET)