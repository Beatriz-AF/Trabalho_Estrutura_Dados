#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processador.h"
#include "arvore.h"

/* ---------- acesso aos campos do comando ---------- */

static const Atrib *achaAtrib(const ComandoSQL *c, const char *campo) {
    int i;
    for (i = 0; i < c->n; i++)
        if (!strcmp(c->atribs[i].campo, campo))
            return &c->atribs[i];
    return NULL;
}

/* Preenche *v e devolve 1 se o campo inteiro existe no comando */
static int obterInt(const ComandoSQL *c, const char *campo, int *v) {
    const Atrib *a = achaAtrib(c, campo);
    if (a == NULL)
        return 0;
    *v = a->ival;
    return 1;
}

/* Devolve o texto do campo, ou NULL se ele nao esta no comando */
static const char *obterStr(const ComandoSQL *c, const char *campo) {
    const Atrib *a = achaAtrib(c, campo);
    return a != NULL ? a->sval : NULL;
}

/* ---------- filtro (where): sem where, todos os registros casam ---------- */

static int casaPessoa(const Pessoa *p, const ComandoSQL *c) {
    const Atrib *w = &c->where;
    if (!c->tem_where) return 1;
    if (!strcmp(w->campo, "codigo")) return p->codigo == w->ival;
    if (!strcmp(w->campo, "nome")) return !strcmp(p->nome, w->sval);
    if (!strcmp(w->campo, "fone")) return !strcmp(p->fone, w->sval);
    if (!strcmp(w->campo, "endereco")) return !strcmp(p->endereco, w->sval);
    if (!strcmp(w->campo, "data_nascimento")) return !strcmp(p->data_nascimento, w->sval);
    return 0;
}

static int casaTipo(const TipoPet *t, const ComandoSQL *c) {
    const Atrib *w = &c->where;
    if (!c->tem_where) return 1;
    if (!strcmp(w->campo, "codigo")) return t->codigo == w->ival;
    if (!strcmp(w->campo, "descricao")) return !strcmp(t->descricao, w->sval);
    return 0;
}

static int casaPet(const Pet *p, const ComandoSQL *c) {
    const Atrib *w = &c->where;
    if (!c->tem_where) return 1;
    if (!strcmp(w->campo, "codigo")) return p->codigo == w->ival;
    if (!strcmp(w->campo, "codigo_pes")) return p->codigo_pes == w->ival;
    if (!strcmp(w->campo, "codigo_tipo")) return p->codigo_tipo == w->ival;
    if (!strcmp(w->campo, "nome")) return !strcmp(p->nome, w->sval);
    return 0;
}

/* ---------- regras que dependem de outras listas ---------- */

static int pessoaTemPets(const Pet *pets, int codigoPessoa) {
    const Pet *p;
    for (p = pets; p != NULL; p = p->prox)
        if (p->codigo_pes == codigoPessoa) return 1;
    return 0;
}

static int tipoEmUso(const Pet *pets, int codigoTipo) {
    const Pet *p;
    for (p = pets; p != NULL; p = p->prox)
        if (p->codigo_tipo == codigoTipo) return 1;
    return 0;
}

/* ---------- formatacao para o select ---------- */

static void formataPessoa(const Pessoa *p, char *buf, size_t tam) {
    snprintf(buf, tam, "codigo=%d | nome=%s | fone=%s | endereco=%s | data_nascimento=%s",
             p->codigo, p->nome, p->fone, p->endereco, p->data_nascimento);
}

static void formataTipo(const TipoPet *t, char *buf, size_t tam) {
    snprintf(buf, tam, "codigo=%d | descricao=%s", t->codigo, t->descricao);
}

static void formataPet(const Pet *p, char *buf, size_t tam) {
    snprintf(buf, tam, "codigo=%d | codigo_pes=%d | nome=%s | codigo_tipo=%d",
             p->codigo, p->codigo_pes, p->nome, p->codigo_tipo);
}

/* ---------- 1) filtragem ---------- */

void filtrar_validos(Fila *entrada, Fila *validos) {
    char erro[256];
    ComandoSQL cmd;
    Comando *no;

    while ((no = fila_desenfileirar(entrada)) != NULL) {
        if (validar_comando(no->descricao, &cmd, erro, sizeof erro)) {
            no->tabela = cmd.tabela;
            fila_anexar(validos, no);
        } else {
            printf("[INVALIDO #%d] %s\n     motivo: %s\n", no->seq + 1,
                   no->descricao + strspn(no->descricao, " \t\r\n"), erro);
            free(no);
        }
    }
}

/* ---------- 2) distribuicao ---------- */

