#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include "fila.h"
#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

typedef struct {
    Pessoa *pessoas;
    TipoPet *tipos;
    Pet *pets;
} Banco;

void filtrar_validos(Fila *entrada, Fila *validos);

void distribuir(Fila *validos, Fila *fpessoa, Fila *ftipo, Fila *fpet);

void processar_filas(Banco *b, Fila *fpessoa, Fila *ftipo, Fila *fpet);

#endif
