#include <stdio.h>

#include "pessoa/pessoa.h"
#include "tipo_pet/tipo_pet.h"
#include "pet/pet.h"
#include "comandos/processador.h"

int main(void) {

    Pessoa *pessoas = NULL;
    TipoPet *tipos = NULL;
    Pet *pets = NULL;

    printf("--- TESTE UPDATE PARCIAL PET ---\n");

    inserirPessoa(&pessoas, 1, "Maria",
                  "85999999999", "15/06/2000", "Rua A");

    inserirPessoa(&pessoas, 2, "Joao",
                  "85988888888", "20/08/2001", "Rua B");

    inserirTipoPet(&tipos, 1, "Cachorro");
    inserirTipoPet(&tipos, 2, "Gato");

    inserirPet(&pets, 10, 1, "Bob", 1);

    const char *testes[] = {

        "UPDATE pet SET nome = 'Max' WHERE codigo = 10;",

        "UPDATE pet SET codigo_pes = 2 WHERE codigo = 10;",

        "UPDATE pet SET codigo_tipo = 2 WHERE codigo = 10;",

        "UPDATE pet SET nome = 'Luna', codigo_cli = 1 WHERE codigo = 10;",

        "UPDATE pet SET codigo_pes = 999 WHERE codigo = 10;",

        "UPDATE pet SET codigo_tipo = 999 WHERE codigo = 10;",

        "UPDATE pet SET nome = '' WHERE codigo = 10;",

        "UPDATE pet SET nome = 'Mel', nome = 'Bob' WHERE codigo = 10;",

        "UPDATE pet SET nome = 'Mel' WHERE codigo = 999;"
    };

    int quantidade = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < quantidade; i++) {

        printf("\nTeste %d:\n", i + 1);

        if (executarUpdatePetSQL(
                testes[i], pets, pessoas, tipos)) {

            printf("UPDATE executado!\n");

        } else {

            printf("UPDATE recusado!\n");
        }
    }

    printf("\n--- RESULTADO FINAL ---\n");

    listarPets(pets);

    liberarPets(&pets);
    liberarPessoas(&pessoas);
    liberarTiposPet(&tipos);

    return 0;
}