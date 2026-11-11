
#ifndef PESSOA_H
#define PESSOA_H

//definir a estrutura pessoa
typedef struct pessoa { 
    int codigo;
    char nome[100];
    char fone[20];
    char data_nascimento[11];
    char endereco[150];
//armazenei os dados

    struct pessoa *ant;
    struct pessoa *prox;
//lista duplamente encadeada
} Pessoa;

// Funcoes da lista de pessoas
Pessoa *buscarPessoa(Pessoa *inicio, int codigo);
int inserirPessoa(Pessoa **inicio, int codigo,
                  const char *nome, const char *fone,
                  const char *data, const char *endereco);
void listarPessoas(Pessoa *inicio);
void liberarPessoas(Pessoa **inicio);
//digo quais funcoes vao ser usadas
#endif

