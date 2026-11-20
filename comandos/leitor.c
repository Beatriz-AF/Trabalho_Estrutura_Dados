#include <stdio.h>
#include "leitor.h"

int ler_script_arquivo(const char *nome, Fila *f) {
    FILE *arq = fopen(nome, "r");
    int ok;
    if (!arq) {
        perror("Nao foi possivel abrir o script");
        return 0;
    }
    ok = ler_comandos(arq, f);
    fclose(arq);
    if (!ok) fprintf(stderr, "Erro: falta de memoria ao ler o script.\n");
    return ok;
}

int ler_script_usuario(Fila *f) {
    int ok;
    printf("Digite os comandos (cada um termina com ';').\n"
           "Finalize com Ctrl+D (Linux/Mac) ou Ctrl+Z e Enter (Windows).\n");
    ok = ler_comandos(stdin, f);
    if (!ok) fprintf(stderr, "Erro: falta de memoria ao ler os comandos.\n");
    return ok;
}
