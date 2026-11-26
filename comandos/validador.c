
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "validador.h"

// Ignora espacos, tabs e quebras de linha
static void pularEspacos(const char **p) {
    while (isspace((unsigned char)**p)) {
        (*p)++;
    }
}

// Verifica uma palavra do SQL
static int consumirPalavra(const char **p,
                          const char *palavra) {

    pularEspacos(p);

    const char *atual = *p;
    const char *esperada = palavra;

    while (*esperada != '\0') {
        if (toupper((unsigned char)*atual) !=
            toupper((unsigned char)*esperada)) {
            return 0;
        }

        atual++;
        esperada++;
    }

    if (isalnum((unsigned char)*atual) ||
        *atual == '_') {
        return 0;
    }

    *p = atual;
    return 1;
}

// Verifica um caractere esperado
static int consumirCaractere(const char **p, char esperado) {

    pularEspacos(p);

    if (**p != esperado) {
        return 0;
    }

    (*p)++;
    return 1;
}

// Le um numero inteiro positivo
static int lerCodigo(const char **p, int *codigo) {

    pularEspacos(p);

    char *fim;
    errno = 0;

    long valor = strtol(*p, &fim, 10);

    if (fim == *p || errno == ERANGE ||
        valor <= 0 || valor > INT_MAX) {
        return 0;
    }

    *codigo = (int)valor;
    *p = fim;

    return 1;
}

// Le um texto entre aspas simples
static int lerDescricao(const char **p,
                        char descricao[100]) {

    pularEspacos(p);

    if (**p != '\'') {
        return 0;
    }

    (*p)++;

    int i = 0;

    while (**p != '\0' && **p != '\'') {

        if (i >= 99) {
            return 0;
        }

        descricao[i] = **p;
        i++;
        (*p)++;
    }

    if (**p != '\'' || i == 0) {
        return 0;
    }

    descricao[i] = '\0';
    (*p)++;

    return 1;
}

// Valida INSERT INTO tipo_pet
int validarInsertTipoPet(const char *texto,
                         int *codigo,
                         char descricao[100]) {

    if (texto == NULL || codigo == NULL ||
        descricao == NULL) {
        return 0;
    }

    const char *p = texto;
    int codigoTemp;
    char descricaoTemp[100];

    if (!consumirPalavra(&p, "INSERT") ||
        !consumirPalavra(&p, "INTO") ||
        !consumirPalavra(&p, "TIPO_PET") ||
        !consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ',') ||
        !lerDescricao(&p, descricaoTemp) ||
        !consumirCaractere(&p, ')') ||
        !consumirCaractere(&p, ';')) {

        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0') {
        return 0;
    }

    *codigo = codigoTemp;
    strcpy(descricao, descricaoTemp);

    return 1;
}
