
--Teste de leitura e execucao SQL

INSERT INTO pessoa VALUES (1, 'Maria', '85999999999', '10/05/2000', 'Rua A');
INSERT INTO pessoa VALUES (2, 'Joao', '85988888888', '15/08/2001', 'Rua B');

INSERT INTO tipo_pet VALUES (1, 'Cachorro');
INSERT INTO tipo_pet VALUES (2, 'Gato');

INSERT INTO pet VALUES (10, 1, 'Bob', 1);
INSERT INTO pet VALUES (11, 2, 'Luna', 2);

SELECT * FROM pessoa ORDER BY nome;
SELECT * FROM pet ORDER BY nome;

UPDATE pet SET codigo_pes = 1, nome = 'Rex', codigo_tipo = 1 WHERE codigo = 10;

SELECT * FROM pet WHERE codigo = 10;

DELETE FROM pet WHERE codigo = 11;

SELECT * FROM pet;
