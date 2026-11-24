
#ifndef PET_H
#define PET_H

typedef struct pet {
    int codigo;
    int codigo_pes;
    char nome[100];
    int codigo_tipo;

    struct pet *ant;
    struct pet *prox;
} Pet;

Pet *buscarPet(Pet *inicio, int codigo);

int inserirPet(Pet **inicio, int codigo, int codigoPes,
               const char *nome, int codigoTipo);

int atualizarPet(Pet *inicio, int codigo, int codigoPes,
                 const char *nome, int codigoTipo);

int removerPet(Pet **inicio, int codigo);

void listarPets(Pet *inicio);

void liberarPets(Pet **inicio);

#endif
