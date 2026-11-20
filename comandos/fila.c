#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

void fila_criar(Fila *f) {
    f->ini = f->fim = NULL;
    f->tam = 0;
    f->seq_prox = 0;
}

int fila_vazia(const Fila *f) {
    return f->ini == NULL;
}

static void anexa(Fila *f, Comando *no) {
    no->prox = NULL;
    if (f->fim) f->fim->prox = no;
    else f->ini = no;
    f->fim = no;
    f->tam++;
}

int fila_enfileirar(Fila *f, const char *descricao) {
    Comando *no = malloc(sizeof *no);
    if (!no) return 0;
    no->seq = f->seq_prox++;
    no->tabela = TAB_PESSOA;
    snprintf(no->descricao, sizeof no->descricao, "%s", descricao);
    anexa(f, no);
    return 1;
}

/* Anexa um no ja existente, preservando seu seq */
void fila_anexar(Fila *f, Comando *no) {
    anexa(f, no);
}

Comando *fila_desenfileirar(Fila *f) {
    Comando *no = f->ini;
    if (!no) return NULL;
    f->ini = no->prox;
    if (!f->ini) f->fim = NULL;
    f->tam--;
    no->prox = NULL;
    return no;
}

void fila_liberar(Fila *f) {
    Comando *no;
    while ((no = fila_desenfileirar(f)) != NULL) free(no);
}

static int somente_espacos(const char *s) {
    for (; *s; s++)
        if (!isspace((unsigned char)*s)) return 0;
    return 1;
}

/* Le o script inteiro e separa comandos pelo ';' (ignorando ';' dentro de aspas) */
int ler_comandos(FILE *arq, Fila *f) {
    char buf[MAX_LINHA];
    size_t n = 0;
    int aspas = 0, c;

    while ((c = fgetc(arq)) != EOF) {
        if (c == '\'') aspas = !aspas;
        if (n < sizeof buf - 1) buf[n++] = (char)c;
        if (c == ';' && !aspas) {
            buf[n] = '\0';
            if (!fila_enfileirar(f, buf)) return 0;
            n = 0;
        }
    }
    buf[n] = '\0';
    /* sobra sem ';' final: entra na fila para ser reportada como invalida */
    if (n > 0 && !somente_espacos(buf))
        if (!fila_enfileirar(f, buf)) return 0;
    return 1;
}
