#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

int main() {

    Pessoa *listaPessoas = NULL;

    inserirPessoa(&listaPessoas, 1, "Joao",
                  "859995566", "04/10/2002", "Rua A");

    inserirPessoa(&listaPessoas, 2, "Maria",
                  "859996677", "10/05/2001", "Rua B");

    inserirPessoa(&listaPessoas, 3, "Jose",
                  "859997788", "15/08/2000", "Rua C");

    printf("\n--- LISTA PESSOAS ---\n");
    listarPessoas(listaPessoas);

    liberarPessoas(&listaPessoas);

    TipoPet *listaTiposPet = NULL;

    inserirTipoPet(&listaTiposPet, 1, "Cachorro");
    inserirTipoPet(&listaTiposPet, 2, "Gato");
    inserirTipoPet(&listaTiposPet, 3, "Coelho");

    printf("\n--- TIPOS DE PET CADASTRADOS ---\n");
    listarTiposPet(listaTiposPet);

    liberarTiposPet(&listaTiposPet);

    Pet *listaPets = NULL;

    inserirPet(&listaPets, 1, 10, "Bob", 1);
    inserirPet(&listaPets, 2, 10, "Luna", 2);
    inserirPet(&listaPets, 3, 20, "Mel", 1);

    printf("\n--- PETS CADASTRADOS ---\n");
    listarPets(listaPets);

    atualizarPet(listaPets, 1, -1, "Bob Junior", -1);

    printf("\n--- PETS APOS ATUALIZACAO ---\n");
    listarPets(listaPets);

    removerPet(&listaPets, 2);

    printf("\n--- PETS APOS REMOCAO ---\n");
    listarPets(listaPets);

    liberarPets(&listaPets);
    
    return 0;
}