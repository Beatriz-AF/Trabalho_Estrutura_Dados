
#include <stdio.h>
#include "integridade.h"

int cadastrarPetValidado(Pet **listaPets,
                        Pessoa *listaPessoas,
                        TipoPet *listaTipos,
                        int codigo,
                        int codigoPes,
                        const char *nome,
                        int codigoTipo) {

    if (buscarPessoa(listaPessoas, codigoPes) == NULL) {
        printf("Erro: dono nao cadastrado!\n");
        return 0;
    }

    if (buscarTipoPet(listaTipos, codigoTipo) == NULL) {
        printf("Erro: tipo de pet nao cadastrado!\n");
        return 0;
    }

    return inserirPet(listaPets, codigo,
                      codigoPes, nome, codigoTipo);
}


int atualizarPetValidado(Pet *listaPets,
                        Pessoa *listaPessoas,
                        TipoPet *listaTipos,
                        int codigo,
                        int codigoPes,
                        const char *nome,
                        int codigoTipo) {

    if (buscarPet(listaPets, codigo) == NULL) {
        printf("Erro: pet nao encontrado!\n");
        return 0;
    }

    if (codigoPes == 0 || codigoTipo == 0) {
        printf("Erro: codigo invalido!\n");
        return 0;
    }

    if (codigoPes > 0 &&
        buscarPessoa(listaPessoas, codigoPes) == NULL) {

        printf("Erro: novo dono nao cadastrado!\n");
        return 0;
    }

    if (codigoTipo > 0 &&
        buscarTipoPet(listaTipos, codigoTipo) == NULL) {

        printf("Erro: novo tipo nao cadastrado!\n");
        return 0;
    }

    return atualizarPet(listaPets, codigo,
                        codigoPes, nome, codigoTipo);
}


int removerPessoaSeguro(Pessoa **listaPessoas,
                       Pet *listaPets,
                       int codigo) {

    if (buscarPessoa(*listaPessoas, codigo) == NULL) {
        printf("Erro: pessoa nao encontrada!\n");
        return 0;
    }

    Pet *aux = listaPets;

    while (aux != NULL) {

        if (aux->codigo_pes == codigo) {
            printf("Erro: pessoa possui pets cadastrados!\n");
            return 0;
        }

        aux = aux->prox;
    }

    return removerPessoa(listaPessoas, codigo);
}


int removerTipoPetSeguro(TipoPet **listaTipos,
                        Pet *listaPets,
                        int codigo) {

    if (buscarTipoPet(*listaTipos, codigo) == NULL) {
        printf("Erro: tipo de pet nao encontrado!\n");
        return 0;
    }

    Pet *aux = listaPets;

    while (aux != NULL) {

        if (aux->codigo_tipo == codigo) {
            printf("Erro: tipo possui pets cadastrados!\n");
            return 0;
        }

        aux = aux->prox;
    }

    return removerTipoPet(listaTipos, codigo);
}
