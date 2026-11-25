#include <stdio.h>
#include <string.h>
#include "arquivos.h"
#include "../integridade/integridade.h"

typedef struct {
    int codigo;
    char nome[100];
    char fone[20];
    char data_nascimento[11];
    char endereco[150];
} RegistroPessoa;

typedef struct {
    int codigo;
    char descricao[100];
} RegistroTipoPet;

typedef struct {
    int codigo;
    int codigo_pes;
    char nome[100];
    int codigo_tipo;
} RegistroPet;


int salvarPessoas(Pessoa *inicio, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de pessoas!\n");
        return 0;
    }

    Pessoa *aux = inicio;

    while (aux != NULL) {

        RegistroPessoa registro = {0};

        registro.codigo = aux->codigo;

        snprintf(registro.nome, sizeof(registro.nome),
                 "%s", aux->nome);

        snprintf(registro.fone, sizeof(registro.fone),
                 "%s", aux->fone);

        snprintf(registro.data_nascimento,
                 sizeof(registro.data_nascimento),
                 "%s", aux->data_nascimento);

        memcpy(registro.endereco,
                aux->endereco,
                sizeof(registro.endereco));

        if (fwrite(&registro, sizeof(registro), 1, arquivo) != 1) {
            printf("Erro ao salvar pessoa!\n");
            fclose(arquivo);
            return 0;
        }

        aux = aux->prox;
    }

    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar arquivo de pessoas!\n");
        return 0;
    }

    return 1;
}


int salvarTiposPet(TipoPet *inicio, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de tipos!\n");
        return 0;
    }

    TipoPet *aux = inicio;

    while (aux != NULL) {

        RegistroTipoPet registro = {0};

        registro.codigo = aux->codigo;

        snprintf(registro.descricao,
                 sizeof(registro.descricao),
                 "%s", aux->descricao);

        if (fwrite(&registro, sizeof(registro), 1, arquivo) != 1) {
            printf("Erro ao salvar tipo de pet!\n");
            fclose(arquivo);
            return 0;
        }

        aux = aux->prox;
    }

    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar arquivo de tipos!\n");
        return 0;
    }

    return 1;
}


int salvarPets(Pet *inicio, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de pets!\n");
        return 0;
    }

    Pet *aux = inicio;

    while (aux != NULL) {

        RegistroPet registro = {0};

        registro.codigo = aux->codigo;
        registro.codigo_pes = aux->codigo_pes;

        snprintf(registro.nome, sizeof(registro.nome),
                 "%s", aux->nome);

        registro.codigo_tipo = aux->codigo_tipo;

        if (fwrite(&registro, sizeof(registro), 1, arquivo) != 1) {
            printf("Erro ao salvar pet!\n");
            fclose(arquivo);
            return 0;
        }

        aux = aux->prox;
    }

    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar arquivo de pets!\n");
        return 0;
    }

    return 1;
}

int carregarPessoas(Pessoa **inicio, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de pessoas!\n");
        return 0;
    }

    if (*inicio != NULL) {
        printf("Erro: lista de pessoas nao esta vazia!\n");
        fclose(arquivo);
        return 0;
    }

    Pessoa *temporaria = NULL;
    RegistroPessoa registro;

    while (fread(&registro, sizeof(registro), 1, arquivo) == 1) {

        registro.nome[sizeof(registro.nome) - 1] = '\0';
        registro.fone[sizeof(registro.fone) - 1] = '\0';
        registro.data_nascimento[
            sizeof(registro.data_nascimento) - 1] = '\0';
        registro.endereco[
            sizeof(registro.endereco) - 1] = '\0';

        if (!inserirPessoa(&temporaria, registro.codigo,
                           registro.nome, registro.fone,
                           registro.data_nascimento,
                           registro.endereco)) {

            liberarPessoas(&temporaria);
            fclose(arquivo);
            return 0;
        }
    }

    if (ferror(arquivo)) {
        printf("Erro durante leitura de pessoas!\n");
        liberarPessoas(&temporaria);
        fclose(arquivo);
        return 0;
    }

    if (fclose(arquivo) != 0) {
        liberarPessoas(&temporaria);
        return 0;
    }

    *inicio = temporaria;
    return 1;
}


int carregarTiposPet(TipoPet **inicio, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de tipos!\n");
        return 0;
    }

    if (*inicio != NULL) {
        printf("Erro: lista de tipos nao esta vazia!\n");
        fclose(arquivo);
        return 0;
    }

    TipoPet *temporaria = NULL;
    RegistroTipoPet registro;

    while (fread(&registro, sizeof(registro), 1, arquivo) == 1) {

        registro.descricao[
            sizeof(registro.descricao) - 1] = '\0';

        if (!inserirTipoPet(&temporaria, registro.codigo,
                            registro.descricao)) {

            liberarTiposPet(&temporaria);
            fclose(arquivo);
            return 0;
        }
    }

    if (ferror(arquivo)) {
        printf("Erro durante leitura de tipos!\n");
        liberarTiposPet(&temporaria);
        fclose(arquivo);
        return 0;
    }

    if (fclose(arquivo) != 0) {
        liberarTiposPet(&temporaria);
        return 0;
    }

    *inicio = temporaria;
    return 1;
}


int carregarPets(Pet **inicio,
                 Pessoa *listaPessoas,
                 TipoPet *listaTipos,
                 const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de pets!\n");
        return 0;
    }

    if (*inicio != NULL) {
        printf("Erro: lista de pets nao esta vazia!\n");
        fclose(arquivo);
        return 0;
    }

    Pet *temporaria = NULL;
    RegistroPet registro;

    while (fread(&registro, sizeof(registro), 1, arquivo) == 1) {

        registro.nome[sizeof(registro.nome) - 1] = '\0';

        if (!cadastrarPetValidado(&temporaria,
                                 listaPessoas,
                                 listaTipos,
                                 registro.codigo,
                                 registro.codigo_pes,
                                 registro.nome,
                                 registro.codigo_tipo)) {

            liberarPets(&temporaria);
            fclose(arquivo);
            return 0;
        }
    }

    if (ferror(arquivo)) {
        printf("Erro durante leitura de pets!\n");
        liberarPets(&temporaria);
        fclose(arquivo);
        return 0;
    }

    if (fclose(arquivo) != 0) {
        liberarPets(&temporaria);
        return 0;
    }

    *inicio = temporaria;
    return 1;
}

