#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"


int inserirArvore(NoArvore **raiz,
                 const char *nome,
                 int codigo) {

    if (nome == NULL || nome[0] == '\0' ||
        strlen(nome) >= 100 || codigo <= 0) {
        printf("Erro: dados invalidos para a arvore!\n");
        return 0;
    }

    if (*raiz == NULL) {

        NoArvore *novo = malloc(sizeof(NoArvore));

        if (novo == NULL) {
            printf("Erro ao alocar memoria!\n");
            return 0;
        }

        snprintf(novo->nome, sizeof(novo->nome),
                 "%s", nome);

        novo->codigo = codigo;
        novo->esquerda = NULL;
        novo->direita = NULL;

        *raiz = novo;
        return 1;
    }

    int comparacao = strcmp(nome, (*raiz)->nome);

    if (comparacao < 0) {
        return inserirArvore(&(*raiz)->esquerda,
                            nome, codigo);
    }

    if (comparacao > 0) {
        return inserirArvore(&(*raiz)->direita,
                            nome, codigo);
    }

    if (codigo < (*raiz)->codigo) {
        return inserirArvore(&(*raiz)->esquerda,
                            nome, codigo);
    }

    if (codigo > (*raiz)->codigo) {
        return inserirArvore(&(*raiz)->direita,
                            nome, codigo);
    }

    printf("Erro: registro ja existe na arvore!\n");
    return 0;
}


NoArvore *buscarArvore(NoArvore *raiz,
                      const char *nome) {

    if (raiz == NULL || nome == NULL) {
        return NULL;
    }

    int comparacao = strcmp(nome, raiz->nome);

    if (comparacao == 0) {
        return raiz;
    }

    if (comparacao < 0) {
        return buscarArvore(raiz->esquerda, nome);
    }

    return buscarArvore(raiz->direita, nome);
}


void percorrerEmOrdem(NoArvore *raiz) {

    if (raiz == NULL) {
        return;
    }

    percorrerEmOrdem(raiz->esquerda);

    printf("Codigo: %d | Nome: %s\n",
           raiz->codigo, raiz->nome);

    percorrerEmOrdem(raiz->direita);
}


void liberarArvore(NoArvore *raiz) {

    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);

    free(raiz);
}


int construirArvorePessoas(NoArvore **raiz, Pessoa *listaPessoas) {

    Pessoa *aux = listaPessoas;

    while (aux != NULL) {

        if (!inserirArvore(raiz, aux->nome, aux->codigo)) {
            return 0;
        }

        aux = aux->prox;
    }

    return 1;
}


int construirArvoreTiposPet(NoArvore **raiz,
                           TipoPet *listaTipos) {

    TipoPet *aux = listaTipos;

    while (aux != NULL) {

        if (!inserirArvore(raiz, aux->descricao,
                          aux->codigo)) {
            return 0;
        }

        aux = aux->prox;
    }

    return 1;
}


int construirArvorePets(NoArvore **raiz,
                       Pet *listaPets) {

    Pet *aux = listaPets;

    while (aux != NULL) {

        if (!inserirArvore(raiz, aux->nome,
                          aux->codigo)) {
            return 0;
        }

        aux = aux->prox;
    }

    return 1;
}

