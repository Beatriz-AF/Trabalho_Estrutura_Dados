#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "../pessoa/pessoa.h"
#include "../tipo_pet/tipo_pet.h"
#include "../pet/pet.h"

int salvarPessoas(Pessoa *inicio, const char *nomeArquivo);

int salvarTiposPet(TipoPet *inicio, const char *nomeArquivo);

int salvarPets(Pet *inicio, const char *nomeArquivo);


int carregarPessoas(Pessoa **inicio, const char *nomeArquivo);

int carregarTiposPet(TipoPet **inicio, const char *nomeArquivo);

int carregarPets(Pet **inicio,
                 Pessoa *listaPessoas,
                 TipoPet *listaTipos,
                 const char *nomeArquivo);

#endif
