#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include "validador.h"

/* No da fila de comandos (struct comando) */
typedef struct Comando {
    int seq;                    /* posicao original no script (ordem de execucao) */
    Tabela tabela;              /* tabela alvo, preenchido apos a validacao */
    char descricao[MAX_LINHA];  /* texto do comando */
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
