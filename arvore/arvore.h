
#ifndef ARVORE_H
#define ARVORE_H
#include "../pessoa/pessoa.h"
#include "../tipo_pet/tipo_pet.h"
#include "../pet/pet.h"

typedef struct no_arvore {
    char nome[100];
    int codigo;

    struct no_arvore *esquerda;
    struct no_arvore *direita;
} NoArvore;

int inserirArvore(NoArvore **raiz,
                 const char *nome,
                 int codigo);

NoArvore *buscarArvore(NoArvore *raiz,
                      const char *nome);

void percorrerEmOrdem(NoArvore *raiz);

void liberarArvore(NoArvore *raiz);

int construirArvorePessoas(NoArvore **raiz, Pessoa *listaPessoas);

int construirArvoreTiposPet(NoArvore **raiz,
                           TipoPet *listaTipos);

int construirArvorePets(NoArvore **raiz,
                       Pet *listaPets);


#endif
