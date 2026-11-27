
#include <stdio.h>
#include "processador.h"
#include "leitor.h"
#include "validador.h"
#include "../integridade/integridade.h"
#include "../arvore/arvore.h"

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
                                    nome, fone, data, endereco) &&
                !validarInsertPessoaComCampos(
                    texto, &codigo, nome, fone, data, endereco)) {

                printf("Erro: INSERT pessoa invalido!\n");
                return 0;
            }

            return inserirPessoa(pessoas, codigo,
                                nome, fone,
                                data, endereco);
        }

        case COMANDO_TIPO_PET: {
            char descricao[100];

            if (!validarInsertTipoPet(texto, &codigo, descricao) &&
                !validarInsertTipoPetComCampos(texto, &codigo,
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


int executarUpdatePetSQL(const char *texto,
                         Pet *pets,
                         Pessoa *pessoas,
                         TipoPet *tipos) {

    if (texto == NULL) {
        return 0;
    }

    int codigo;
    int codigoPes;
    int codigoTipo;
    char nome[100];

    if (!validarUpdatePet(texto,
                          &codigo,
                          &codigoPes,
                          nome,
                          &codigoTipo)) {

        printf("Erro: comando UPDATE pet invalido!\n");
        return 0;
    }

    return atualizarPetValidado(
        pets, pessoas, tipos,
        codigo, codigoPes, nome, codigoTipo);
}


int executarComandoSQL(const char *texto,
                      Pessoa **pessoas,
                      TipoPet **tipos,
                      Pet **pets) {

    if (texto == NULL || pessoas == NULL ||
        tipos == NULL || pets == NULL) {
        return 0;
    }

    TipoOperacao operacao;
    TipoEntidade entidade;

    if (!identificarComando(texto, &operacao, &entidade)) {
        printf("Erro: comando SQL nao reconhecido!\n");
        return 0;
    }

    switch (operacao) {

        case OPERACAO_INSERT:
            return executarInsertSQL(
                texto, pessoas, tipos, pets);

        case OPERACAO_DELETE:
            return executarDeleteSQL(
                texto, pessoas, tipos, pets);

        case OPERACAO_UPDATE:

            switch (entidade) {

                case COMANDO_PESSOA:
                    return executarUpdatePessoaSQL(
                        texto, *pessoas);

                case COMANDO_TIPO_PET:
                    return executarUpdateTipoPetSQL(
                        texto, tipos);

                case COMANDO_PET:
                    return executarUpdatePetSQL(
                        texto, *pets,
                        *pessoas, *tipos);

                default:
                    return 0;
            }

        case OPERACAO_SELECT:
            return executarSelectSQL(
                texto, *pessoas, *tipos, *pets);

        default:
            printf("Operacao desconhecida!\n");
            return 0;
    }
}


static void imprimirArvoreOrdenada(
    NoArvore *raiz,
    TipoEntidade entidade,
    Pessoa *pessoas,
    TipoPet *tipos,
    Pet *pets) {

    if (raiz == NULL) {
        return;
    }

    imprimirArvoreOrdenada(
        raiz->esquerda, entidade,
        pessoas, tipos, pets);

    switch (entidade) {

        case COMANDO_PESSOA: {
            Pessoa *p = buscarPessoa(
                pessoas, raiz->codigo);

            if (p != NULL) {
                printf("\nCodigo: %d\n", p->codigo);
                printf("Nome: %s\n", p->nome);
                printf("Telefone: %s\n", p->fone);
                printf("Nascimento: %s\n",
                       p->data_nascimento);
                printf("Endereco: %s\n", p->endereco);
            }
            break;
        }

        case COMANDO_TIPO_PET: {
            TipoPet *t = buscarTipoPet(
                tipos, raiz->codigo);

            if (t != NULL) {
                printf("\nCodigo: %d\n", t->codigo);
                printf("Descricao: %s\n",
                       t->descricao);
            }
            break;
        }

        case COMANDO_PET: {
            Pet *pet = buscarPet(
                pets, raiz->codigo);

            if (pet != NULL) {
                printf("\nCodigo: %d\n", pet->codigo);
                printf("Codigo do dono: %d\n",
                       pet->codigo_pes);
                printf("Nome: %s\n", pet->nome);
                printf("Codigo do tipo: %d\n",
                       pet->codigo_tipo);
            }
            break;
        }
    }

    imprimirArvoreOrdenada(
        raiz->direita, entidade,
        pessoas, tipos, pets);
}


static int executarSelectOrdenadoSQL(
    TipoEntidade entidade,
    Pessoa *pessoas,
    TipoPet *tipos,
    Pet *pets) {

    NoArvore *raiz = NULL;

    switch (entidade) {

        case COMANDO_PESSOA: {
            Pessoa *aux = pessoas;

            while (aux != NULL) {
                inserirArvore(&raiz,
                             aux->nome,
                             aux->codigo);
                aux = aux->prox;
            }
            break;
        }

        case COMANDO_TIPO_PET: {
            TipoPet *aux = tipos;

            while (aux != NULL) {
                inserirArvore(&raiz,
                             aux->descricao,
                             aux->codigo);
                aux = aux->prox;
            }
            break;
        }

        case COMANDO_PET: {
            Pet *aux = pets;

            while (aux != NULL) {
                inserirArvore(&raiz,
                             aux->nome,
                             aux->codigo);
                aux = aux->prox;
            }
            break;
        }

        default:
            return 0;
    }

    if (raiz == NULL) {
        printf("Nenhum registro cadastrado!\n");
        return 1;
    }

    imprimirArvoreOrdenada(
        raiz, entidade, pessoas, tipos, pets);

    liberarArvore(raiz);

    return 1;
}

int executarSelectSQL(const char *texto,
                     Pessoa *pessoas,
                     TipoPet *tipos,
                     Pet *pets) {
    
    TipoEntidade entidadeOrdenada;

    if (validarSelectOrderBySQL(
            texto, &entidadeOrdenada)) {

        return executarSelectOrdenadoSQL(
            entidadeOrdenada,
            pessoas, tipos, pets);
    }


    TipoEntidade entidade;
    int temWhere;
    int codigo;

    if (!validarSelectSQL(texto, &entidade,
                          &temWhere, &codigo)) {

        printf("Erro: comando SELECT invalido!\n");
        return 0;
    }

    switch (entidade) {

        case COMANDO_PESSOA:

            if (!temWhere) {
                listarPessoas(pessoas);
            } else {
                Pessoa *p = buscarPessoa(pessoas, codigo);

                if (p == NULL) {
                    printf("Pessoa nao encontrada!\n");
                    return 0;
                }

                printf("\nCodigo: %d\n", p->codigo);
                printf("Nome: %s\n", p->nome);
                printf("Telefone: %s\n", p->fone);
                printf("Nascimento: %s\n",
                       p->data_nascimento);
                printf("Endereco: %s\n", p->endereco);
            }

            return 1;

        case COMANDO_TIPO_PET:

            if (!temWhere) {
                listarTiposPet(tipos);
            } else {
                TipoPet *t = buscarTipoPet(tipos, codigo);

                if (t == NULL) {
                    printf("Tipo nao encontrado!\n");
                    return 0;
                }

                printf("\nCodigo: %d\n", t->codigo);
                printf("Descricao: %s\n", t->descricao);
            }

            return 1;

        case COMANDO_PET:

            if (!temWhere) {
                listarPets(pets);
            } else {
                Pet *pet = buscarPet(pets, codigo);

                if (pet == NULL) {
                    printf("Pet nao encontrado!\n");
                    return 0;
                }

                printf("\nCodigo: %d\n", pet->codigo);
                printf("Codigo do dono: %d\n",
                       pet->codigo_pes);
                printf("Nome: %s\n", pet->nome);
                printf("Codigo do tipo: %d\n",
                       pet->codigo_tipo);
            }

            return 1;

        default:
            return 0;
    }
}
