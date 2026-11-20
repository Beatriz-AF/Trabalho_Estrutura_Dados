#ifndef PET_H
#define PET_H

/* Estrutura e funcoes da lista de pets (mesmo padrao de pessoa e tipo_pet).
   As regras que dependem de outras listas (dono e tipo existentes) ficam no processador. */

typedef struct pet {
    int codigo;
    int codigo_pes;          /* codigo da pessoa dona */
    char nome[100];
    int codigo_tipo;         /* codigo do tipo de pet */

    struct pet *ant;
    struct pet *prox;
} Pet;

Pet *buscarPet(Pet *inicio, int codigo);

int inserirPet(Pet **inicio, int codigo, int codigoPes,
               const char *nome, int codigoTipo);

/* codigoPes e codigoTipo < 0 e nome NULL significam "nao alterar" */
int atualizarPet(Pet *inicio, int codigo, int codigoPes,
                 const char *nome, int codigoTipo);

int removerPet(Pet **inicio, int codigo);

void liberarPets(Pet **inicio);

#endif
