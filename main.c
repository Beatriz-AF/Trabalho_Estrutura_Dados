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

printf("\n--- TESTE UPDATE PESSOA ---\n");

Pessoa *pessoasUpdate = NULL;

inserirPessoa(&pessoasUpdate, 1,
              "Maria",
              "85999999999",
              "10/05/2000",
              "Rua A");

printf("\n--- ANTES DO UPDATE ---\n");
listarPessoas(pessoasUpdate);

printf("\n--- EXECUTANDO UPDATE ---\n");

if (executarUpdatePessoaSQL(
        "UPDATE pessoa SET nome = 'Maria Silva', "
        "fone = '85988888888', "
        "data_nascimento = '15/06/2000', "
        "endereco = 'Rua B' WHERE codigo = 1;",
        pessoasUpdate)) {

    printf("UPDATE executado com sucesso!\n");

} else {
    printf("Falha no UPDATE!\n");
}

printf("\n--- DEPOIS DO UPDATE ---\n");
listarPessoas(pessoasUpdate);

printf("\n--- TESTE CODIGO INEXISTENTE ---\n");

if (!executarUpdatePessoaSQL(
        "UPDATE pessoa SET nome = 'Ana', "
        "fone = '85977777777', "
        "data_nascimento = '20/03/2001', "
        "endereco = 'Rua C' WHERE codigo = 99;",
        pessoasUpdate)) {

    printf("Atualizacao recusada!\n");
}

liberarPessoas(&pessoasUpdate);

return 0;
}