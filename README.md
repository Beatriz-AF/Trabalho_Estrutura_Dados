# Trabalho 2 – 2ª etapa: fila de comandos e listas de pessoa, tipo_pet e pet

Integração do código de cadastro de pessoa e tipo_pet (da Beatriz, `Trabalho_Estrutura_Dados`)
com a parte de comandos (fila, leitura de script, validação, processamento) e com o módulo de pet.

## Estrutura de pastas

```
projeto_integrado/
├── main.c              programa principal
├── Makefile
├── .gitignore
├── comandos/           parte de COMANDOS  (guia em comandos/LEIA-ME.md)
│   ├── fila.c/.h           fila geral de comandos
│   ├── leitor.c/.h         leitura do script (arquivo ou usuario)
│   ├── validador.c/.h      validacao de INSERT, UPDATE, DELETE, SELECT
│   ├── processador.c/.h    filas por tabela e execucao chamando o cadastro
│   ├── arvore.c/.h         arvore binaria do select ... order by nome
│   └── test_validador.c    teste isolado da validacao
├── cadastro/           parte de CADASTRO
│   ├── pessoa.c/.h         codigo da Beatriz (sem alteracoes)
│   ├── tipo_pet.c/.h       codigo da Beatriz (sem alteracoes)
│   ├── pet.c/.h            pet, no mesmo estilo
│   └── persistencia.c/.h   salvar e carregar pessoas, tipos e pets
├── script_exemplo.txt
└── script_teste_invalidos.txt
```

## Compilacao

Linux / Mac:
```
make            # gera ./programa
make test       # valida 30 casos de comandos (sem o cadastro)
make clean
```

Windows (MinGW), a partir da pasta raiz:
```
gcc -std=c99 -Wall -Icomandos -Icadastro -o programa.exe main.c comandos/fila.c comandos/leitor.c comandos/validador.c comandos/processador.c comandos/arvore.c cadastro/pessoa.c cadastro/tipo_pet.c cadastro/pet.c cadastro/persistencia.c
```

## Execucao

```
./programa script_exemplo.txt        # script por argumento
./programa                           # menu: 1 = arquivo, 2 = digitar comandos
```

Os dados ficam em `pessoas.txt`, `tipos_pet.txt` e `pets.txt` (pasta de execucao).
Sao lidos ao iniciar e regravados ao final, na mesma ordem.

## Regras implementadas

- Codigo de pessoa, tipo_pet e pet nao pode se repetir (verificado pelo cadastro e pelo processador).
- `codigo_pes` do pet deve ser uma pessoa cadastrada; `codigo_tipo` deve ser um tipo cadastrado.
- Pessoa com pets nao pode ser removida (o registro e mantido e o erro e mostrado).
- Tipo de pet em uso por pets nao pode ser removido (mesma estrategia).
- Codigo de pessoa e de tipo_pet nao pode ser alterado (evita quebrar as referencias).
- Alteracao de pet nao aceita `codigo_pes` ou `codigo_tipo` inexistentes.
- Data de nascimento e obrigatoria (dd/mm/aaaa). Essa regra vem do cadastro da Beatriz.
- `update` e `delete` exigem `where`. `order by` so aceita o campo nome (em tipo_pet, `descricao`).

## Observacoes

- Os campos de texto do cadastro da Beatriz tem tamanho fixo (nome 100, fone 20, endereco 255).
  Textos maiores sao truncados.
- O exemplo do enunciado usa `codigo_cli` no insert de pet; o programa aceita como apelido de `codigo_pes`.
- No enunciado, `josé` aparece sem aspas; o `script_exemplo.txt` traz `'josé'`.
- Os inserts de `joão` e `maria` do exemplo nao tem data; o script ja inclui as datas, porque o cadastro exige.
