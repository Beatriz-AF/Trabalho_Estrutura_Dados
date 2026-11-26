#include <stdio.h>
#include <stdlib.h>

#include "pessoa/pessoa.h"
#include "tipo_pet/tipo_pet.h"
#include "pet/pet.h"
#include "integridade/integridade.h"
#include "arquivos/arquivos.h"
#include "arvore/arvore.h"
#include "comandos/fila.h"
#include "comandos/leitor.h"
#include "comandos/validador.h"

int main() {

printf("\n--- TESTE DO VALIDADOR SQL ---\n");

const char *comandosTeste[] = {
    "INSERT INTO tipo_pet VALUES (1, 'Cachorro');",
    "INSERT INTO tipo_pet VALUES (2, 'Gato');",
    "INSERT INTO tipo_pet VALUES (0, 'Coelho');",
    "INSERT INTO tipo_pet VALUES (3, 'Papagaio'",
    "INSERT INTO tipo_pet VALUES (abc, 'Peixe');"
};

for (int i = 0; i < 5; i++) {

    int codigo;
    char descricao[100];

    printf("\nComando: %s\n", comandosTeste[i]);

    if (validarInsertTipoPet(comandosTeste[i],
                            &codigo, descricao)) {

        printf("VALIDO | Codigo: %d | Descricao: %s\n",
               codigo, descricao);

    } else {
        printf("Comando invalido!\n");
    }
}

return 0;
}