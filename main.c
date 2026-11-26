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

int main() {

printf("\n--- TESTE PROCESSADOR DELETE ---\n");

Pessoa *pessoasDelete = NULL;
TipoPet *tiposDelete = NULL;
Pet *petsDelete = NULL;

inserirPessoa(&pessoasDelete, 1, "Maria",
              "85999999999", "10/05/2000", "Rua A");

inserirTipoPet(&tiposDelete, 1, "Cachorro");

cadastrarPetValidado(&petsDelete,
                     pessoasDelete,
                     tiposDelete,
                     10, 1, "Bob", 1);

printf("\n--- TENTANDO EXCLUIR MARIA ---\n");

if (!executarDeleteSQL(
        "DELETE FROM pessoa WHERE codigo = 1;",
        &pessoasDelete, &tiposDelete, &petsDelete)) {

    printf("Exclusao recusada!\n");
}

printf("\n--- EXCLUINDO BOB ---\n");

if (executarDeleteSQL(
        "DELETE FROM pet WHERE codigo = 10;",
        &pessoasDelete, &tiposDelete, &petsDelete)) {

    printf("Pet removido com sucesso!\n");
}

printf("\n--- EXCLUINDO MARIA ---\n");

if (executarDeleteSQL(
        "DELETE FROM pessoa WHERE codigo = 1;",
        &pessoasDelete, &tiposDelete, &petsDelete)) {

    printf("Pessoa removida com sucesso!\n");
}

printf("\n--- EXCLUINDO TIPO CACHORRO ---\n");

if (executarDeleteSQL(
        "DELETE FROM tipo_pet WHERE codigo = 1;",
        &pessoasDelete, &tiposDelete, &petsDelete)) {

    printf("Tipo removido com sucesso!\n");
}

printf("\n--- LISTAS FINAIS ---\n");

listarPessoas(pessoasDelete);
listarTiposPet(tiposDelete);
listarPets(petsDelete);

liberarPets(&petsDelete);
liberarTiposPet(&tiposDelete);
liberarPessoas(&pessoasDelete);

return 0;
}