# Parte de comandos

Esta pasta contem a parte de comandos: fila, leitura, validacao e processamento.
O processamento chama o cadastro (pasta `../cadastro`) pelos headers `pessoa.h`, `tipo_pet.h`, `pet.h` e `persistencia.h`.

## Arquivos

| Arquivo | Responsabilidade |
|---|---|
| `fila.c/.h` | Fila geral de comandos (`struct Comando` com `descricao`, `seq`, `tabela`). `ler_comandos` separa o texto por `;`, ignorando `;` dentro de aspas. |
| `leitor.c/.h` | `ler_script_arquivo` e `ler_script_usuario`. |
| `validador.c/.h` | `validar_comando` analisa a sintaxe e preenche uma `ComandoSQL` com tipo, tabela, campos, valores e condicao. |
| `processador.c/.h` | `filtrar_validos`, `distribuir` e `processar_filas`. |
| `arvore.c/.h` | Arvore binaria usada no `order by`. |
| `test_validador.c` | Teste isolado da validacao (`make test`). |

## Fluxo

```
script (arquivo ou usuario)       leitor.c  -> ler_comandos
fila geral (texto bruto)          processador.c -> filtrar_validos   (usa validador.c)
fila so com validos               processador.c -> distribuir
filas pessoa | tipo_pet | pet     processador.c -> processar_filas   (ordem original, chama cadastro)
listas em memoria                 main.c -> salvarPessoas, salvarTiposPet, salvarPets
```

## Como usar

### 1. Ler o script
```c
Fila comandos;
fila_criar(&comandos);
ler_script_arquivo("script_exemplo.txt", &comandos);   /* arquivo */
ler_script_usuario(&comandos);                          /* digitado (Ctrl+D ou Ctrl+Z e Enter) */
```

### 2. Validar um comando
```c
ComandoSQL c;
char erro[256];
if (validar_comando("select * from pet order by nome;", &c, erro, sizeof erro)) {
    /* c.tipo = TIPO_SELECT, c.tabela = TAB_PET, c.tem_order = 1 */
} else {
    printf("invalido: %s\n", erro);
}
```
Em `insert`, os campos ficam em `c.atribs[0..c.n-1]`. Cada `Atrib` tem `campo`, `tipo` (`VAL_INT` ou `VAL_STR`), `ival` e `sval`.
Em `update`, `c.atribs` sao as atribuicoes. Em `update`, `delete` e `select` com filtro, a condicao esta em `c.where` (com `c.tem_where = 1`).

### 3. Filtrar, separar e processar
```c
Banco b;   /* b.pessoas, b.tipos, b.pets: ponteiros de inicio, comecam em NULL */
Fila validos, fpessoa, ftipo, fpet;
fila_criar(&validos); fila_criar(&fpessoa);
fila_criar(&ftipo);   fila_criar(&fpet);

filtrar_validos(&comandos, &validos);
distribuir(&validos, &fpessoa, &ftipo, &fpet);
processar_filas(&b, &fpessoa, &ftipo, &fpet);
```

## Como o processador chama o cadastro

| Operacao | Funcao do cadastro |
|---|---|
| Inserir | `inserirPessoa`, `inserirTipoPet`, `inserirPet` |
| Buscar | `buscarPessoa`, `buscarTipoPet`, `buscarPet` |
| Atualizar | `atualizarPessoa`, `atualizarTipoPet`, `atualizarPet` |
| Remover | `removerPessoa`, `removerTipoPet`, `removerPet` |
| Liberar memoria | `liberarPessoas`, `liberarTiposPet`, `liberarPets` |

As regras que dependem de outras listas (dono existente, tipo existente, pessoa com pets, tipo em uso)
ficam em `processador.c`, porque o cadastro de pet nao conhece as outras listas.
O filtro (`where`) e feito pelo processador, comparando os campos de cada registro.

## Como testar

```
make test     # 30 casos de validacao, com a interpretacao de cada comando
```

## Ajustes

- Para aceitar um comando novo, altere `validador.c` (funcoes `parse_*`) e adicione o caso em `test_validador.c`.
- A regra de data obrigatoria vem do cadastro da Beatriz (`inserirPessoa`). A constante `DATA_NASCIMENTO_OBRIGATORIA` em `validador.h` deve permanecer `1`.
