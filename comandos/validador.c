
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include "validador.h"
#include "leitor.h"

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

static int lerEntidade(const char **p,
                       TipoEntidade *entidade) {

    if (consumirPalavra(p, "PESSOA")) {
        *entidade = COMANDO_PESSOA;

    } else if (consumirPalavra(p, "TIPO_PET")) {
        *entidade = COMANDO_TIPO_PET;

    } else if (consumirPalavra(p, "PET")) {
        *entidade = COMANDO_PET;

    } else {
        return 0;
    }

    return 1;
}


static int fimComando(const char **p) {

    pularEspacos(p);

    return **p == '\0';
}

int validarSelectCompleto(
    const char *texto,
    TipoEntidade *entidade,
    int *temWhere,
    int *codigo,
    int *temOrderBy) {

    if (!texto || !entidade || !temWhere ||
        !codigo || !temOrderBy) {
        return 0;
    }

    const char *p = texto;

    *temWhere = 0;
    *temOrderBy = 0;
    *codigo = 0;

    if (!consumirPalavra(&p, "SELECT") ||
        !consumirCaractere(&p, '*') ||
        !consumirPalavra(&p, "FROM") ||
        !lerEntidade(&p, entidade)) {
        return 0;
    }

    // WHERE opcional.
    if (consumirPalavra(&p, "WHERE")) {

        *temWhere = 1;

        if (!consumirPalavra(&p, "CODIGO") ||
            !consumirCaractere(&p, '=') ||
            !lerCodigo(&p, codigo)) {
            return 0;
        }
    }

    // ORDER BY opcional.
    if (consumirPalavra(&p, "ORDER")) {

        *temOrderBy = 1;

        if (!consumirPalavra(&p, "BY")) {
            return 0;
        }

        if (*entidade == COMANDO_TIPO_PET) {

            if (!consumirPalavra(&p, "DESCRICAO") &&
                !consumirPalavra(&p, "NOME")) {
                return 0;
            }

        } else {

            if (!consumirPalavra(&p, "NOME")) {
                return 0;
            }
        }
    }

    return consumirCaractere(&p, ';') &&
           fimComando(&p);
}


int validarSelectSQL(const char *texto,
                     TipoEntidade *entidade,
                     int *temWhere,
                     int *codigo) {

    int temOrderBy;

    return validarSelectCompleto(
        texto, entidade, temWhere,
        codigo, &temOrderBy) && !temOrderBy;
}


