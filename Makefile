CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Icomandos -Icadastro
VPATH = comandos:cadastro

# parte de comandos (fila, leitura, validacao, processamento)
OBJ_COMANDOS = processador.o fila.o validador.o arvore.o leitor.o
# parte de cadastro (pessoa e tipo_pet da Beatriz + pet e persistencia)
OBJ_CADASTRO = pessoa.o tipo_pet.o pet.o persistencia.o

programa: main.o $(OBJ_COMANDOS) $(OBJ_CADASTRO)
	$(CC) $(CFLAGS) -o programa main.o $(OBJ_COMANDOS) $(OBJ_CADASTRO)

test_validador: test_validador.o validador.o
	$(CC) $(CFLAGS) -o test_validador test_validador.o validador.o

test: test_validador
	./test_validador

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o programa test_validador
