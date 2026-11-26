
#ifndef LEITOR_H
#define LEITOR_H

#include "fila.h"

typedef enum {
    OPERACAO_INSERT,
    OPERACAO_UPDATE,
    OPERACAO_DELETE,
    OPERACAO_SELECT
} TipoOperacao;

int identificarComando(const char *texto,
                      TipoOperacao *operacao,
                      TipoEntidade *entidade);

                      int adicionarComandoSQL(Fila *fila,
                       const char *texto,
                       int sequencia);
                       
#endif
