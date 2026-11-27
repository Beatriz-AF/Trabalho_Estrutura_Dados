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
#include "comandos/processador.h"
#include "comandos/script.h"

int main() {

printf("\n--- TESTE UPDATE PARCIAL PESSOA ---\n");

Pessoa *pessoasParcial = NULL;
TipoPet *tiposParcial = NULL;
Pet *petsParcial = NULL;

inserirPessoa(
    &pessoasParcial, 1,
    "Maria", "85999999999",
    "10/05/2000", "Rua A");

const char *testesUpdate[] = {

    // 1: Altera somente telefone.
    "UPDATE pessoa SET fone = '85988888888' "
    "WHERE codigo = 1;",

    // 2: Altera nome e endereco.
    "UPDATE pessoa SET endereco = 'Rua B', "
    "nome = 'Maria Silva' WHERE codigo = 1;",

    // 3: Altera data de nascimento.
    "UPDATE pessoa SET data_nascimento = '15/06/2000' "
    "WHERE codigo = 1;",

    // 4: Invalido, data inexistente.
    "UPDATE pessoa SET data_nascimento = '31/02/2000' "
    "WHERE codigo = 1;",

    // 5: Invalido, nome vazio.
    "UPDATE pessoa SET nome = '' "
    "WHERE codigo = 1;",

    // 6: Invalido, campo repetido.
    "UPDATE pessoa SET fone = '111', "
    "fone = '222' WHERE codigo = 1;",

    // 7: Invalido, pessoa inexistente.
    "UPDATE pessoa SET nome = 'Ana' "
    "WHERE codigo = 99;"
};

for (int i = 0; i < 7; i++) {

    printf("\nTeste %d:\n", i + 1);

    if (executarComandoSQL(
            testesUpdate[i],
            &pessoasParcial,
            &tiposParcial,
            &petsParcial)) {

        printf("UPDATE executado!\n");

    } else {
        printf("UPDATE recusado!\n");
    }
}

printf("\n--- RESULTADO FINAL ---\n");
listarPessoas(pessoasParcial);

liberarPets(&petsParcial);
liberarTiposPet(&tiposParcial);
liberarPessoas(&pessoasParcial);

return  0;
}