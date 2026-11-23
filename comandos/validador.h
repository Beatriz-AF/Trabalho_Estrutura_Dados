#ifndef VALIDADOR_H
#define VALIDADOR_H

#include <stddef.h>

#define DATA_NASCIMENTO_OBRIGATORIA 1

#define MAX_TEXTO  256
#define MAX_IDENT  32
#define MAX_CAMPOS 8
#define MAX_LINHA  1024

typedef enum { TIPO_INSERT, TIPO_UPDATE, TIPO_DELETE, TIPO_SELECT } TipoComando;
typedef enum { TAB_PESSOA = 0, TAB_TIPO_PET, TAB_PET } Tabela;
typedef enum { VAL_INT, VAL_STR } TipoValor;

typedef struct {
    char campo[MAX_IDENT];
    TipoValor tipo;
    int ival;
    char sval[MAX_TEXTO];
} Atrib;

typedef struct {
    TipoComando tipo;
    Tabela tabela;
    int n;
    Atrib atribs[MAX_CAMPOS];
    int tem_where;
    Atrib where;
    int tem_order;
} ComandoSQL;

int validar_comando(const char *texto, ComandoSQL *c, char *erro, size_t tam);
const char *nome_tabela(Tabela t);
const char *campo_ordenacao(Tabela t);
int data_valida(const char *s);
int dividir_linha(char *linha, char *partes[], int max);

#endif
