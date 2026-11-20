#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

/* Gravacao e leitura das listas em arquivo texto (um registro por linha, campos separados por ';').
   Ao carregar, as listas ficam na mesma ordem em que foram gravadas. */

#include "pessoa.h"
#include "tipo_pet.h"
#include "pet.h"

/* pessoas.txt:   codigo;nome;fone;data_nascimento;endereco */
int salvarPessoas(const Pessoa *inicio, const char *arq);
int carregarPessoas(Pessoa **inicio, const char *arq);

/* tipos_pet.txt: codigo;descricao */
int salvarTiposPet(const TipoPet *inicio, const char *arq);
int carregarTiposPet(TipoPet **inicio, const char *arq);

/* pets.txt:      codigo;codigo_pes;nome;codigo_tipo */
int salvarPets(const Pet *inicio, const char *arq);
int carregarPets(Pet **inicio, const char *arq);

#endif
