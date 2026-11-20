#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include "fila.h"
#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

/* Listas gerais: ponteiros para o inicio de cada lista (mesmo padrao do cadastro) */
typedef struct {
    Pessoa *pessoas;
    TipoPet *tipos;
    Pet *pets;
} Banco;

/* 1) Filtra a fila de comandos, deixando apenas os validos */
void filtrar_validos(Fila *entrada, Fila *validos);

/* 2) Separa os validos em filas de pessoa, tipo_pet e pet */
void distribuir(Fila *validos, Fila *fpessoa, Fila *ftipo, Fila *fpet);

/* 3) Executa as operacoes na ordem original do script, chamando o cadastro */
void processar_filas(Banco *b, Fila *fpessoa, Fila *ftipo, Fila *fpet);

#endif
