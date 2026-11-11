
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"

Pessoa *buscarPessoa(Pessoa *inicio, int codigo) {
    Pessoa *aux = inicio;

    while (aux != NULL) {
        if (aux->codigo == codigo)
            return aux;

        aux = aux->prox;
    }

    return NULL;
}

int inserirPessoa(Pessoa **inicio, int codigo,
                  const char *nome, const char *fone,
                  const char *data, const char *endereco) {

    if (codigo <= 0 || nome == NULL || nome[0] == '\0' ||
        data == NULL || data[0] == '\0') {
        printf("Erro: campos obrigatorios ausentes!\n");
        return 0;
    }

    if (buscarPessoa(*inicio, codigo) != NULL) {
        printf("Erro: codigo ja cadastrado!\n");
        return 0;
    }

    Pessoa *nova = malloc(sizeof(Pessoa));

    if (nova == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 0;
    }

    nova->codigo = codigo;

    snprintf(nova->nome, sizeof(nova->nome), "%s", nome);
    snprintf(nova->fone, sizeof(nova->fone), "%s",
             fone != NULL ? fone : "");
    snprintf(nova->data_nascimento,
             sizeof(nova->data_nascimento), "%s", data);
    snprintf(nova->endereco, sizeof(nova->endereco), "%s",
             endereco != NULL ? endereco : "");

    nova->ant = NULL;
    nova->prox = *inicio;

    if (*inicio != NULL) {
        (*inicio)->ant = nova;
    }

    *inicio = nova;

    printf("Pessoa cadastrada com sucesso!\n");
    return 1;
}

void listarPessoas(Pessoa *inicio) {
    Pessoa *aux = inicio;

    if (aux == NULL) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }

    while (aux != NULL) {
        printf("\nCodigo: %d\n", aux->codigo);
        printf("Nome: %s\n", aux->nome);
        printf("Telefone: %s\n", aux->fone);
        printf("Nascimento: %s\n", aux->data_nascimento);
        printf("Endereco: %s\n", aux->endereco);

        aux = aux->prox;
    }
}

void liberarPessoas(Pessoa **inicio) {
    Pessoa *aux = *inicio;

    while (aux != NULL) {
        Pessoa *proximo = aux->prox;
        free(aux);
        aux = proximo;
    }

    *inicio = NULL;
}
