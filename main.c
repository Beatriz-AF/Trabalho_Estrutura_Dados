#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"
#include "integridade.h"

int main() {

    Pessoa *pessoasIntegridade = NULL;
    TipoPet *tiposIntegridade = NULL;
    Pet *petsIntegridade = NULL;

    inserirPessoa(&pessoasIntegridade, 1, "Joao","85999999999", "10/05/2000", "Rua A");

    inserirPessoa(&pessoasIntegridade, 2, "Maria","85988888888", "15/08/2001", "Rua B");

    inserirTipoPet(&tiposIntegridade, 1, "Cachorro");
    inserirTipoPet(&tiposIntegridade, 2, "Gato");

    cadastrarPetValidado(&petsIntegridade, pessoasIntegridade, tiposIntegridade, 10, 1, "Bob", 1);

    printf("\n--- TESTE UPDATE INVALIDO ---\n");

    atualizarPetValidado(petsIntegridade, pessoasIntegridade, tiposIntegridade, 10, 99, NULL, -1);

    printf("\n--- TESTE UPDATE VALIDO ---\n");

    atualizarPetValidado(petsIntegridade, pessoasIntegridade, tiposIntegridade, 10, 2, "Bob Junior", 2);

    listarPets(petsIntegridade);

    printf("\n--- TESTE DELETE PESSOA ---\n");

    removerPessoaSeguro(&pessoasIntegridade, petsIntegridade, 2);

    printf("\n--- TESTE DELETE TIPO ---\n");

    removerTipoPetSeguro(&tiposIntegridade, petsIntegridade, 2);

    printf("\n--- REMOVENDO PET ---\n");

    removerPet(&petsIntegridade, 10);

    printf("\n--- EXCLUSOES APOS REMOVER PET ---\n");

    removerPessoaSeguro(&pessoasIntegridade, petsIntegridade, 2);

    removerTipoPetSeguro(&tiposIntegridade, petsIntegridade, 2);

    liberarPets(&petsIntegridade);
    liberarTiposPet(&tiposIntegridade);
    liberarPessoas(&pessoasIntegridade);

    
    return 0;
}