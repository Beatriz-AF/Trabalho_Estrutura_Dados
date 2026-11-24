
#ifndef INTEGRIDADE_H
#define INTEGRIDADE_H

#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

int cadastrarPetValidado(Pet **listaPets,
                        Pessoa *listaPessoas,
                        TipoPet *listaTipos,
                        int codigo,
                        int codigoPes,
                        const char *nome,
                        int codigoTipo);


int atualizarPetValidado(Pet *listaPets,
                        Pessoa *listaPessoas,
                        TipoPet *listaTipos,
                        int codigo,
                        int codigoPes,
                        const char *nome,
                        int codigoTipo);

int removerPessoaSeguro(Pessoa **listaPessoas,
                       Pet *listaPets,
                       int codigo);

int removerTipoPetSeguro(TipoPet **listaTipos,
                       Pet *listaPets,
                       int codigo);


#endif
