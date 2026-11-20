#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"

#define TAM_LINHA 1024

/* Devolve o proximo campo da linha (separada por ';'). Campo ausente vira "". */
static const char *proximoCampo(char **cur) {
    char *ini = *cur, *fim;

    if (ini == NULL)
        return "";

    fim = strchr(ini, ';');
    if (fim != NULL) {
        *fim = '\0';
        *cur = fim + 1;
    } else {
        *cur = NULL;
    }
    return ini;
}

/* Grava do ultimo para o primeiro: ao carregar (inserindo no inicio),
   a ordem da lista volta exatamente a mesma. */
static void gravaPessoa(FILE *f, const Pessoa *p) {
    if (p == NULL)
        return;
    gravaPessoa(f, p->prox);
    fprintf(f, "%d;%s;%s;%s;%s\n", p->codigo, p->nome, p->fone,
            p->data_nascimento, p->endereco);
}

static void gravaTipoPet(FILE *f, const TipoPet *t) {
    if (t == NULL)
        return;
    gravaTipoPet(f, t->prox);
    fprintf(f, "%d;%s\n", t->codigo, t->descricao);
}

static void gravaPet(FILE *f, const Pet *p) {
    if (p == NULL)
        return;
    gravaPet(f, p->prox);
    fprintf(f, "%d;%d;%s;%d\n", p->codigo, p->codigo_pes, p->nome, p->codigo_tipo);
}

int salvarPessoas(const Pessoa *inicio, const char *arq) {
    FILE *f = fopen(arq, "w");
    if (f == NULL)
        return 0;
    gravaPessoa(f, inicio);
    fclose(f);
    return 1;
}

int carregarPessoas(Pessoa **inicio, const char *arq) {
    FILE *f = fopen(arq, "r");
    char linha[TAM_LINHA];

    if (f == NULL)
        return 1;   /* arquivo ainda nao existe: lista comeca vazia */

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char *cur;
        Pessoa *p;

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0')
            continue;

        p = malloc(sizeof(Pessoa));
        if (p == NULL) {
            fclose(f);
            return 0;
        }

        cur = linha;
        p->codigo = atoi(proximoCampo(&cur));
        snprintf(p->nome, sizeof(p->nome), "%s", proximoCampo(&cur));
        snprintf(p->fone, sizeof(p->fone), "%s", proximoCampo(&cur));
        snprintf(p->data_nascimento, sizeof(p->data_nascimento), "%s", proximoCampo(&cur));
        snprintf(p->endereco, sizeof(p->endereco), "%s", proximoCampo(&cur));

        p->ant = NULL;
        p->prox = *inicio;
        if (*inicio != NULL)
            (*inicio)->ant = p;
        *inicio = p;
    }

    fclose(f);
    return 1;
}

int salvarTiposPet(const TipoPet *inicio, const char *arq) {
    FILE *f = fopen(arq, "w");
    if (f == NULL)
        return 0;
    gravaTipoPet(f, inicio);
    fclose(f);
    return 1;
}

int carregarTiposPet(TipoPet **inicio, const char *arq) {
    FILE *f = fopen(arq, "r");
    char linha[TAM_LINHA];

    if (f == NULL)
        return 1;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char *cur;
        TipoPet *t;

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0')
            continue;

        t = malloc(sizeof(TipoPet));
        if (t == NULL) {
            fclose(f);
            return 0;
        }

        cur = linha;
        t->codigo = atoi(proximoCampo(&cur));
        snprintf(t->descricao, sizeof(t->descricao), "%s", proximoCampo(&cur));

        t->ant = NULL;
        t->prox = *inicio;
        if (*inicio != NULL)
            (*inicio)->ant = t;
        *inicio = t;
    }

    fclose(f);
    return 1;
}

int salvarPets(const Pet *inicio, const char *arq) {
    FILE *f = fopen(arq, "w");
    if (f == NULL)
        return 0;
    gravaPet(f, inicio);
    fclose(f);
    return 1;
}

int carregarPets(Pet **inicio, const char *arq) {
    FILE *f = fopen(arq, "r");
    char linha[TAM_LINHA];

    if (f == NULL)
        return 1;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char *cur;
        Pet *p;

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0')
            continue;

        p = malloc(sizeof(Pet));
        if (p == NULL) {
            fclose(f);
            return 0;
        }

        cur = linha;
        p->codigo = atoi(proximoCampo(&cur));
        p->codigo_pes = atoi(proximoCampo(&cur));
        snprintf(p->nome, sizeof(p->nome), "%s", proximoCampo(&cur));
        p->codigo_tipo = atoi(proximoCampo(&cur));

        p->ant = NULL;
        p->prox = *inicio;
        if (*inicio != NULL)
            (*inicio)->ant = p;
        *inicio = p;
    }

    fclose(f);
    return 1;
}
