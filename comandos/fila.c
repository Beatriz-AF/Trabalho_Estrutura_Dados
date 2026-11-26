
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

void inicializarFila(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

int filaVazia(const Fila *fila) {
    return fila->inicio == NULL;
}


int enfileirar(Fila *fila, const char *texto,
              int sequencia, TipoEntidade entidade) {

    if (texto == NULL || texto[0] == '\0' ||
        strlen(texto) >= TAM_COMANDO ||
        sequencia < 0) {
        printf("Erro: comando invalido!\n");
        return 0;
    }

    if (entidade < COMANDO_PESSOA ||
        entidade > COMANDO_PET) {
        printf("Erro: entidade invalida!\n");
        return 0;
    }

    NoComando *novo = malloc(sizeof(NoComando));

    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 0;
    }

    snprintf(novo->texto, sizeof(novo->texto),
             "%s", texto);

    novo->sequencia = sequencia;
    novo->entidade = entidade;
    novo->prox = NULL;

    if (filaVazia(fila)) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }

    return 1;
}


int desenfileirar(Fila *fila, NoComando *resultado) {

    if (filaVazia(fila) || resultado == NULL) {
        return 0;
    }

    NoComando *aux = fila->inicio;

    *resultado = *aux;
    resultado->prox = NULL;

    fila->inicio = aux->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(aux);

    return 1;
}


void listarFila(const Fila *fila) {

    NoComando *aux = fila->inicio;

    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }

    while (aux != NULL) {
        printf("\nSequencia: %d\n", aux->sequencia);
        printf("Comando: %s\n", aux->texto);
        printf("Entidade: %d\n", aux->entidade);

        aux = aux->prox;
    }
}


void liberarFila(Fila *fila) {

    NoComando resultado;

    while (desenfileirar(fila, &resultado)) {
    }
}


int distribuirComandos(Fila *geral,
                      Fila *pessoas,
                      Fila *tipos,
                      Fila *pets) {

    if (geral == NULL || pessoas == NULL ||
        tipos == NULL || pets == NULL) {
        return 0;
    }

    while (!filaVazia(geral)) {

        NoComando *comando = geral->inicio;
        Fila *destino = NULL;

        switch (comando->entidade) {

            case COMANDO_PESSOA:
                destino = pessoas;
                break;

            case COMANDO_TIPO_PET:
                destino = tipos;
                break;

            case COMANDO_PET:
                destino = pets;
                break;

            default:
                printf("Erro: entidade desconhecida!\n");
                return 0;
        }

        if (!enfileirar(destino,
                       comando->texto,
                       comando->sequencia,
                       comando->entidade)) {
            return 0;
        }

        NoComando removido;
        desenfileirar(geral, &removido);
    }

    return 1;
}


int retirarProximoComando(Fila *pessoas,
                          Fila *tipos,
                          Fila *pets,
                          NoComando *resultado) {

    if (pessoas == NULL || tipos == NULL ||
        pets == NULL || resultado == NULL) {
        return 0;
    }

    Fila *proxima = NULL;

    if (!filaVazia(pessoas)) {
        proxima = pessoas;
    }

    if (!filaVazia(tipos)) {

        if (proxima == NULL ||
            tipos->inicio->sequencia <
            proxima->inicio->sequencia) {

            proxima = tipos;
        }
    }

    if (!filaVazia(pets)) {

        if (proxima == NULL ||
            pets->inicio->sequencia <
            proxima->inicio->sequencia) {

            proxima = pets;
        }
    }

    if (proxima == NULL) {
        return 0;
    }

    return desenfileirar(proxima, resultado);
}
