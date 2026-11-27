
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


int validarUpdateTipoPet(const char *texto,
                         int *codigo,
                         char descricao[100]);

int validarUpdatePessoa(const char *texto,
                        int *codigo,
                        char nome[100],
                        char fone[20],
                        char data[11],
                        char endereco[150]);


int validarUpdatePet(const char *texto,
                     int *codigo,
                     int *codigoPes,
                     char nome[100],
                     int *codigoTipo);


int validarSelectSQL(const char *texto,
                     TipoEntidade *entidade,
                     int *temWhere,
                     int *codigo);

int validarSelectOrderBySQL(const char *texto,
                            TipoEntidade *entidade);


int validarInsertTipoPetComCampos(const char *texto,
                                 int *codigo,
                                 char descricao[100]);


int validarInsertPessoaComCampos(
    const char *texto,
    int *codigo,
    char nome[100],
    char fone[20],
    char data[11],
    char endereco[150]);


int validarInsertPetComCampos(
    const char *texto,
    int *codigo,
    int *codigoPes,
    char nome[100],
    int *codigoTipo);


#define CAMPO_PESSOA_NOME     1u
#define CAMPO_PESSOA_FONE     2u
#define CAMPO_PESSOA_DATA     4u
#define CAMPO_PESSOA_ENDERECO 8u

int validarUpdatePessoaParcial(
    const char *texto,
    int *codigo,
    unsigned int *campos,
    char nome[100],
    char fone[20],
    char data[11],
    char endereco[150]);

    #define CAMPO_PET_DONO 1u
#define CAMPO_PET_NOME 2u
#define CAMPO_PET_TIPO 4u

int validarUpdatePetParcial(
    const char *texto,
    int *codigo,
    unsigned int *campos,
    int *codigoPes,
    char nome[100],
    int *codigoTipo);
#endif