void distribuir(Fila *validos, Fila *fpessoa, Fila *ftipo, Fila *fpet) {
    Comando *no;

    while ((no = fila_desenfileirar(validos)) != NULL) {
        if (no->tabela == TAB_PESSOA) fila_anexar(fpessoa, no);
        else if (no->tabela == TAB_TIPO_PET) fila_anexar(ftipo, no);
        else fila_anexar(fpet, no);
    }
}

/* ---------- 3) execucao ---------- */

/* Fila cujo primeiro comando tem o menor seq (ordem do script) */
static Fila *fila_mais_antiga(Fila *a, Fila *b, Fila *c) {
    Fila *filas[3];
    Fila *melhor = NULL;
    int i;

    filas[0] = a;
    filas[1] = b;
    filas[2] = c;
    for (i = 0; i < 3; i++)
        if (!fila_vazia(filas[i]) &&
            (melhor == NULL || filas[i]->ini->seq < melhor->ini->seq))
            melhor = filas[i];
    return melhor;
}

static void exec_insert(Banco *b, const ComandoSQL *c) {
    int cod = 0, pes = 0, tip = 0, ok = 0;

    obterInt(c, "codigo", &cod);

    switch (c->tabela) {
    case TAB_PESSOA:
        ok = inserirPessoa(&b->pessoas, cod, obterStr(c, "nome"), obterStr(c, "fone"),
                           obterStr(c, "data_nascimento"), obterStr(c, "endereco"));
        break;

    case TAB_TIPO_PET:
        ok = inserirTipoPet(&b->tipos, cod, obterStr(c, "descricao"));
        break;

    case TAB_PET:
        obterInt(c, "codigo_pes", &pes);
        obterInt(c, "codigo_tipo", &tip);
        if (buscarPessoa(b->pessoas, pes) == NULL) {
            printf("ERRO: codigo_pes=%d nao corresponde a uma pessoa cadastrada\n", pes);
            return;
        }
        if (buscarTipoPet(b->tipos, tip) == NULL) {
            printf("ERRO: codigo_tipo=%d nao corresponde a um tipo de pet cadastrado\n", tip);
            return;
        }
        ok = inserirPet(&b->pets, cod, pes, obterStr(c, "nome"), tip);
        break;
    }

    if (ok)
        printf("OK: registro inserido em %s (codigo=%d)\n", nome_tabela(c->tabela), cod);
}

static void exec_update(Banco *b, const ComandoSQL *c) {
    const Atrib *a;
    int afetados = 0, temCod = 0, novoCod = 0;
    int pes = -1, tip = -1;     /* -1 = campo nao alterado */
    const char *nome, *fone, *data, *end, *desc;

    /* regras de integridade, antes de alterar qualquer registro */
    a = achaAtrib(c, "codigo");
    if (a != NULL) {
        if (c->tabela != TAB_PET) {
            printf("ERRO: o codigo de %s nao pode ser alterado\n", nome_tabela(c->tabela));
            return;
        }
        temCod = 1;
        novoCod = a->ival;
    }

    if (c->tabela == TAB_PET) {
        if (obterInt(c, "codigo_pes", &pes) && buscarPessoa(b->pessoas, pes) == NULL) {
            printf("ERRO: codigo_pes=%d nao corresponde a uma pessoa cadastrada\n", pes);
            return;
        }
        if (obterInt(c, "codigo_tipo", &tip) && buscarTipoPet(b->tipos, tip) == NULL) {
            printf("ERRO: codigo_tipo=%d nao corresponde a um tipo de pet cadastrado\n", tip);
            return;
        }
    }

    nome = obterStr(c, "nome");
    fone = obterStr(c, "fone");
    data = obterStr(c, "data_nascimento");
    end = obterStr(c, "endereco");
    desc = obterStr(c, "descricao");

    switch (c->tabela) {
    case TAB_PESSOA: {
        Pessoa *p;
        for (p = b->pessoas; p != NULL; p = p->prox) {
            if (!casaPessoa(p, c)) continue;
            if (atualizarPessoa(b->pessoas, p->codigo, nome, fone, data, end))
                afetados++;
        }
        break;
    }

    case TAB_TIPO_PET: {
        TipoPet *t;
        for (t = b->tipos; t != NULL; t = t->prox) {
            if (!casaTipo(t, c)) continue;
            if (atualizarTipoPet(b->tipos, t->codigo, desc))
                afetados++;
        }
        break;
    }

    case TAB_PET: {
        Pet *p, *outro;
        if (temCod) {
            int casam = 0;
            for (p = b->pets; p != NULL; p = p->prox)
                if (casaPet(p, c)) casam++;
            if (casam > 1) {
                printf("ERRO: nao e possivel dar o mesmo codigo a %d pets\n", casam);
                return;
            }
            outro = buscarPet(b->pets, novoCod);
            if (outro != NULL && !casaPet(outro, c)) {
                printf("ERRO: ja existe pet com codigo %d\n", novoCod);
                return;
            }
        }
        for (p = b->pets; p != NULL; p = p->prox) {
            if (!casaPet(p, c)) continue;
            if (atualizarPet(b->pets, p->codigo, pes, nome, tip)) {
                if (temCod) p->codigo = novoCod;
                afetados++;
            }
        }
        break;
    }
    }

    printf("OK: %d registro(s) alterado(s) em %s\n", afetados, nome_tabela(c->tabela));
}

