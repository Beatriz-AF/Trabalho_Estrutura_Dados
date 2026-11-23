#include <stdio.h>
#include <string.h>
#include "validador.h"

static int total = 0, falhas = 0;

static const char *nome_tipo(TipoComando t) {
    switch (t) {
    case TIPO_INSERT: return "INSERT";
    case TIPO_UPDATE: return "UPDATE";
    case TIPO_DELETE: return "DELETE";
    default:          return "SELECT";
    }
}

static void mostra_atrib(const Atrib *a) {
    if (a->tipo == VAL_INT) printf("%s = %d (inteiro)", a->campo, a->ival);
    else                    printf("%s = '%s' (texto)", a->campo, a->sval);
}

static void mostra_interpretacao(const ComandoSQL *c) {
    printf("   tipo: %s | tabela: %s\n", nome_tipo(c->tipo), nome_tabela(c->tabela));
    for (int i = 0; i < c->n; i++) {
        printf("   %s: ", c->tipo == TIPO_UPDATE ? "atribuicao" : "campo    ");
        mostra_atrib(&c->atribs[i]);
        printf("\n");
    }
    if (c->tem_where) {
        printf("   where: ");
        mostra_atrib(&c->where);
        printf("\n");
    }
    if (c->tem_order)
        printf("   order by: %s\n", campo_ordenacao(c->tabela));
}

static void caso(const char *texto, int esperado) {
    ComandoSQL c;
    char erro[256];
    int ok = validar_comando(texto, &c, erro, sizeof erro);
    total++;
    if (ok != esperado) {
        falhas++;
        printf("FALHOU (esperado %s): %s\n", esperado ? "valido" : "invalido", texto);
        if (!ok) printf("   motivo: %s\n", erro);
        return;
    }
    if (ok) {
        printf("VALIDO: %s\n", texto);
        mostra_interpretacao(&c);
    } else {
        printf("INVALIDO (esperado): %s\n   motivo: %s\n", texto, erro);
    }
    printf("\n");
}

int main(void) {
    printf("===== comandos VALIDOS =====\n\n");
    caso("insert into pessoa(codigo, nome, fone, data_nascimento) values(1, 'joão', '859995566', '04/10/2002');", 1);
    caso("insert into tipo_pet(codigo, descricao) values(1, 'cachorro');", 1);
    caso("insert into pet(codigo, codigo_pes, nome, codigo_tipo) values(1, 1, 'bilu', 2);", 1);
    caso("insert into pet(codigo, codigo_cli, nome, codigo_tipo) values(1, 1, 'bilu', 2);", 1);
    caso("update pet set nome = 'brother', codigo_tipo = 1 where codigo = 2;", 1);
    caso("update pessoa set fone = '1' where codigo = 1;", 1);
    caso("delete from pet where codigo = 3;", 1);
    caso("select * from pet where codigo = 3;", 1);
    caso("select * from pet order by nome;", 1);
    caso("select * from tipo_pet order by descricao;", 1);
    caso("select * from pessoa;", 1);
    caso("SELECT * FROM pet WHERE codigo = -5 ORDER BY nome;", 1);

    printf("===== comandos INVALIDOS =====\n\n");
    caso("insert into pessoa(codigo, nome, fone) values(1, 'ana', '1');", 0);
    caso("insert into pessoa(codigo, nome, data_nascimento) values(3, josé, '04/10/2001');", 0);
    caso("insert into pessoa(codigo, nome, data_nascimento) values('x', 'ana', '01/01/2000');", 0);
    caso("insert into pessoa(codigo, nome, data_nascimento) values(1, 'ana', '31/02/2000');", 0);
    caso("insert into pessoa(codigo, nome, data_nascimento) values(1, 'ana');", 0);
    caso("insert into pessoa(codigo, nome, data_nascimento, xx) values(1, 'a', '01/01/2000', 1);", 0);
    caso("insert into pessoa(codigo, codigo, nome, data_nascimento) values(1, 2, 'a', '01/01/2000');", 0);
    caso("insert into cliente(codigo) values(1);", 0);
    caso("insert into pet codigo, nome values(1, 'a');", 0);
    caso("update pet set nome = 'a';", 0);
    caso("delete from pet;", 0);
    caso("select * from pet order by codigo;", 0);
    caso("select * from pet where codigo = 3", 0);
    caso("select * from pet where codigo = 3; extra", 0);
    caso("select nome from pet;", 0);
    caso("select * from pet where codigo = 'abc';", 0);
    caso("drop table pet;", 0);
    caso("", 0);

    printf("==============================\n");
    printf("%d caso(s) testado(s), %d falha(s)\n", total, falhas);
    return falhas ? 1 : 0;
}
