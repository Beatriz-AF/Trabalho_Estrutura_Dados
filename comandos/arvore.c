#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

NoArvore *arvore_inserir(NoArvore *raiz, const char *chave, const char *linha) {
    if (!raiz) {
        NoArvore *no = calloc(1, sizeof *no);
        if (!no) return NULL;
        snprintf(no->chave, sizeof no->chave, "%s", chave);
        snprintf(no->linha, sizeof no->linha, "%s", linha);
        return no;
    }
    /* iguais vao para a direita, mantendo a ordem de insercao entre eles */
    if (strcmp(chave, raiz->chave) < 0)
        raiz->esq = arvore_inserir(raiz->esq, chave, linha);
    else
        raiz->dir = arvore_inserir(raiz->dir, chave, linha);
    return raiz;
}

void arvore_imprimir(const NoArvore *raiz) {
    if (!raiz) return;
    arvore_imprimir(raiz->esq);
    printf("  %s\n", raiz->linha);
    arvore_imprimir(raiz->dir);
}

void arvore_liberar(NoArvore *raiz) {
    if (!raiz) return;
    arvore_liberar(raiz->esq);
    arvore_liberar(raiz->dir);
    free(raiz);
}
