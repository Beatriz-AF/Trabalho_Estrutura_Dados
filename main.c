#include <stdio.h>
#include <string.h>
#include "processador.h"
#include "persistencia.h"
#include "leitor.h"

int main(int argc, char *argv[]) {
    Banco b;
    Fila comandos, validos, fpessoa, ftipo, fpet;
    char opcao[16], nome_arq[256] = "";
    int ok;

    b.pessoas = NULL;
    b.tipos = NULL;
    b.pets = NULL;
    if (!carregarPessoas(&b.pessoas, "pessoas.txt") ||
        !carregarTiposPet(&b.tipos, "tipos_pet.txt") ||
        !carregarPets(&b.pets, "pets.txt")) {
        fprintf(stderr, "Erro: falta de memoria ao carregar os dados.\n");
        return 1;
    }
    printf("Dados carregados dos arquivos.\n\n");

    fila_criar(&comandos);
    fila_criar(&validos);
    fila_criar(&fpessoa);
    fila_criar(&ftipo);
    fila_criar(&fpet);

    if (argc > 1) {
        ok = ler_script_arquivo(argv[1], &comandos);
    } else {
        printf("1 - ler script de arquivo\n2 - digitar comandos\nEscolha: ");
        if (!fgets(opcao, sizeof opcao, stdin)) return 1;
        if (opcao[0] == '1') {
            printf("Nome do arquivo: ");
            if (!fgets(nome_arq, sizeof nome_arq, stdin)) return 1;
            nome_arq[strcspn(nome_arq, "\r\n")] = '\0';
            ok = ler_script_arquivo(nome_arq, &comandos);
        } else {
            ok = ler_script_usuario(&comandos);
        }
    }
    if (!ok) return 1;
    printf("%d comando(s) lido(s)\n\n", comandos.tam);

    filtrar_validos(&comandos, &validos);
    printf("\n%d comando(s) valido(s)\n", validos.tam);

    distribuir(&validos, &fpessoa, &ftipo, &fpet);
    printf("Filas: pessoa=%d, tipo_pet=%d, pet=%d\n", fpessoa.tam, ftipo.tam, fpet.tam);

    printf("\n----- processamento -----");
    processar_filas(&b, &fpessoa, &ftipo, &fpet);
    printf("\n-------------------------\n");

    if (salvarPessoas(b.pessoas, "pessoas.txt") &&
        salvarTiposPet(b.tipos, "tipos_pet.txt") &&
        salvarPets(b.pets, "pets.txt"))
        printf("Alteracoes salvas nos arquivos.\n");
    else
        fprintf(stderr, "Erro ao salvar os arquivos de dados.\n");

    liberarPessoas(&b.pessoas);
    liberarTiposPet(&b.tipos);
    liberarPets(&b.pets);
    fila_liberar(&comandos);
    fila_liberar(&validos);
    fila_liberar(&fpessoa);
    fila_liberar(&ftipo);
    fila_liberar(&fpet);
    return 0;
}
