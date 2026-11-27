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

printf("\n--- TESTE INSERT PESSOA COM CAMPOS ---\n");

Pessoa *pessoasCamposTeste = NULL;
TipoPet *tiposCamposTeste = NULL;
Pet *petsCamposTeste = NULL;

const char *testes[] = {

    // Valido: campos obrigatorios
    "INSERT INTO pessoa(codigo, nome, data_nascimento) "
    "VALUES(1, 'Maria', '10/05/2000');",

    // Valido: campos em outra ordem
    "INSERT INTO pessoa(nome, endereco, codigo, "
    "data_nascimento, fone) "
    "VALUES('Joao', 'Rua B', 2, '15/08/2001', "
    "'85988888888');",

    // Invalido: falta data de nascimento
    "INSERT INTO pessoa(codigo, nome, fone) "
    "VALUES(3, 'Ana', '85977777777');",

    // Invalido: data inexistente
    "INSERT INTO pessoa(codigo, nome, data_nascimento) "
    "VALUES(4, 'Pedro', '31/02/2000');",

    // Invalido: coluna repetida
    "INSERT INTO pessoa(codigo, nome, nome, "
    "data_nascimento) "
    "VALUES(5, 'Lia', 'Ana', '10/10/2000');",

    // Invalido: codigo repetido
    "INSERT INTO pessoa(codigo, nome, data_nascimento) "
    "VALUES(1, 'Julia', '12/12/2001');"
};

for (int i = 0; i < 6; i++) {

    printf("\nTeste %d:\n", i + 1);

    if (executarComandoSQL(
            testes[i],
            &pessoasCamposTeste,
            &tiposCamposTeste,
            &petsCamposTeste)) {

        printf("INSERT executado!\n");
    } else {
        printf("INSERT recusado!\n");
    }
}

printf("\n--- PESSOAS CADASTRADAS ---\n");
listarPessoas(pessoasCamposTeste);

liberarPets(&petsCamposTeste);
liberarTiposPet(&tiposCamposTeste);
liberarPessoas(&pessoasCamposTeste);

return  0;
}