int validarSelectOrderBySQL(const char *texto,
                            TipoEntidade *entidade) {

    int temWhere;
    int codigo;
    int temOrderBy;

    return validarSelectCompleto(
        texto, entidade, &temWhere,
        &codigo, &temOrderBy) &&
        temOrderBy && !temWhere;
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


int validarInsertPetComCampos(
    const char *texto,
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

    // 0: codigo
    // 1: codigo_pes
    // 2: nome
    // 3: codigo_tipo

    int colunas[4];
    int presentes[4] = {0};
    int quantidade = 0;

    int codigoTemp = 0;
    int codigoPesTemp = 0;
    int codigoTipoTemp = 0;
    char nomeTemp[100] = "";

    if (!consumirPalavra(&p, "INSERT") ||
        !consumirPalavra(&p, "INTO") ||
        !consumirPalavra(&p, "PET") ||
        !consumirCaractere(&p, '(')) {
        return 0;
    }

    // Identifica as colunas informadas.
    while (1) {

        int campo = -1;

        if (consumirPalavra(&p, "CODIGO")) {
            campo = 0;

        } else if (consumirPalavra(&p, "CODIGO_PES") ||
                   consumirPalavra(&p, "CODIGO_CLI")) {
            campo = 1;

        } else if (consumirPalavra(&p, "NOME")) {
            campo = 2;

        } else if (consumirPalavra(&p, "CODIGO_TIPO") ||
                   consumirPalavra(&p, "CODIGO_TIPO_PET")) {
            campo = 3;
        }

        if (campo == -1 ||
            presentes[campo] ||
            quantidade >= 4) {
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

    // Todos os campos sao obrigatorios.
    if (quantidade != 4 ||
        !presentes[0] || !presentes[1] ||
        !presentes[2] || !presentes[3]) {
        return 0;
    }

    if (!consumirPalavra(&p, "VALUES") ||
        !consumirCaractere(&p, '(')) {
        return 0;
    }

    // Le os valores na ordem das colunas.
    for (int i = 0; i < quantidade; i++) {

        int valido = 0;

        switch (colunas[i]) {

            case 0:
                valido = lerCodigo(&p, &codigoTemp);
                break;

            case 1:
                valido = lerCodigo(&p, &codigoPesTemp);
                break;

            case 2:
                valido = lerTexto(
                    &p, nomeTemp, sizeof(nomeTemp));
                break;

            case 3:
                valido = lerCodigo(&p, &codigoTipoTemp);
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

    if (*p != '\0' || nomeTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;
    *codigoPes = codigoPesTemp;
    *codigoTipo = codigoTipoTemp;

    strcpy(nome, nomeTemp);

    return 1;
}


int validarUpdatePessoaParcial(
    const char *texto,
    int *codigo,
    unsigned int *campos,
    char nome[100],
    char fone[20],
    char data[11],
    char endereco[150]) {

    if (texto == NULL || codigo == NULL ||
        campos == NULL || nome == NULL ||
        fone == NULL || data == NULL ||
        endereco == NULL) {
        return 0;
    }

    const char *p = texto;

    unsigned int camposTemp = 0;
    int codigoTemp;

    char nomeTemp[100] = "";
    char foneTemp[20] = "";
    char dataTemp[11] = "";
    char enderecoTemp[150] = "";

    if (!consumirPalavra(&p, "UPDATE") ||
        !consumirPalavra(&p, "PESSOA") ||
        !consumirPalavra(&p, "SET")) {
        return 0;
    }

    while (1) {

        unsigned int campo = 0;
        int valido = 0;

        if (consumirPalavra(&p, "NOME")) {
            campo = CAMPO_PESSOA_NOME;

        } else if (consumirPalavra(&p, "FONE")) {
            campo = CAMPO_PESSOA_FONE;

        } else if (consumirPalavra(&p, "DATA_NASCIMENTO")) {
            campo = CAMPO_PESSOA_DATA;

        } else if (consumirPalavra(&p, "ENDERECO")) {
            campo = CAMPO_PESSOA_ENDERECO;

        } else {
            return 0;
        }

        // Impede atualizar o mesmo campo duas vezes.
        if (camposTemp & campo) {
            return 0;
        }

        if (!consumirCaractere(&p, '=')) {
            return 0;
        }

        switch (campo) {

            case CAMPO_PESSOA_NOME:
                valido = lerTexto(
                    &p, nomeTemp, sizeof(nomeTemp));
                break;

            case CAMPO_PESSOA_FONE:
                valido = lerTexto(
                    &p, foneTemp, sizeof(foneTemp));
                break;

            case CAMPO_PESSOA_DATA:
                valido = lerTexto(
                    &p, dataTemp, sizeof(dataTemp));
                break;

            case CAMPO_PESSOA_ENDERECO:
                valido = lerTexto(
                    &p, enderecoTemp, sizeof(enderecoTemp));
                break;
        }

        if (!valido) {
            return 0;
        }

        camposTemp |= campo;

        // Se houver virgula, le outro campo.
        if (consumirCaractere(&p, ',')) {
            continue;
        }

        break;
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

    // Nome nao pode ser vazio.
    if ((camposTemp & CAMPO_PESSOA_NOME) &&
        nomeTemp[0] == '\0') {
        return 0;
    }

    // Se a data foi alterada, precisa ser valida.
    if ((camposTemp & CAMPO_PESSOA_DATA) &&
        !dataValida(dataTemp)) {
        return 0;
    }

    *codigo = codigoTemp;
    *campos = camposTemp;

    strcpy(nome, nomeTemp);
    strcpy(fone, foneTemp);
    strcpy(data, dataTemp);
    strcpy(endereco, enderecoTemp);

    return 1;
}

int validarUpdatePetParcial(
    const char *texto,
    int *codigo,
    unsigned int *campos,
    int *codigoPes,
    char nome[100],
    int *codigoTipo) {

    if (texto == NULL || codigo == NULL ||
        campos == NULL || codigoPes == NULL ||
        nome == NULL || codigoTipo == NULL) {
        return 0;
    }

    const char *p = texto;

    unsigned int camposTemp = 0;

    int codigoTemp;
    int codigoPesTemp = 0;
    int codigoTipoTemp = 0;

    char nomeTemp[100] = "";

    if (!consumirPalavra(&p, "UPDATE") ||
        !consumirPalavra(&p, "PET") ||
        !consumirPalavra(&p, "SET")) {
        return 0;
    }

    while (1) {

        unsigned int campo = 0;
        int valido = 0;

        if (consumirPalavra(&p, "CODIGO_PES") ||
            consumirPalavra(&p, "CODIGO_CLI")) {

            campo = CAMPO_PET_DONO;

        } else if (consumirPalavra(&p, "NOME")) {

            campo = CAMPO_PET_NOME;

        } else if (consumirPalavra(&p, "CODIGO_TIPO") ||
                   consumirPalavra(&p, "CODIGO_TIPO_PET")) {

            campo = CAMPO_PET_TIPO;

        } else {
            return 0;
        }

        // Impede campos repetidos.
        if (camposTemp & campo) {
            return 0;
        }

        if (!consumirCaractere(&p, '=')) {
            return 0;
        }

        switch (campo) {

            case CAMPO_PET_DONO:
                valido = lerCodigo(&p, &codigoPesTemp);
                break;

            case CAMPO_PET_NOME:
                valido = lerTexto(
                    &p, nomeTemp, sizeof(nomeTemp));
                break;

            case CAMPO_PET_TIPO:
                valido = lerCodigo(&p, &codigoTipoTemp);
                break;
        }

        if (!valido) {
            return 0;
        }

        camposTemp |= campo;

        if (consumirCaractere(&p, ',')) {
            continue;
        }

        break;
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

    // O nome do pet nao pode ser vazio.
    if ((camposTemp & CAMPO_PET_NOME) &&
        nomeTemp[0] == '\0') {
        return 0;
    }

    *codigo = codigoTemp;
    *campos = camposTemp;
    *codigoPes = codigoPesTemp;
    *codigoTipo = codigoTipoTemp;

    strcpy(nome, nomeTemp);

    return 1;
}

int validarComandoSQL(const char *texto) {

    TipoOperacao operacao;
    TipoEntidade entidade;

    if (!identificarComando(
            texto, &operacao, &entidade)) {
        return 0;
    }

    int codigo;
    int codigoPes;
    int codigoTipo;
    int temWhere;

    unsigned int campos;

    char nome[100];
    char descricao[100];
    char fone[20];
    char data[11];
    char endereco[150];

    switch (operacao) {

        case OPERACAO_INSERT:

            switch (entidade) {

                case COMANDO_PESSOA:

                    return validarInsertPessoa(
                        texto, &codigo, nome,
                        fone, data, endereco) ||

                        validarInsertPessoaComCampos(
                            texto, &codigo, nome,
                            fone, data, endereco);

                case COMANDO_TIPO_PET:

                    return validarInsertTipoPet(
                        texto, &codigo, descricao) ||

                        validarInsertTipoPetComCampos(
                            texto, &codigo, descricao);

                case COMANDO_PET:

                    return validarInsertPet(
                        texto, &codigo, &codigoPes,
                        nome, &codigoTipo) ||

                        validarInsertPetComCampos(
                            texto, &codigo, &codigoPes,
                            nome, &codigoTipo);

                default:
                    return 0;
            }

        case OPERACAO_UPDATE:

            switch (entidade) {

                case COMANDO_PESSOA:

                    return validarUpdatePessoaParcial(
                        texto, &codigo, &campos,
                        nome, fone, data, endereco);

                case COMANDO_TIPO_PET:

                    return validarUpdateTipoPet(
                        texto, &codigo, descricao);

                case COMANDO_PET:

                    return validarUpdatePetParcial(
                        texto, &codigo, &campos,
                        &codigoPes, nome, &codigoTipo);

                default:
                    return 0;
            }

        case OPERACAO_DELETE:

            return validarDeleteSQL(
                texto, &entidade, &codigo);

        case OPERACAO_SELECT: {

            int temOrderBy;

            return validarSelectCompleto(
                texto, &entidade, &temWhere,
                &codigo, &temOrderBy);
        }

        default:
            return 0;
    }
}

int validarFilaGeral(Fila *fila) {

    if (fila == NULL) {
        return 0;
    }

    NoComando *atual = fila->inicio;
    NoComando *anterior = NULL;

    int validos = 0;
    int invalidos = 0;

    while (atual != NULL) {

        if (!validarComandoSQL(atual->texto)) {

            printf("\nComando invalido: %s\n",
                   atual->texto);

            NoComando *remover = atual;

            atual = atual->prox;

            if (anterior == NULL) {
                fila->inicio = atual;
            } else {
                anterior->prox = atual;
            }

            if (remover == fila->fim) {
                fila->fim = anterior;
            }

            free(remover);

            invalidos++;

        } else {

            validos++;

            anterior = atual;
            atual = atual->prox;
        }
    }

    printf("\n--- RESULTADO DA VALIDACAO ---\n");
    printf("Comandos validos: %d\n", validos);
    printf("Comandos invalidos: %d\n", invalidos);

    return validos;
}
