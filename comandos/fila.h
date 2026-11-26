
#ifndef FILA_H
#define FILA_H

#define TAM_COMANDO 512

typedef enum {
    COMANDO_PESSOA,
    COMANDO_TIPO_PET,
    COMANDO_PET
} TipoEntidade;

typedef struct no_comando {
    char texto[TAM_COMANDO];
    int sequencia;
    TipoEntidade entidade;

    struct no_comando *prox;
} NoComando;

typedef struct {
    NoComando *inicio;
    NoComando *fim;
} Fila;

void inicializarFila(Fila *fila);

int filaVazia(const Fila *fila);

int enfileirar(Fila *fila, const char *texto,
              int sequencia, TipoEntidade entidade);

int desenfileirar(Fila *fila, NoComando *resultado);

void listarFila(const Fila *fila);

void liberarFila(Fila *fila);


int distribuirComandos(Fila *geral,
                      Fila *pessoas,
                      Fila *tipos,
                      Fila *pets);

int retirarProximoComando(Fila *pessoas,
                          Fila *tipos,
                          Fila *pets,
                          NoComando *resultado);

#endif
