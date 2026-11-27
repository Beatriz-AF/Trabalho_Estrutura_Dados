
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "script.h"
#include "leitor.h"

int carregarScriptSQL(const char *nomeArquivo,
                      Fila *filaGeral) {

    if (nomeArquivo == NULL || filaGeral == NULL) {
        return 0;
    }

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo SQL!\n");
        return 0;
    }

    char linha[1024];
    int numeroLinha = 0;
    int sequencia = 1;

    // A fila geral deve estar vazia antes da leitura.
    if (!filaVazia(filaGeral)) {
        printf("Erro: fila geral nao esta vazia!\n");
        fclose(arquivo);
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        numeroLinha++;

        size_t tamanho = strlen(linha);

        // Rejeita linhas maiores que o limite.
        if (strchr(linha, '\n') == NULL && !feof(arquivo)) {
            printf("Erro: linha %d muito longa!\n",
                   numeroLinha);
            fclose(arquivo);
            return 0;
        }

        // Remove espacos e quebras de linha do final.
        while (tamanho > 0 &&
               isspace((unsigned char)linha[tamanho - 1])) {

            linha[--tamanho] = '\0';
        }

        char *comando = linha;

        // Ignora espacos no inicio.
        while (isspace((unsigned char)*comando)) {
            comando++;
        }

        // Ignora linhas vazias e comentarios.
        if (*comando == '\0' ||
            (comando[0] == '-' && comando[1] == '-')) {
            continue;
        }

        if (strlen(comando) >= TAM_COMANDO) {
            printf("Erro: comando da linha %d muito longo!\n",
                   numeroLinha);
            fclose(arquivo);
            return 0;
        }

        if (sequencia == INT_MAX) {
            printf("Erro: muitos comandos!\n");
            fclose(arquivo);
            return 0;
        }

        TipoOperacao operacao;
        TipoEntidade entidade;

        // Ignora comandos nao reconhecidos.
        if (!identificarComando(comando,
                            &operacao, &entidade)) {

            printf("Comando invalido na linha %d!\n",
                numeroLinha);
            continue;
        }

        // Adiciona o comando a fila geral.
        if (!enfileirar(filaGeral, comando,
                        sequencia, entidade)) {

            fclose(arquivo);
            return 0;
        }

        sequencia++;
    }

    if (ferror(arquivo)) {
        printf("Erro durante a leitura do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    printf("%d comandos carregados!\n",
           sequencia - 1);

    return 1;
}
