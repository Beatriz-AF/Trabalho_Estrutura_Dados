#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"
#include "integridade.h"
#include "arquivos.h"

int main() {

    
Pessoa *pessoasArquivo = NULL;
TipoPet *tiposArquivo = NULL;
Pet *petsArquivo = NULL;

inserirPessoa(&pessoasArquivo, 1, "Joao",
              "85999999999", "10/05/2000", "Rua A");

inserirTipoPet(&tiposArquivo, 1, "Cachorro");

cadastrarPetValidado(&petsArquivo,
                     pessoasArquivo,
                     tiposArquivo,
                     10, 1, "Bob", 1);

printf("\n--- TESTE DE SALVAMENTO ---\n");

if (salvarPessoas(pessoasArquivo, "pessoas.dat")) {
    printf("Pessoas salvas com sucesso!\n");
}

if (salvarTiposPet(tiposArquivo, "tipos_pet.dat")) {
    printf("Tipos de pet salvos com sucesso!\n");
}

if (salvarPets(petsArquivo, "pets.dat")) {
    printf("Pets salvos com sucesso!\n");
}

liberarPets(&petsArquivo);
liberarTiposPet(&tiposArquivo);
liberarPessoas(&pessoasArquivo);


printf("\n--- TESTE DE CARREGAMENTO ---\n");

Pessoa *pessoasCarregadas = NULL;
TipoPet *tiposCarregados = NULL;
Pet *petsCarregados = NULL;

int pessoasOk = carregarPessoas(
    &pessoasCarregadas, "pessoas.dat");

int tiposOk = carregarTiposPet(
    &tiposCarregados, "tipos_pet.dat");

if (pessoasOk && tiposOk) {
    carregarPets(&petsCarregados,
                pessoasCarregadas,
                tiposCarregados,
                "pets.dat");
}

printf("\n--- PESSOAS RECUPERADAS ---\n");
listarPessoas(pessoasCarregadas);

printf("\n--- TIPOS RECUPERADOS ---\n");
listarTiposPet(tiposCarregados);

printf("\n--- PETS RECUPERADOS ---\n");
listarPets(petsCarregados);

liberarPets(&petsCarregados);
liberarTiposPet(&tiposCarregados);
liberarPessoas(&pessoasCarregadas);

return 0;
}