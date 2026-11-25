
#include <stdio.h>
#include <stdlib.h>
#include "pet.h"

Pet *buscarPet(Pet *inicio, int codigo) {

    Pet *aux = inicio;

    while (aux != NULL) {

        if (aux->codigo == codigo) {
            return aux;
        }

        aux = aux->prox;
    }

    return NULL;
}


int inserirPet(Pet **inicio, int codigo, int codigoPes,
               const char *nome, int codigoTipo) {

    if (codigo <= 0 || codigoPes <= 0 ||
        codigoTipo <= 0 || nome == NULL ||
        nome[0] == '\0') {

        printf("Erro: dados invalidos!\n");
        return 0;
    }

    if (buscarPet(*inicio, codigo) != NULL) {
        printf("Erro: codigo do pet ja cadastrado!\n");
        return 0;
    }

    Pet *novo = malloc(sizeof(Pet));

    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 0;
    }

    novo->codigo = codigo;
    novo->codigo_pes = codigoPes;
    snprintf(novo->nome, sizeof(novo->nome), "%s", nome);
    novo->codigo_tipo = codigoTipo;

    novo->ant = NULL;
    novo->prox = *inicio;

    if (*inicio != NULL) {
        (*inicio)->ant = novo;
    }

    *inicio = novo;

    return 1;
}


int atualizarPet(Pet *inicio, int codigo, int codigoPes,
                 const char *nome, int codigoTipo) {

    Pet *aux = buscarPet(inicio, codigo);

    if (aux == NULL) {
        printf("Erro: pet nao encontrado!\n");
        return 0;
    }

    if (nome != NULL && nome[0] == '\0') {
        printf("Erro: nome invalido!\n");
        return 0;
    }

    if (codigoPes == 0 || codigoTipo == 0) {
        printf("Erro: codigo de dono ou tipo invalido!\n");
        return 0;
    }

    if (codigoPes > 0) {
        aux->codigo_pes = codigoPes;
    }

    if (nome != NULL) {
        snprintf(aux->nome, sizeof(aux->nome), "%s", nome);
    }

    if (codigoTipo > 0) {
        aux->codigo_tipo = codigoTipo;
    }

    return 1;
}


int removerPet(Pet **inicio, int codigo) {

    Pet *aux = buscarPet(*inicio, codigo);

    if (aux == NULL) {
        printf("Erro: pet nao encontrado!\n");
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


void listarPets(Pet *inicio) {

    Pet *aux = inicio;

    if (aux == NULL) {
        printf("Nenhum pet cadastrado.\n");
        return;
    }

    while (aux != NULL) {
        printf("\nCodigo: %d\n", aux->codigo);
        printf("Nome: %s\n", aux->nome);
        printf("Codigo do dono: %d\n", aux->codigo_pes);
        printf("Codigo do tipo: %d\n", aux->codigo_tipo);

        aux = aux->prox;
    }
}


void liberarPets(Pet **inicio) {

    Pet *aux = *inicio;

    while (aux != NULL) {
        Pet *proximo = aux->prox;

        free(aux);

        aux = proximo;
    }

    *inicio = NULL;
}

