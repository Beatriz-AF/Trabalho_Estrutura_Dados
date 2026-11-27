#include <stdio.h>
#include <stdlib.h>

#include "pessoa/pessoa.h"
#include "tipo_pet/tipo_pet.h"
#include "pet/pet.h"

#include "arquivos/arquivos.h"

#include "comandos/fila.h"
#include "comandos/script.h"
#include "comandos/validador.h"
#include "comandos/processador.h"

int main(void) {

    // Listas duplamente encadeadas.
    Pessoa *pessoas = NULL;
    TipoPet *tipos = NULL;
    Pet *pets = NULL;

    // Filas de comandos.
    Fila filaGeral;
    Fila filaPessoas;
    Fila filaTipos;
    Fila filaPets;

    inicializarFila(&filaGeral);
    inicializarFila(&filaPessoas);
    inicializarFila(&filaTipos);
    inicializarFila(&filaPets);

    int sucesso = 0;
    int executados = 0;
    int recusados = 0;

    printf("\n--- SISTEMA DE CADASTRO DE PETS ---\n");

    // 1. Carregamento dos dados.
    printf("\n--- CARREGANDO DADOS ---\n");

    if (!carregarPessoas(&pessoas, "dados/pessoas.dat") ||
        !carregarTiposPet(&tipos, "dados/tipos_pet.dat") ||
        !carregarPets(&pets, pessoas, tipos, "dados/pets.dat")) {

        printf("Erro ao carregar os dados!\n");
        goto finalizar;
    }

    printf("Carregamento concluido!\n");

    // 2. Leitura do arquivo SQL.
    printf("\n--- LEITURA DOS COMANDOS ---\n");

    if (!carregarScriptSQL("comandos.sql", &filaGeral)) {

        printf("Erro ao carregar o script SQL!\n");
        goto finalizar;
    }

    // 3. Validacao da fila geral.
    validarFilaGeral(&filaGeral);

    // 4. Distribuicao dos comandos.
    printf("\n--- DISTRIBUICAO DOS COMANDOS ---\n");

    if (!distribuirComandos(
            &filaGeral,
            &filaPessoas,
            &filaTipos,
            &filaPets)) {

        printf("Erro ao distribuir comandos!\n");
        goto finalizar;
    }

    printf("Comandos distribuidos com sucesso!\n");

    // 5. Execucao na ordem original.
    printf("\n--- EXECUCAO DOS COMANDOS ---\n");

    NoComando comando;

    while (retirarProximoComando(
            &filaPessoas,
            &filaTipos,
            &filaPets,
            &comando)) {

        printf("\nComando %d: %s\n",
               comando.sequencia,
               comando.texto);

        if (executarComandoSQL(
                comando.texto,
                &pessoas,
                &tipos,
                &pets)) {

            executados++;

        } else {

            recusados++;
            printf("Comando recusado!\n");
        }
    }

    // 6. Salvamento dos dados.
    printf("\n--- SALVANDO DADOS ---\n");

    if (!salvarPessoas(pessoas, "dados/pessoas.dat") ||
        !salvarTiposPet(tipos, "dados/tipos_pet.dat") ||
        !salvarPets(pets, "dados/pets.dat")) {

        printf("Erro ao salvar os dados!\n");
        goto finalizar;
    }

    printf("Dados salvos com sucesso!\n");

    // 7. Resumo da execucao.
    printf("\n--- RESUMO DA EXECUCAO ---\n");
    printf("Comandos executados: %d\n", executados);
    printf("Comandos recusados na execucao: %d\n", recusados);

    sucesso = 1;

finalizar:

    // 8. Liberacao da memoria.
    liberarFila(&filaGeral);
    liberarFila(&filaPessoas);
    liberarFila(&filaTipos);
    liberarFila(&filaPets);

    liberarPets(&pets);
    liberarPessoas(&pessoas);
    liberarTiposPet(&tipos);

    if (sucesso) {
        printf("\nPrograma finalizado com sucesso!\n");
    } else {
        printf("\nPrograma encerrado com erros!\n");
    }

    return sucesso ? EXIT_SUCCESS : EXIT_FAILURE;
}