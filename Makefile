# Especificamos el compilador
CC = gcc
CFLAGS = -Wall -Wextra -pthread -lrt -fPIC

# Generar todos los archivos
all: libclaves.so servidor cliente

# Crear la biblioteca dinámica
libclaves.so: proxy-mq.o claves.o
	$(CC) -shared -o libclaves.so proxy-mq.o claves.o $(CFLAGS)

# Compilar el servidor
servidor: servidor-mq.o claves.o proxy-mq.o
	$(CC) -o servidor servidor-mq.o claves.o proxy-mq.o $(CFLAGS)

# Compilar el cliente
cliente: app-cliente.o claves.o proxy-mq.o
	$(CC) -o cliente app-cliente.o claves.o proxy-mq.o $(CFLAGS)

# Reglas para compilar archivos .c a .o
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Eliminar archivos compilados
clean:
	rm -f *.o servidor cliente libclaves.so