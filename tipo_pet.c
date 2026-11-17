#include <stdio.h>
#include <stdlib.h>
#include "tipo_pet.h"

TipoPet *buscarTipoPet(TipoPet *inicio, int codigo) {

    TipoPet *aux = inicio;

    while (aux != NULL) {
        if (aux->codigo == codigo)
            return aux;

        aux = aux->prox;
    }

    return NULL;
}

int inserirTipoPet(TipoPet **inicio, int codigo,
                   const char *descricao) {

    if (codigo <= 0 || descricao == NULL ||
        descricao[0] == '\0') {
        printf("Erro: campos invalidos!\n");
        return 0;
    }

    if (buscarTipoPet(*inicio, codigo) != NULL) {
        printf("Erro: codigo ja cadastrado!\n");
        return 0;
    }

    TipoPet *novo = malloc(sizeof(TipoPet));

    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 0;
    }

    novo->codigo = codigo;

    snprintf(novo->descricao,
             sizeof(novo->descricao), "%s", descricao);

    novo->ant = NULL;
    novo->prox = *inicio;

    if (*inicio != NULL) {
        (*inicio)->ant = novo;
    }

    *inicio = novo;

    return 1;
}

int atualizarTipoPet(TipoPet *inicio, int codigo,
                     const char *descricao) {

    TipoPet *aux = buscarTipoPet(inicio, codigo);

    if (aux == NULL) {
        printf("Erro: tipo de pet nao encontrado!\n");
        return 0;
    }

    if (descricao == NULL || descricao[0] == '\0') {
        printf("Erro: descricao invalida!\n");
        return 0;
    }

    snprintf(aux->descricao,
             sizeof(aux->descricao), "%s", descricao);

    return 1;
}

int removerTipoPet(TipoPet **inicio, int codigo) {

    TipoPet *aux = buscarTipoPet(*inicio, codigo);

    if (aux == NULL) {
        printf("Erro: tipo de pet nao encontrado!\n");
        return 0;
    }

    if (aux->ant != NULL) {
        aux->ant->prox = aux->prox;
    } else {
        *inicio = aux->prox;
    }

    if (aux->prox != NULL) {
        aux->prox->ant = aux->ant;
    }

    free(aux);

    return 1;
}

void listarTiposPet(TipoPet *inicio) {

    TipoPet *aux = inicio;

    if (aux == NULL) {
        printf("Nenhum tipo de pet cadastrado.\n");
        return;
    }

    while (aux != NULL) {

        printf("\nCodigo: %d\n", aux->codigo);
        printf("Descricao: %s\n", aux->descricao);

        aux = aux->prox;
    }
}

void liberarTiposPet(TipoPet **inicio) {

    TipoPet *aux = *inicio;

    while (aux != NULL) {
        TipoPet *proximo = aux->prox;
        free(aux);
        aux = proximo;
    }

    *inicio = NULL;
}