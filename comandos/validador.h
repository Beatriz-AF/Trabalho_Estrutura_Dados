
#ifndef VALIDADOR_H
#define VALIDADOR_H

int validarInsertTipoPet(const char *texto,
                         int *codigo,
                         char descricao[100]);


int validarInsertPessoa(const char *texto,
                        int *codigo,
                        char nome[100],
                        char fone[20],
                        char data[11],
                        char endereco[150]);


int validarInsertPet(const char *texto,
                     int *codigo,
                     int *codigoPes,
                     char nome[100],
                     int *codigoTipo);


#include "fila.h"

int validarDeleteSQL(const char *texto,
                     TipoEntidade *entidade,
                     int *codigo);

#endif
