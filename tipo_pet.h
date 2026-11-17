#ifndef TIPO_PET_H
#define TIPO_PET_H

typedef struct tipo_pet {
    int codigo;
    char descricao[100];

    struct tipo_pet *ant;
    struct tipo_pet *prox;
} TipoPet;

TipoPet *buscarTipoPet(TipoPet *inicio, int codigo);

int inserirTipoPet(TipoPet **inicio, int codigo,
                   const char *descricao);

int atualizarTipoPet(TipoPet *inicio, int codigo,
                     const char *descricao);

int removerTipoPet(TipoPet **inicio, int codigo);

void listarTiposPet(TipoPet *inicio);

void liberarTiposPet(TipoPet **inicio);

#endif