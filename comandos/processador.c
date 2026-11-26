
#include <stdio.h>
#include "processador.h"
#include "leitor.h"
#include "validador.h"
#include "../integridade/integridade.h"

int executarInsertSQL(const char *texto,
                     Pessoa **pessoas,
                     TipoPet **tipos,
                     Pet **pets) {

    TipoOperacao operacao;
    TipoEntidade entidade;

    if (texto == NULL || pessoas == NULL ||
        tipos == NULL || pets == NULL) {
        return 0;
    }

    if (!identificarComando(texto, &operacao, &entidade)) {
        printf("Erro: comando nao reconhecido!\n");
        return 0;
    }

    if (operacao != OPERACAO_INSERT) {
        printf("Erro: operacao ainda nao implementada!\n");
        return 0;
    }

    int codigo;
    char nome[100];

    switch (entidade) {

        case COMANDO_PESSOA: {
            char fone[20];
            char data[11];
            char endereco[150];

            if (!validarInsertPessoa(texto, &codigo,
                                    nome, fone,
                                    data, endereco)) {
                printf("Erro: INSERT pessoa invalido!\n");
                return 0;
            }

            return inserirPessoa(pessoas, codigo,
                                nome, fone,
                                data, endereco);
        }

        case COMANDO_TIPO_PET: {
            char descricao[100];

            if (!validarInsertTipoPet(texto, &codigo,
                                     descricao)) {
                printf("Erro: INSERT tipo_pet invalido!\n");
                return 0;
            }

            return inserirTipoPet(tipos, codigo, descricao);
        }

        case COMANDO_PET: {
            int codigoPes;
            int codigoTipo;

            if (!validarInsertPet(texto, &codigo,
                                 &codigoPes, nome,
                                 &codigoTipo)) {
                printf("Erro: INSERT pet invalido!\n");
                return 0;
            }

            return cadastrarPetValidado(pets, *pessoas,
                                       *tipos, codigo,
                                       codigoPes, nome,
                                       codigoTipo);
        }

        default:
            printf("Erro: entidade desconhecida!\n");
            return 0;
    }
}


int executarDeleteSQL(const char *texto,
                     Pessoa **pessoas,
                     TipoPet **tipos,
                     Pet **pets) {

    if (texto == NULL || pessoas == NULL ||
        tipos == NULL || pets == NULL) {
        return 0;
    }

    TipoEntidade entidade;
    int codigo;

    if (!validarDeleteSQL(texto, &entidade, &codigo)) {
        printf("Erro: comando DELETE invalido!\n");
        return 0;
    }

    switch (entidade) {

        case COMANDO_PESSOA:
            return removerPessoaSeguro(
                pessoas, *pets, codigo);

        case COMANDO_TIPO_PET:
            return removerTipoPetSeguro(
                tipos, *pets, codigo);

        case COMANDO_PET:
            return removerPet(pets, codigo);

        default:
            printf("Erro: entidade desconhecida!\n");
            return 0;
    }
}


int executarUpdateTipoPetSQL(const char *texto,
                             TipoPet **tipos) {

    if (texto == NULL || tipos == NULL) {
        return 0;
    }

    int codigo;
    char descricao[100];

    if (!validarUpdateTipoPet(texto,
                             &codigo,
                             descricao)) {

        printf("Erro: comando UPDATE invalido!\n");
        return 0;
    }

    return atualizarTipoPet(*tipos, codigo, descricao);
}
int executarUpdatePessoaSQL(const char *texto,
                           Pessoa *pessoas) {

    if (texto == NULL) {
        return 0;
    }

    int codigo;
    char nome[100];
    char fone[20];
    char data[11];
    char endereco[150];

    if (!validarUpdatePessoa(texto, &codigo,
                             nome, fone,
                             data, endereco)) {
        printf("Erro: comando UPDATE pessoa invalido!\n");
        return 0;
    }

    return atualizarPessoa(pessoas, codigo,
                           nome, fone,
                           data, endereco);
}