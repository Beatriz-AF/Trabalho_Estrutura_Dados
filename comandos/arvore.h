#ifndef ARVORE_H
#define ARVORE_H

#include "validador.h"

/* Arvore binaria de busca usada para o select ... order by nome */
typedef struct NoArvore {
    char chave[MAX_TEXTO];
    char linha[MAX_LINHA];
    struct NoArvore *esq, *dir;
} NoArvore;

NoArvore *arvore_inserir(NoArvore *raiz, const char *chave, const char *linha);
void arvore_imprimir(const NoArvore *raiz);   /* in-order: sai ordenado */
void arvore_liberar(NoArvore *raiz);

#endif
