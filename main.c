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

printf("\n--- TESTE SELECT ORDER BY ---\n");

Pessoa *pessoasOrd = NULL;
TipoPet *tiposOrd = NULL;
Pet *petsOrd = NULL;

// Pessoas fora de ordem alfabetica
inserirPessoa(&pessoasOrd, 1, "Maria",
              "85999999999", "10/05/2000", "Rua A");

inserirPessoa(&pessoasOrd, 2, "Ana",
              "85988888888", "15/08/2001", "Rua B");

inserirPessoa(&pessoasOrd, 3, "Pedro",
              "85977777777", "20/03/2002", "Rua C");

// Tipos fora de ordem alfabetica
inserirTipoPet(&tiposOrd, 1, "Gato");
inserirTipoPet(&tiposOrd, 2, "Cachorro");
inserirTipoPet(&tiposOrd, 3, "Coelho");

// Pets fora de ordem alfabetica
cadastrarPetValidado(&petsOrd,
                     pessoasOrd, tiposOrd,
                     10, 1, "Luna", 1);

cadastrarPetValidado(&petsOrd,
                     pessoasOrd, tiposOrd,
                     11, 2, "Amora", 2);

cadastrarPetValidado(&petsOrd,
                     pessoasOrd, tiposOrd,
                     12, 3, "Bob", 3);

printf("\n--- PESSOAS ORDENADAS ---\n");

executarComandoSQL(
    "SELECT * FROM pessoa ORDER BY nome;",
    &pessoasOrd, &tiposOrd, &petsOrd);

printf("\n--- TIPOS ORDENADOS ---\n");

executarComandoSQL(
    "SELECT * FROM tipo_pet ORDER BY descricao;",
    &pessoasOrd, &tiposOrd, &petsOrd);

printf("\n--- PETS ORDENADOS ---\n");

executarComandoSQL(
    "SELECT * FROM pet ORDER BY nome;",
    &pessoasOrd, &tiposOrd, &petsOrd);

liberarPets(&petsOrd);
liberarTiposPet(&tiposOrd);
liberarPessoas(&pessoasOrd);



printf("\n--- TESTE DE ARQUIVO SQL ---\n");

Pessoa *pessoasScript = NULL;
TipoPet *tiposScript = NULL;
Pet *petsScript = NULL;

Fila geralScript;
Fila filaPessoasScript;
Fila filaTiposScript;
Fila filaPetsScript;

inicializarFila(&geralScript);
inicializarFila(&filaPessoasScript);
inicializarFila(&filaTiposScript);
inicializarFila(&filaPetsScript);

if (carregarScriptSQL("comandos.sql", &geralScript)) {

    if (distribuirComandos(&geralScript,
                           &filaPessoasScript,
                           &filaTiposScript,
                           &filaPetsScript)) {

        NoComando comando;

        while (retirarProximoComando(
                   &filaPessoasScript,
                   &filaTiposScript,
                   &filaPetsScript,
                   &comando)) {

            printf("\n--- Comando %d ---\n",
                   comando.sequencia);

            printf("%s\n", comando.texto);

            if (executarComandoSQL(
                    comando.texto,
                    &pessoasScript,
                    &tiposScript,
                    &petsScript)) {

                printf("Executado com sucesso!\n");

            } else {
                printf("Falha na execucao!\n");
            }
        }
    }
}

printf("\n--- ESTADO FINAL DOS CADASTROS ---\n");

printf("\nPESSOAS:\n");
listarPessoas(pessoasScript);

printf("\nTIPOS:\n");
listarTiposPet(tiposScript);

printf("\nPETS:\n");
listarPets(petsScript);

liberarFila(&geralScript);
liberarFila(&filaPessoasScript);
liberarFila(&filaTiposScript);
liberarFila(&filaPetsScript);

liberarPets(&petsScript);
liberarTiposPet(&tiposScript);
liberarPessoas(&pessoasScript);

return  0;
}