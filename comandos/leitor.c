
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "leitor.h"

int identificarComando(const char *texto,
                      TipoOperacao *operacao,
                      TipoEntidade *entidade) {

    char palavra1[32] = "";
    char palavra2[32] = "";
    char palavra3[32] = "";
    char palavra4[32] = "";
    char tabela[32] = "";

    if (texto == NULL || operacao == NULL ||
        entidade == NULL) {
        return 0;
    }

    int quantidade = sscanf(texto, "%31s %31s %31s %31s",
                            palavra1, palavra2,
                            palavra3, palavra4);

    if (quantidade < 2) {
        return 0;
    }

    // Converte as palavras para maiusculas
    char *palavras[] = {
        palavra1, palavra2, palavra3, palavra4
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; palavras[i][j] != '\0'; j++) {
            palavras[i][j] =
                (char) toupper((unsigned char) palavras[i][j]);
        }
    }

    if (strcmp(palavra1, "INSERT") == 0 &&
        strcmp(palavra2, "INTO") == 0 &&
        quantidade >= 3) {

        *operacao = OPERACAO_INSERT;
        strcpy(tabela, palavra3);

    } else if (strcmp(palavra1, "UPDATE") == 0) {

        *operacao = OPERACAO_UPDATE;
        strcpy(tabela, palavra2);

    } else if (strcmp(palavra1, "DELETE") == 0 &&
               strcmp(palavra2, "FROM") == 0 &&
               quantidade >= 3) {

        *operacao = OPERACAO_DELETE;
        strcpy(tabela, palavra3);

    } else if (strcmp(palavra1, "SELECT") == 0 &&
               strcmp(palavra3, "FROM") == 0 &&
               quantidade >= 4) {

        *operacao = OPERACAO_SELECT;
        strcpy(tabela, palavra4);

    } else {
        return 0;
    }

    // Remove o ponto e virgula, se existir
    char *pontoVirgula = strchr(tabela, ';');

    if (pontoVirgula != NULL) {
        *pontoVirgula = '\0';
    }

    if (strcmp(tabela, "PESSOA") == 0) {
        *entidade = COMANDO_PESSOA;

    } else if (strcmp(tabela, "TIPO_PET") == 0) {
        *entidade = COMANDO_TIPO_PET;

    } else if (strcmp(tabela, "PET") == 0) {
        *entidade = COMANDO_PET;

    } else {
        return 0;
    }

    return 1;
}


int adicionarComandoSQL(Fila *fila,
                       const char *texto,
                       int sequencia) {

    TipoOperacao operacao;
    TipoEntidade entidade;

    if (fila == NULL) {
        return 0;
    }

    if (!identificarComando(texto, &operacao, &entidade)) {
        printf("Erro: comando SQL nao reconhecido!\n");
        return 0;
    }

    return enfileirar(fila, texto, sequencia, entidade);
}
