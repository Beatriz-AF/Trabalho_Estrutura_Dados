
#ifndef PESSOA_H
//verifica se pessoa ainda nao foi definido, se nao, define a estrutura da pessoa e declara as funcoes que manipulam a lista de pessoas
#define PESSOA_H
//define a estrutura da pessoa para impedir inclusoes repetidas

//define a estrutura da pessoa e declara as funcoes que manipulam a lista de pessoas

typedef struct pessoa {
    int codigo;
    char nome[100];
    char fone[20];
    char data_nascimento[11];
    char endereco[255];

    struct pessoa *ant;
    struct pessoa *prox;
} Pessoa;

// Funcoes da lista de pessoas
Pessoa *buscarPessoa(Pessoa *inicio, int codigo);
int inserirPessoa(Pessoa **inicio, int codigo,
                  const char *nome, const char *fone,
                  const char *data, const char *endereco);
void listarPessoas(Pessoa *inicio);
void liberarPessoas(Pessoa **inicio);

int atualizarPessoa(Pessoa *inicio, int codigo,
                    const char *nome, const char *fone,
                    const char *data, const char *endereco);

int removerPessoa(Pessoa **inicio, int codigo);

#endif
