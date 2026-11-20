#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validador.h"

typedef struct {
    const char *nome;
    int inteiro;
    int obrigatorio;
    int data;
} CampoInfo;

static const CampoInfo CAMPOS_PESSOA[] = {
    {"codigo", 1, 1, 0},
    {"nome", 0, 1, 0},
    {"fone", 0, 0, 0},
    {"endereco", 0, 0, 0},
    {"data_nascimento", 0, DATA_NASCIMENTO_OBRIGATORIA, 1}
};

static const CampoInfo CAMPOS_TIPO_PET[] = {
    {"codigo", 1, 1, 0},
    {"descricao", 0, 1, 0}
};

static const CampoInfo CAMPOS_PET[] = {
    {"codigo", 1, 1, 0},
    {"codigo_pes", 1, 1, 0},
    {"nome", 0, 1, 0},
    {"codigo_tipo", 1, 1, 0}
};

static const CampoInfo *campos_de(Tabela t, int *qtd) {
    switch (t) {
    case TAB_PESSOA:
        *qtd = sizeof CAMPOS_PESSOA / sizeof CAMPOS_PESSOA[0];
        return CAMPOS_PESSOA;
    case TAB_TIPO_PET:
        *qtd = sizeof CAMPOS_TIPO_PET / sizeof CAMPOS_TIPO_PET[0];
        return CAMPOS_TIPO_PET;
    default:
        *qtd = sizeof CAMPOS_PET / sizeof CAMPOS_PET[0];
        return CAMPOS_PET;
    }
}

static const CampoInfo *campo_info(Tabela t, const char *nome) {
    int qtd;
    const CampoInfo *ci = campos_de(t, &qtd);
    for (int i = 0; i < qtd; i++)
        if (!strcmp(ci[i].nome, nome)) return &ci[i];
    return NULL;
}

const char *nome_tabela(Tabela t) {
    return t == TAB_PESSOA ? "pessoa" : t == TAB_TIPO_PET ? "tipo_pet" : "pet";
}

const char *campo_ordenacao(Tabela t) {
    return t == TAB_TIPO_PET ? "descricao" : "nome";
}

int data_valida(const char *s) {
    static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d, m, a, max;
    if (strlen(s) != 10 || s[2] != '/' || s[5] != '/') return 0;
    for (int i = 0; i < 10; i++)
        if (i != 2 && i != 5 && !isdigit((unsigned char)s[i])) return 0;
    d = atoi(s);
    m = atoi(s + 3);
    a = atoi(s + 6);
    if (a < 1900 || a > 2100 || m < 1 || m > 12 || d < 1) return 0;
    max = dias[m - 1];
    if (m == 2 && ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0)) max = 29;
    return d <= max;
}

int dividir_linha(char *linha, char *partes[], int max) {
    int n = 0;
    char *s = linha;
    partes[n++] = s;
    while (n < max && (s = strchr(s, ';')) != NULL) {
        *s = '\0';
        s++;
        partes[n++] = s;
    }
    return n;
}

/* ---------- analisador (lexer/parser simples) ---------- */

static int falha(char *erro, size_t tam, const char *msg) {
    if (erro && tam) snprintf(erro, tam, "%s", msg);
    return 0;
}

static void pular(const char **p) {
    while (isspace((unsigned char)**p)) (*p)++;
}

/* Le identificador (letras, digitos, _), convertido para minusculas */
static int ler_ident(const char **p, char *buf, size_t tam) {
    size_t n = 0;
    pular(p);
    if (!isalpha((unsigned char)**p) && **p != '_') return 0;
    while (isalnum((unsigned char)**p) || **p == '_') {
        if (n + 1 >= tam) return 0;
        buf[n++] = (char)tolower((unsigned char)**p);
        (*p)++;
    }
    buf[n] = '\0';
    return 1;
}

static int ler_char(const char **p, char c) {
    pular(p);
    if (**p != c) return 0;
    (*p)++;
    return 1;
}

