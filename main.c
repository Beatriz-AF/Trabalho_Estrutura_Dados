#include <stdio.h>
#include "pessoa.h"
#include "tipo_pet.h"

int main() {

    Pessoa *listaPessoas = NULL;

    inserirPessoa(&listaPessoas, 1, "Joao",
                  "859995566", "04/10/2002", "Rua A");

    inserirPessoa(&listaPessoas, 2, "Maria",
                  "859996677", "10/05/2001", "Rua B");

    inserirPessoa(&listaPessoas, 3, "Jose",
                  "859997788", "15/08/2000", "Rua C");

    printf("\n--- LISTA ORIGINAL ---\n");
    listarPessoas(listaPessoas);

    atualizarPessoa(listaPessoas, 1,
                    "Joao Pedro", NULL, NULL, NULL);

    printf("\n--- LISTA APOS ATUALIZACAO ---\n");
    listarPessoas(listaPessoas);

    removerPessoa(&listaPessoas, 2);

    printf("\n--- LISTA APOS REMOCAO ---\n");
    listarPessoas(listaPessoas);

    liberarPessoas(&listaPessoas);

    TipoPet *listaTiposPet = NULL;

    inserirTipoPet(&listaTiposPet, 1, "Cachorro");
    inserirTipoPet(&listaTiposPet, 2, "Gato");
    inserirTipoPet(&listaTiposPet, 3, "Coelho");

    printf("\n--- TIPOS DE PET CADASTRADOS ---\n");
    listarTiposPet(listaTiposPet);

    atualizarTipoPet(listaTiposPet, 1, "Cao");

    printf("\n--- TIPOS APOS ATUALIZACAO ---\n");
    listarTiposPet(listaTiposPet);

    removerTipoPet(&listaTiposPet, 2);

    printf("\n--- TIPOS APOS REMOCAO ---\n");
    listarTiposPet(listaTiposPet);

    liberarTiposPet(&listaTiposPet);
    
    return 0;
}