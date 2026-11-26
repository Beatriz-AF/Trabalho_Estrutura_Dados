#include <stdio.h>
#include <stdlib.h>

#include "pessoa/pessoa.h"
#include "tipo_pet/tipo_pet.h"
#include "pet/pet.h"
#include "integridade/integridade.h"
#include "arquivos/arquivos.h"
#include "arvore/arvore.h"
#include "comandos/fila.h"

int main() {


printf("\n--- TESTE DE DISTRIBUICAO ---\n");

Fila geral, pessoas, tipos, pets;

inicializarFila(&geral);
inicializarFila(&pessoas);
inicializarFila(&tipos);
inicializarFila(&pets);

enfileirar(&geral,
           "INSERT INTO pessoa ...",
           1, COMANDO_PESSOA);

enfileirar(&geral,
           "INSERT INTO pet ...",
           2, COMANDO_PET);

enfileirar(&geral,
           "SELECT * FROM pessoa;",
           3, COMANDO_PESSOA);

enfileirar(&geral,
           "DELETE FROM tipo_pet WHERE codigo = 2;",
           4, COMANDO_TIPO_PET);

if (distribuirComandos(&geral, &pessoas,
                       &tipos, &pets)) {

    printf("\n--- FILA PESSOAS ---\n");
    listarFila(&pessoas);

    printf("\n--- FILA TIPOS ---\n");
    listarFila(&tipos);

    printf("\n--- FILA PETS ---\n");
    listarFila(&pets);

    printf("\n--- ORDEM FINAL ---\n");

    NoComando atual;

    while (retirarProximoComando(&pessoas, &tipos,
                                &pets, &atual)) {

        printf("Sequencia %d: %s\n",
               atual.sequencia, atual.texto);
    }
}

liberarFila(&geral);
liberarFila(&pessoas);
liberarFila(&tipos);
liberarFila(&pets);

return 0;
}