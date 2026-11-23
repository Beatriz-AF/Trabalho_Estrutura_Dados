#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

int salvarPessoas(const Pessoa *inicio, const char *arq);
int carregarPessoas(Pessoa **inicio, const char *arq);

int salvarTiposPet(const TipoPet *inicio, const char *arq);
int carregarTiposPet(TipoPet **inicio, const char *arq);

int salvarPets(const Pet *inicio, const char *arq);
int carregarPets(Pet **inicio, const char *arq);

#endif