static int ler_kw(const char **p, const char *kw) {
    char b[MAX_IDENT];
    const char *q = *p;
    if (!ler_ident(&q, b, sizeof b) || strcmp(b, kw)) return 0;
    *p = q;
    return 1;
}

/* Valor: 'texto' (aspas simples) ou inteiro (opcionalmente negativo) */
static int ler_valor(const char **p, Atrib *a) {
    pular(p);
    if (**p == '\'') {
        size_t n = 0;
        (*p)++;
        while (**p && **p != '\'') {
            if (n + 1 >= MAX_TEXTO) return 0;
            a->sval[n++] = **p;
            (*p)++;
        }
        if (**p != '\'') return 0;
        (*p)++;
        a->sval[n] = '\0';
        a->tipo = VAL_STR;
        return strchr(a->sval, ';') == NULL && strchr(a->sval, '\n') == NULL;
    }
    {
        const char *q = *p;
        int neg = 0, digitos = 0;
        long v = 0;
        if (*q == '-') { neg = 1; q++; }
        while (isdigit((unsigned char)*q)) {
            if (++digitos > 9) return 0;
            v = v * 10 + (*q++ - '0');
        }
        if (!digitos) return 0;
        a->tipo = VAL_INT;
        a->ival = (int)(neg ? -v : v);
        *p = q;
        return 1;
    }
}

static void copia_valor(Atrib *dst, const Atrib *src) {
    dst->tipo = src->tipo;
    dst->ival = src->ival;
    snprintf(dst->sval, sizeof dst->sval, "%s", src->sval);
}

/* Confere tipo do valor e regras do campo (data, obrigatorio vazio) */
static int checa_valor(const CampoInfo *ci, const Atrib *a, char *erro, size_t tam) {
    char msg[320];
    if (ci->inteiro && a->tipo != VAL_INT) {
        snprintf(msg, sizeof msg, "campo '%s' espera numero inteiro", ci->nome);
        return falha(erro, tam, msg);
    }
    if (!ci->inteiro && a->tipo != VAL_STR) {
        snprintf(msg, sizeof msg, "campo '%s' espera texto entre aspas simples", ci->nome);
        return falha(erro, tam, msg);
    }
    if (ci->data && a->sval[0] && !data_valida(a->sval)) {
        snprintf(msg, sizeof msg, "data invalida em '%s' (use dd/mm/aaaa)", a->sval);
        return falha(erro, tam, msg);
    }
    if (!ci->inteiro && ci->obrigatorio && a->sval[0] == '\0') {
        snprintf(msg, sizeof msg, "campo '%s' nao pode ser vazio", ci->nome);
        return falha(erro, tam, msg);
    }
    return 1;
}

/* Le nome de campo, aplica alias (codigo_cli -> codigo_pes) e confere existencia */
static int ler_campo(const char **p, Tabela t, char *out, size_t tam, char *erro, size_t etam) {
    char id[MAX_IDENT];
    char msg[160];
    if (!ler_ident(p, id, sizeof id)) return falha(erro, etam, "nome de campo invalido");
    if (t == TAB_PET && !strcmp(id, "codigo_cli"))
        snprintf(id, sizeof id, "%s", "codigo_pes");
    if (!campo_info(t, id)) {
        snprintf(msg, sizeof msg, "campo '%s' nao existe na tabela %s", id, nome_tabela(t));
        return falha(erro, etam, msg);
    }
    snprintf(out, tam, "%s", id);
    return 1;
}

static int ler_tabela(const char **p, Tabela *t) {
    char id[MAX_IDENT];
    if (!ler_ident(p, id, sizeof id)) return 0;
    if (!strcmp(id, "pessoa")) *t = TAB_PESSOA;
    else if (!strcmp(id, "tipo_pet")) *t = TAB_TIPO_PET;
    else if (!strcmp(id, "pet")) *t = TAB_PET;
    else return 0;
    return 1;
}

