
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "validador.h"

static void pularEspacos(const char **p) {
    while (isspace((unsigned char)**p)) {
        (*p)++;
    }
}

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


static int consumirCaractere(const char **p, char esperado) {

    pularEspacos(p);

    if (**p != esperado) {
        return 0;
    }

    (*p)++;
    return 1;
}

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


static int lerTexto(const char **p,
                    char *destino,
                    size_t tamanho) {

    pularEspacos(p);

    if (**p != '\'' || tamanho == 0) {
        return 0;
    }

    (*p)++;

    size_t i = 0;

    while (**p != '\0' && **p != '\'') {

        if (i >= tamanho - 1) {
            return 0;
        }

        destino[i] = **p;
        i++;
        (*p)++;
    }

    if (**p != '\'') {
        return 0;
    }

    destino[i] = '\0';
    (*p)++;

    return 1;
}

int validarInsertPessoa(const char *texto,
                        int *codigo,
                        char nome[100],
                        char fone[20],
                        char data[11],
                        char endereco[150]) {

    if (texto == NULL || codigo == NULL ||
        nome == NULL || fone == NULL ||
        data == NULL || endereco == NULL) {
        return 0;
    }

    const char *p = texto;

    int codigoTemp;
    char nomeTemp[100];
    char foneTemp[20];
    char dataTemp[11];
    char enderecoTemp[150];

    if (!consumirPalavra(&p, "INSERT") ||
        !consumirPalavra(&p, "INTO") ||
        !consumirPalavra(&p, "PESSOA") ||
        !consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, nomeTemp, sizeof(nomeTemp)) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, foneTemp, sizeof(foneTemp)) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, dataTemp, sizeof(dataTemp)) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, enderecoTemp, sizeof(enderecoTemp)) ||
        !consumirCaractere(&p, ')') ||
        !consumirCaractere(&p, ';')) {

        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0') {
        return 0;
    }

    if (nomeTemp[0] == '\0' ||
        dataTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;

    strcpy(nome, nomeTemp);
    strcpy(fone, foneTemp);
    strcpy(data, dataTemp);
    strcpy(endereco, enderecoTemp);

    return 1;
}


int validarInsertPet(const char *texto,
                     int *codigo,
                     int *codigoPes,
                     char nome[100],
                     int *codigoTipo) {

    if (texto == NULL || codigo == NULL ||
        codigoPes == NULL || nome == NULL ||
        codigoTipo == NULL) {
        return 0;
    }

    const char *p = texto;

    int codigoTemp;
    int codigoPesTemp;
    int codigoTipoTemp;
    char nomeTemp[100];

    if (!consumirPalavra(&p, "INSERT") ||
        !consumirPalavra(&p, "INTO") ||
        !consumirPalavra(&p, "PET") ||
        !consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ',') ||
        !lerCodigo(&p, &codigoPesTemp) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, nomeTemp, sizeof(nomeTemp)) ||
        !consumirCaractere(&p, ',') ||
        !lerCodigo(&p, &codigoTipoTemp) ||
        !consumirCaractere(&p, ')') ||
        !consumirCaractere(&p, ';')) {

        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0' || nomeTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;
    *codigoPes = codigoPesTemp;
    *codigoTipo = codigoTipoTemp;

    strcpy(nome, nomeTemp);

    return 1;
}


int validarDeleteSQL(const char *texto,
                     TipoEntidade *entidade,
                     int *codigo) {

    if (texto == NULL || entidade == NULL ||
        codigo == NULL) {
        return 0;
    }

    const char *p = texto;
    TipoEntidade entidadeTemp;
    int codigoTemp;

    if (!consumirPalavra(&p, "DELETE") ||
        !consumirPalavra(&p, "FROM")) {
        return 0;
    }

    if (consumirPalavra(&p, "PESSOA")) {
        entidadeTemp = COMANDO_PESSOA;

    } else if (consumirPalavra(&p, "TIPO_PET")) {
        entidadeTemp = COMANDO_TIPO_PET;

    } else if (consumirPalavra(&p, "PET")) {
        entidadeTemp = COMANDO_PET;

    } else {
        return 0;
    }

    if (!consumirPalavra(&p, "WHERE") ||
        !consumirPalavra(&p, "CODIGO") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ';')) {
        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0') {
        return 0;
    }

    *entidade = entidadeTemp;
    *codigo = codigoTemp;

    return 1;
}


int validarUpdateTipoPet(const char *texto,
                         int *codigo,
                         char descricao[100]) {

    if (texto == NULL || codigo == NULL ||
        descricao == NULL) {
        return 0;
    }

    const char *p = texto;

    int codigoTemp;
    char descricaoTemp[100];

    if (!consumirPalavra(&p, "UPDATE") ||
        !consumirPalavra(&p, "TIPO_PET") ||
        !consumirPalavra(&p, "SET") ||
        !consumirPalavra(&p, "DESCRICAO") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, descricaoTemp, sizeof(descricaoTemp)) ||
        !consumirPalavra(&p, "WHERE") ||
        !consumirPalavra(&p, "CODIGO") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ';')) {

        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0' || descricaoTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;
    strcpy(descricao, descricaoTemp);

    return 1;
}


int validarUpdatePessoa(const char *texto,
                        int *codigo,
                        char nome[100],
                        char fone[20],
                        char data[11],
                        char endereco[150]) {

    if (texto == NULL || codigo == NULL ||
        nome == NULL || fone == NULL ||
        data == NULL || endereco == NULL) {
        return 0;
    }

    const char *p = texto;

    int codigoTemp;
    char nomeTemp[100];
    char foneTemp[20];
    char dataTemp[11];
    char enderecoTemp[150];

    if (!consumirPalavra(&p, "UPDATE") ||
        !consumirPalavra(&p, "PESSOA") ||
        !consumirPalavra(&p, "SET") ||

        !consumirPalavra(&p, "NOME") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, nomeTemp, sizeof(nomeTemp)) ||
        !consumirCaractere(&p, ',') ||

        !consumirPalavra(&p, "FONE") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, foneTemp, sizeof(foneTemp)) ||
        !consumirCaractere(&p, ',') ||

        !consumirPalavra(&p, "DATA_NASCIMENTO") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, dataTemp, sizeof(dataTemp)) ||
        !consumirCaractere(&p, ',') ||

        !consumirPalavra(&p, "ENDERECO") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, enderecoTemp, sizeof(enderecoTemp)) ||

        !consumirPalavra(&p, "WHERE") ||
        !consumirPalavra(&p, "CODIGO") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ';')) {

        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0' ||
        nomeTemp[0] == '\0' ||
        dataTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;

    strcpy(nome, nomeTemp);
    strcpy(fone, foneTemp);
    strcpy(data, dataTemp);
    strcpy(endereco, enderecoTemp);

    return 1;
}