static void exec_delete(Banco *b, const ComandoSQL *c) {
    int afetados = 0;

    switch (c->tabela) {
    case TAB_PESSOA: {
        Pessoa *p = b->pessoas, *prox;
        while (p != NULL) {
            prox = p->prox;
            if (casaPessoa(p, c)) {
                int cod = p->codigo;
                if (pessoaTemPets(b->pets, cod))
                    printf("ERRO: pessoa %d possui pets; remova-os antes (registro mantido)\n", cod);
                else if (removerPessoa(&b->pessoas, cod))
                    afetados++;
            }
            p = prox;
        }
        break;
    }

    case TAB_TIPO_PET: {
        TipoPet *t = b->tipos, *prox;
        while (t != NULL) {
            prox = t->prox;
            if (casaTipo(t, c)) {
                int cod = t->codigo;
                if (tipoEmUso(b->pets, cod))
                    printf("ERRO: tipo %d esta em uso por pets (registro mantido)\n", cod);
                else if (removerTipoPet(&b->tipos, cod))
                    afetados++;
            }
            t = prox;
        }
        break;
    }

    case TAB_PET: {
        Pet *p = b->pets, *prox;
        while (p != NULL) {
            prox = p->prox;
            if (casaPet(p, c) && removerPet(&b->pets, p->codigo))
                afetados++;
            p = prox;
        }
        break;
    }
    }

    printf("OK: %d registro(s) removido(s) de %s\n", afetados, nome_tabela(c->tabela));
}

/* Sem order by: imprime na hora. Com order by: guarda na arvore (sai ordenado). */
static NoArvore *acumula(NoArvore *raiz, int ordenar, const char *chave, const char *linha) {
    if (ordenar) return arvore_inserir(raiz, chave, linha);
    printf("  %s\n", linha);
    return raiz;
}

static void exec_select(const Banco *b, const ComandoSQL *c) {
    NoArvore *raiz = NULL;
    char linha[MAX_LINHA];
    int n = 0;

    switch (c->tabela) {
    case TAB_PESSOA: {
        const Pessoa *p;
        for (p = b->pessoas; p != NULL; p = p->prox) {
            if (!casaPessoa(p, c)) continue;
            formataPessoa(p, linha, sizeof linha);
            raiz = acumula(raiz, c->tem_order, p->nome, linha);
            n++;
        }
        break;
    }

    case TAB_TIPO_PET: {
        const TipoPet *t;
        for (t = b->tipos; t != NULL; t = t->prox) {
            if (!casaTipo(t, c)) continue;
            formataTipo(t, linha, sizeof linha);
            raiz = acumula(raiz, c->tem_order, t->descricao, linha);
            n++;
        }
        break;
    }

    case TAB_PET: {
        const Pet *p;
        for (p = b->pets; p != NULL; p = p->prox) {
            if (!casaPet(p, c)) continue;
            formataPet(p, linha, sizeof linha);
            raiz = acumula(raiz, c->tem_order, p->nome, linha);
            n++;
        }
        break;
    }
    }

    if (c->tem_order) {
        printf("(ordenado por %s, usando arvore binaria)\n", campo_ordenacao(c->tabela));
        arvore_imprimir(raiz);
        arvore_liberar(raiz);
    }
    printf("OK: %d registro(s) encontrado(s) em %s\n", n, nome_tabela(c->tabela));
}

static void executar(Banco *b, const Comando *no) {
    ComandoSQL c;
    char erro[256];

    printf("\n[#%d] %s\n", no->seq + 1, no->descricao + strspn(no->descricao, " \t\r\n"));
    if (!validar_comando(no->descricao, &c, erro, sizeof erro)) {
        printf("ERRO: %s\n", erro);   /* nao deveria ocorrer: ja validado */
        return;
    }
    switch (c.tipo) {
    case TIPO_INSERT: exec_insert(b, &c); break;
    case TIPO_UPDATE: exec_update(b, &c); break;
    case TIPO_DELETE: exec_delete(b, &c); break;
    case TIPO_SELECT: exec_select(b, &c); break;
    }
}

void processar_filas(Banco *b, Fila *fpessoa, Fila *ftipo, Fila *fpet) {
    Fila *f;
    Comando *no;

    while ((f = fila_mais_antiga(fpessoa, ftipo, fpet)) != NULL) {
        no = fila_desenfileirar(f);
        executar(b, no);
        free(no);
    }
}