/* where campo = valor */
static int ler_condicao(const char **p, Tabela t, Atrib *w, char *erro, size_t tam) {
    char campo[MAX_IDENT];
    memset(w, 0, sizeof *w);
    if (!ler_campo(p, t, campo, sizeof campo, erro, tam)) return 0;
    snprintf(w->campo, sizeof w->campo, "%s", campo);
    if (!ler_char(p, '=')) return falha(erro, tam, "esperado '=' na condicao");
    if (!ler_valor(p, w)) return falha(erro, tam, "valor invalido na condicao (use numero ou 'texto')");
    return checa_valor(campo_info(t, campo), w, erro, tam);
}

/* Confere ';' final e que nao ha texto sobrando */
static int fim(const char **p, char *erro, size_t tam) {
    if (!ler_char(p, ';')) return falha(erro, tam, "falta ';' no final do comando");
    pular(p);
    if (**p) return falha(erro, tam, "ha texto depois do ';'");
    return 1;
}

static int parse_insert(const char **p, ComandoSQL *c, char *erro, size_t tam) {
    int qtd, i, j;
    const CampoInfo *ci;
    if (!ler_kw(p, "into")) return falha(erro, tam, "esperado 'into' apos insert");
    if (!ler_tabela(p, &c->tabela)) return falha(erro, tam, "tabela desconhecida (use pessoa, tipo_pet ou pet)");
    if (!ler_char(p, '(')) return falha(erro, tam, "esperado '(' apos o nome da tabela");

    c->n = 0;
    do {
        if (c->n >= MAX_CAMPOS) return falha(erro, tam, "campos demais no insert");
        if (!ler_campo(p, c->tabela, c->atribs[c->n].campo, MAX_IDENT, erro, tam)) return 0;
        for (j = 0; j < c->n; j++)
            if (!strcmp(c->atribs[j].campo, c->atribs[c->n].campo))
                return falha(erro, tam, "campo repetido no insert");
        c->n++;
    } while (ler_char(p, ','));
    if (!ler_char(p, ')')) return falha(erro, tam, "esperado ')' na lista de campos");

    if (!ler_kw(p, "values")) return falha(erro, tam, "esperado 'values'");
    if (!ler_char(p, '(')) return falha(erro, tam, "esperado '(' apos values");
    for (i = 0; i < c->n; i++) {
        Atrib v;
        memset(&v, 0, sizeof v);
        if (!ler_valor(p, &v)) return falha(erro, tam, "valor invalido (use numero ou 'texto')");
        copia_valor(&c->atribs[i], &v);
        if (!checa_valor(campo_info(c->tabela, c->atribs[i].campo), &c->atribs[i], erro, tam))
            return 0;
        if (i < c->n - 1 && !ler_char(p, ','))
            return falha(erro, tam, "quantidade de valores diferente da de campos");
    }
    if (!ler_char(p, ')')) return falha(erro, tam, "quantidade de valores diferente da de campos");

    /* campos obrigatorios (*) precisam estar presentes */
    ci = campos_de(c->tabela, &qtd);
    for (i = 0; i < qtd; i++) {
        int achou = 0;
        char msg[120];
        if (!ci[i].obrigatorio) continue;
        for (j = 0; j < c->n; j++)
            if (!strcmp(c->atribs[j].campo, ci[i].nome)) achou = 1;
        if (!achou) {
            snprintf(msg, sizeof msg, "campo obrigatorio '%s' nao informado", ci[i].nome);
            return falha(erro, tam, msg);
        }
    }
    return fim(p, erro, tam);
}

