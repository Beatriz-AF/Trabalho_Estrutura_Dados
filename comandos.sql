-- Cadastro dos tipos de pet

INSERT INTO tipo_pet(codigo, descricao) VALUES(1, 'Cachorro');
INSERT INTO tipo_pet(codigo, descricao) VALUES(2, 'Gato');

-- Cadastro das pessoas

INSERT INTO pessoa(codigo, nome, fone, data_nascimento) VALUES(1, 'Joao', '859995566', '15/05/2000');

INSERT INTO pessoa(codigo, nome, fone, data_nascimento, endereco) VALUES(2, 'Maria', '859996677', '20/08/2001', 'Rua B, nr 202');

INSERT INTO pessoa(codigo, nome, fone, data_nascimento) VALUES(3, 'Jose', '859997788', '04/10/2001');

-- Cadastro dos pets

INSERT INTO pet(codigo, codigo_pes, nome, codigo_tipo) VALUES(1, 1, 'Bilu', 2);
INSERT INTO pet(codigo, codigo_pes, nome, codigo_tipo) VALUES(2, 1, 'Brother', 1);
INSERT INTO pet(codigo, codigo_pes, nome, codigo_tipo) VALUES(3, 2, 'Minie', 1);

-- Atualizacao de registros

UPDATE pessoa SET fone = '85999999999' WHERE codigo = 1;
UPDATE pet SET nome = 'Billy' WHERE codigo = 1;
UPDATE tipo_pet SET descricao = 'Felino' WHERE codigo = 2;

-- Consulta de um registro especifico

SELECT * FROM pet WHERE codigo = 3;

-- Exclusao de registro

DELETE FROM pet WHERE codigo = 3;

-- Consultas ordenadas

SELECT * FROM pessoa ORDER BY nome;
SELECT * FROM tipo_pet ORDER BY descricao;
SELECT * FROM pet ORDER BY nome;