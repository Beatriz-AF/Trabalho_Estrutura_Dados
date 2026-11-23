#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include "validador.h"

typedef struct Comando {
    int seq;
    Tabela tabela;
    char descricao[MAX_LINHA];
    struct Comando *prox;
} Comando;

typedef struct {
    Comando *ini, *fim;
    int tam;
    int seq_prox;
} Fila;

void fila_criar(Fila *f);
int fila_vazia(const Fila *f);
int fila_enfileirar(Fila *f, const char *descricao);
void fila_anexar(Fila *f, Comando *no);
Comando *fila_desenfileirar(Fila *f);
void fila_liberar(Fila *f);
int ler_comandos(FILE *arq, Fila *f);

#endif
