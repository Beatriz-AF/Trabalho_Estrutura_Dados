#include <stdio.h>
#include <stdlib.h>

#include "pessoa/pessoa.h"
#include "tipo_pet/tipo_pet.h"
#include "pet/pet.h"
#include "integridade/integridade.h"
#include "arquivos/arquivos.h"
#include "arvore/arvore.h"

int main() {


printf("\n--- TESTE ARVORE DE TIPOS DE PET ---\n");

TipoPet *tiposArvore = NULL;
NoArvore *arvoreTipos = NULL;

inserirTipoPet(&tiposArvore, 1, "Cachorro");
inserirTipoPet(&tiposArvore, 2, "Gato");
inserirTipoPet(&tiposArvore, 3, "Coelho");

if (construirArvoreTiposPet(&arvoreTipos, tiposArvore)) {
    percorrerEmOrdem(arvoreTipos);
}

liberarArvore(arvoreTipos);
liberarTiposPet(&tiposArvore);


printf("\n--- TESTE ARVORE DE PETS ---\n");

Pet *petsArvore = NULL;
NoArvore *arvorePets = NULL;

inserirPet(&petsArvore, 1, 10, "Bob", 1);
inserirPet(&petsArvore, 2, 10, "Luna", 2);
inserirPet(&petsArvore, 3, 20, "Mel", 1);
inserirPet(&petsArvore, 4, 20, "Amora", 2);

if (construirArvorePets(&arvorePets, petsArvore)) {
    percorrerEmOrdem(arvorePets);
}

liberarArvore(arvorePets);
liberarPets(&petsArvore);

return 0;
}