static int parse_update(const char **p, ComandoSQL *c, char *erro, size_t tam) {
    if (!ler_tabela(p, &c->tabela)) return falha(erro, tam, "tabela desconhecida (use pessoa, tipo_pet ou pet)");
    if (!ler_kw(p, "set")) return falha(erro, tam, "esperado 'set' apos o nome da tabela");
    c->n = 0;
    do {
        Atrib v;
        if (c->n >= MAX_CAMPOS) return falha(erro, tam, "atribuicoes demais no update");
        if (!ler_campo(p, c->tabela, c->atribs[c->n].campo, MAX_IDENT, erro, tam)) return 0;
        if (!ler_char(p, '=')) return falha(erro, tam, "esperado '=' na atribuicao");
        memset(&v, 0, sizeof v);
        if (!ler_valor(p, &v)) return falha(erro, tam, "valor invalido na atribuicao");
        copia_valor(&c->atribs[c->n], &v);
        if (!checa_valor(campo_info(c->tabela, c->atribs[c->n].campo), &c->atribs[c->n], erro, tam))
            return 0;
        c->n++;
    } while (ler_char(p, ','));
    if (!ler_kw(p, "where")) return falha(erro, tam, "update exige clausula where");
    if (!ler_condicao(p, c->tabela, &c->where, erro, tam)) return 0;
    c->tem_where = 1;
    return fim(p, erro, tam);
}

static int parse_delete(const char **p, ComandoSQL *c, char *erro, size_t tam) {
    if (!ler_kw(p, "from")) return falha(erro, tam, "esperado 'from' apos delete");
    if (!ler_tabela(p, &c->tabela)) return falha(erro, tam, "tabela desconhecida (use pessoa, tipo_pet ou pet)");
    if (!ler_kw(p, "where")) return falha(erro, tam, "delete exige clausula where");
    if (!ler_condicao(p, c->tabela, &c->where, erro, tam)) return 0;
    c->tem_where = 1;
    return fim(p, erro, tam);
}

static int parse_select(const char **p, ComandoSQL *c, char *erro, size_t tam) {
    char campo[MAX_IDENT];
    char msg[160];
    if (!ler_char(p, '*')) return falha(erro, tam, "esperado '*' apos select");
    if (!ler_kw(p, "from")) return falha(erro, tam, "esperado 'from' apos select *");
    if (!ler_tabela(p, &c->tabela)) return falha(erro, tam, "tabela desconhecida (use pessoa, tipo_pet ou pet)");
    if (ler_kw(p, "where")) {
        if (!ler_condicao(p, c->tabela, &c->where, erro, tam)) return 0;
        c->tem_where = 1;
    }
    if (ler_kw(p, "order")) {
        if (!ler_kw(p, "by")) return falha(erro, tam, "esperado 'by' apos order");
        if (!ler_campo(p, c->tabela, campo, sizeof campo, erro, tam)) return 0;
        if (strcmp(campo, campo_ordenacao(c->tabela))) {
            snprintf(msg, sizeof msg, "order by so aceita '%s' nesta tabela", campo_ordenacao(c->tabela));
            return falha(erro, tam, msg);
        }
        c->tem_order = 1;
    }
    return fim(p, erro, tam);
}

int validar_comando(const char *texto, ComandoSQL *c, char *erro, size_t tam) {
    const char *p = texto;
    char kw[MAX_IDENT];
    memset(c, 0, sizeof *c);
    if (erro && tam) erro[0] = '\0';
    if (!ler_ident(&p, kw, sizeof kw))
        return falha(erro, tam, "comando nao reconhecido");
    if (!strcmp(kw, "insert")) { c->tipo = TIPO_INSERT; return parse_insert(&p, c, erro, tam); }
    if (!strcmp(kw, "update")) { c->tipo = TIPO_UPDATE; return parse_update(&p, c, erro, tam); }
    if (!strcmp(kw, "delete")) { c->tipo = TIPO_DELETE; return parse_delete(&p, c, erro, tam); }
    if (!strcmp(kw, "select")) { c->tipo = TIPO_SELECT; return parse_select(&p, c, erro, tam); }
    return falha(erro, tam, "comando deve iniciar com insert, update, delete ou select");
}
