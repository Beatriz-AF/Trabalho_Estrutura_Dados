#ifndef LEITOR_H
#define LEITOR_H

#include "fila.h"

/* Le o script de um arquivo e enfileira os comandos.
   Retorna 1 em sucesso, 0 em falha (a mensagem de erro ja e exibida). */
int ler_script_arquivo(const char *nome, Fila *f);

/* Le os comandos digitados pelo usuario ate o fim da entrada
   (Ctrl+D no Linux/Mac, Ctrl+Z e Enter no Windows).
   Retorna 1 em sucesso, 0 em falha. */
int ler_script_usuario(Fila *f);

#endif
