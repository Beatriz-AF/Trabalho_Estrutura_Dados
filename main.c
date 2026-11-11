
#include <stdio.h>
#include "pessoa.h"

int main() {
    Pessoa *listaPessoas = NULL;

    inserirPessoa(&listaPessoas, 1, "Joao",
                  "859995566", "04/10/2002",
                  "Rua A");

    inserirPessoa(&listaPessoas, 2, "Maria",
                  "859996677", "10/05/2001",
                  "Rua B");

    inserirPessoa(&listaPessoas, 1, "Jose",
                  "859997788", "15/08/2000",
                  "Rua C");

    printf("\n--- LISTA DE PESSOAS ---\n");
    listarPessoas(listaPessoas);

    liberarPessoas(&listaPessoas);

    return 0;
}

