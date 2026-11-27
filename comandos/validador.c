
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "validador.h"

static int dataValida(const char *data);
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
    !dataValida(dataTemp)) {
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


int validarUpdatePet(const char *texto,
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

    if (!consumirPalavra(&p, "UPDATE") ||
        !consumirPalavra(&p, "PET") ||
        !consumirPalavra(&p, "SET") ||

        !consumirPalavra(&p, "CODIGO_PES") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoPesTemp) ||
        !consumirCaractere(&p, ',') ||

        !consumirPalavra(&p, "NOME") ||
        !consumirCaractere(&p, '=') ||
        !lerTexto(&p, nomeTemp, sizeof(nomeTemp)) ||
        !consumirCaractere(&p, ',') ||

        !consumirPalavra(&p, "CODIGO_TIPO") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoTipoTemp) ||

        !consumirPalavra(&p, "WHERE") ||
        !consumirPalavra(&p, "CODIGO") ||
        !consumirCaractere(&p, '=') ||
        !lerCodigo(&p, &codigoTemp) ||
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


int validarSelectSQL(const char *texto,
                     TipoEntidade *entidade,
                     int *temWhere,
                     int *codigo) {

    if (texto == NULL || entidade == NULL ||
        temWhere == NULL || codigo == NULL) {
        return 0;
    }

    const char *p = texto;

    TipoEntidade entidadeTemp;
    int temWhereTemp = 0;
    int codigoTemp = 0;

    if (!consumirPalavra(&p, "SELECT") ||
        !consumirCaractere(&p, '*') ||
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

    pularEspacos(&p);

    if (consumirPalavra(&p, "WHERE")) {

        temWhereTemp = 1;

        if (!consumirPalavra(&p, "CODIGO") ||
            !consumirCaractere(&p, '=') ||
            !lerCodigo(&p, &codigoTemp)) {
            return 0;
        }
    }

    if (!consumirCaractere(&p, ';')) {
        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0') {
        return 0;
    }

    *entidade = entidadeTemp;
    *temWhere = temWhereTemp;
    *codigo = codigoTemp;

    return 1;
}


int validarSelectOrderBySQL(const char *texto,
                            TipoEntidade *entidade) {

    if (texto == NULL || entidade == NULL) {
        return 0;
    }

    const char *p = texto;
    TipoEntidade entidadeTemp;

    if (!consumirPalavra(&p, "SELECT") ||
        !consumirCaractere(&p, '*') ||
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

    if (!consumirPalavra(&p, "ORDER") ||
        !consumirPalavra(&p, "BY")) {
        return 0;
    }

    if (entidadeTemp == COMANDO_TIPO_PET) {
        if (!consumirPalavra(&p, "DESCRICAO") &&
            !consumirPalavra(&p, "NOME")) {
            return 0;
        }
    } else {
        if (!consumirPalavra(&p, "NOME")) {
            return 0;
        }
    }

    if (!consumirCaractere(&p, ';')) {
        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0') {
        return 0;
    }

    *entidade = entidadeTemp;
    return 1;
}


int validarInsertTipoPetComCampos(const char *texto,
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

        !consumirCaractere(&p, '(') ||
        !consumirPalavra(&p, "CODIGO") ||
        !consumirCaractere(&p, ',') ||
        !consumirPalavra(&p, "DESCRICAO") ||
        !consumirCaractere(&p, ')') ||

        !consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(') ||
        !lerCodigo(&p, &codigoTemp) ||
        !consumirCaractere(&p, ',') ||
        !lerTexto(&p, descricaoTemp,
                  sizeof(descricaoTemp)) ||
        !consumirCaractere(&p, ')') ||
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


static int dataValida(const char *data) {

    if (strlen(data) != 10 ||
        data[2] != '/' || data[5] != '/') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            continue;
        }

        if (!isdigit((unsigned char)data[i])) {
            return 0;
        }
    }

    int dia = (data[0] - '0') * 10 +
              (data[1] - '0');

    int mes = (data[3] - '0') * 10 +
              (data[4] - '0');

    int ano = (data[6] - '0') * 1000 +
              (data[7] - '0') * 100 +
              (data[8] - '0') * 10 +
              (data[9] - '0');

    if (ano < 1 || mes < 1 || mes > 12) {
        return 0;
    }

    int diasMes[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    int bissexto = (ano % 400 == 0) ||
                   (ano % 4 == 0 && ano % 100 != 0);

    if (bissexto) {
        diasMes[1] = 29;
    }

    return dia >= 1 && dia <= diasMes[mes - 1];
}


int validarInsertPessoaComCampos(
    const char *texto,
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

    // 0: codigo, 1: nome, 2: fone,
    // 3: data_nascimento, 4: endereco
    int colunas[5];
    int presentes[5] = {0};
    int quantidade = 0;

    int codigoTemp = 0;
    char nomeTemp[100] = "";
    char foneTemp[20] = "";
    char dataTemp[11] = "";
    char enderecoTemp[150] = "";

    if (!consumirPalavra(&p, "INSERT") ||
        !consumirPalavra(&p, "INTO") ||
        !consumirPalavra(&p, "PESSOA") ||
        !consumirCaractere(&p, '(')) {
        return 0;
    }

    // Lê a lista de nomes das colunas.
    while (1) {

        int campo = -1;

        if (consumirPalavra(&p, "CODIGO")) {
            campo = 0;
        } else if (consumirPalavra(&p, "NOME")) {
            campo = 1;
        } else if (consumirPalavra(&p, "FONE")) {
            campo = 2;
        } else if (consumirPalavra(&p, "DATA_NASCIMENTO")) {
            campo = 3;
        } else if (consumirPalavra(&p, "ENDERECO")) {
            campo = 4;
        }

        if (campo == -1 || presentes[campo] ||
            quantidade >= 5) {
            return 0;
        }

        presentes[campo] = 1;
        colunas[quantidade++] = campo;

        if (consumirCaractere(&p, ')')) {
            break;
        }

        if (!consumirCaractere(&p, ',')) {
            return 0;
        }
    }

    // Verifica os campos obrigatorios.
    if (!presentes[0] || !presentes[1] ||
        !presentes[3]) {
        return 0;
    }

    if (!consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(')) {
        return 0;
    }

    // Lê cada valor na ordem das colunas.
    for (int i = 0; i < quantidade; i++) {

        int valido = 0;

        switch (colunas[i]) {

            case 0:
                valido = lerCodigo(&p, &codigoTemp);
                break;

            case 1:
                valido = lerTexto(
                    &p, nomeTemp, sizeof(nomeTemp));
                break;

            case 2:
                valido = lerTexto(
                    &p, foneTemp, sizeof(foneTemp));
                break;

            case 3:
                valido = lerTexto(
                    &p, dataTemp, sizeof(dataTemp));
                break;

            case 4:
                valido = lerTexto(
                    &p, enderecoTemp, sizeof(enderecoTemp));
                break;
        }

        if (!valido) {
            return 0;
        }

        if (i < quantidade - 1 &&
            !consumirCaractere(&p, ',')) {
            return 0;
        }
    }

    if (!consumirCaractere(&p, ')') ||
        !consumirCaractere(&p, ';')) {
        return 0;
    }

    pularEspacos(&p);

    if (*p != '\0' ||
        nomeTemp[0] == '\0' ||
        !dataValida(dataTemp)) {
        return 0;
    }

    *codigo = codigoTemp;

    strcpy(nome, nomeTemp);
    strcpy(fone, foneTemp);
    strcpy(data, dataTemp);
    strcpy(endereco, enderecoTemp);

    return 1;
}

