
#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include "../pessoa/pessoa.h"
#include "../tipo_pet/tipo_pet.h"
#include "../pet/pet.h"

int executarInsertSQL(const char *texto,
                     Pessoa **pessoas,
                     TipoPet **tipos,
                     Pet **pets);


int executarDeleteSQL(const char *texto,
                     Pessoa **pessoas,
                     TipoPet **tipos,
                     Pet **pets);


int executarUpdateTipoPetSQL(const char *texto,
                             TipoPet **tipos);


int executarUpdatePessoaSQL(const char *texto,
                           Pessoa *pessoas);

#endif
