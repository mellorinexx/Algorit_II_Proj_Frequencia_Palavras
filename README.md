# Algorit_II_Proj_Frequencia_Palavras

## Explicação das Sintaxes

### Struct (Linha 10)
Uma palavra-chave (keyword) da linguagem C. Serve para **criar uma estrutura de dados**.
Para entender o que ela faz, imagine uma ficha de registo em papel. Numa ficha de um cliente, por exemplo, tem de preencher o Nome (texto), a Idade (número inteiro) e a Sexo (char). 
A struct agrupa variáveis de tipos diferentes num único "pacote".

```c
struct Palavra {
    char texto[50];
    int frequencia;
};
```

Na prática, o código ensina ao compilador C que, naquele momento, **existe um novo tipo de dado chamado struct Palavra**. Sempre que criar uma variável com este tipo, ela vai ter dentro dela:

1. Uma espaço que armazena as letras da palavra (texto).

2. Um espaço que armazena o número de vezes que ela aparece (frequencia).

#### Porque é que isto é útil?
Se não existisse o comando struct e precisássemos 'fazer na mão'. Para guardar 1000 palavras e as suas respetivas frequências, teria de criar dois arrays separados:

```c
char lista_textos[1000][50];

int lista_frequencias[1000];
```

No projeto, é solicitado que seja em ordem alfabética, sem o **struct** seria BEMM mais difício. Teria de mover uma palavra no vetor lista_textos e lembrar-se de mover também o número exato no vetor lista_frequencias para a mesma posição, para que não ficassem misturados.

Com a struct, nós simplesmente criamos um único vetor (facilita MUITO):

```c
struct Palavra dicionario[1000];
```
Assim, a palavra e a sua frequência ficam juntinhas <3

**Se precisar mudar a posição da palavra no dicionário para a ordenar, a frequência vai junto e elas NUNCA vão se desgrudar!!** porque fazem parte do mesmo pacote (da mesma estrutura).

OBs: para aceder àquilo que está dentro do pacote, usamos um ponto (.). 
Por exemplo:
```c
dicionario[0].frequencia = 1;
```

significa "vá à primeira posição do dicionário e coloque o valor 1 apenas na gaveta da frequência".

### Int caracter (Linha 18)
#### Por que usamos int e não char para guardar uma letra? 
Porque a função fgetc (que lê do ficheiro) pode retornar o valor EOF (End of File / Fim do Ficheiro), que no C vale -1.
O tipo char nem sempre consegue armazenar números negativos adequadamente em todos os computadores, então usamos int por segurança ;)

#### Massss, parece estranho guardar uma letra dentro de uma variável feita para números inteiros (int), não é?**

Na real, para o computador, não existem letras, apenas números (Tabela ASCII).

**Em C, o tipo char não é um tipo exclusivo para texto**. Ele é, na verdade, um número inteiro muito pequeno (ocupa apenas 1 byte na memória).
O tipo int é exatamente a mesma coisa, mas é uma "caixa maior" (ocupa geralmente 4 bytes na memória).

Logo, é usado o int (uma caixa grande) para o caractere que será lido porque ele tem espaço suficiente para guardar as letras (convertidas através da tabela ASCII) e também tem espaço para guardar o número -1 (o famoso EOF: Fim de Ficheiro). Se usássemos o char (uma little caixa), dependendo do sistema operativo, o número -1 poderia ser confundido com um caractere especial ou uma letra com acento (como o 'ÿ').

### isalpha (Linha 22)
A função isalpha() (da biblioteca ctype.h) testa se o caractere é uma letra de A a Z.
```c
 while (!isalpha(caractere)) {
    printf("NÃO É UMA LETRA DE A a Z ");
}

```

### tolower (Linha 32)

Passa para minusculo: Pega na letra (ex: 'A') e transforma em minúscula (ex: 'a'). Isso garante que "The" e "the" não sejam tratadas como duas palavras diferentes no seu dicionário.

```c
palavra_lida = tolower(caractere);
```

### char stop_words[][TAM_MAX_PALAVRA]

Em C, não existe um tipo "lista de textos" nativo. O que temos aqui é uma matriz de caracteres (um vetor de 2 dimensões). Imagine uma tabela de Excel: cada linha é uma palavra diferente, e cada coluna guarda uma letra dessa palavra (até ao máximo de 50 letras, definido por TAM_MAX_PALAVRA).

(C FAZENDO A GENTE SOFRERRRR)


### strcmp (String Compare)

Em C, você não pode usar == para comparar textos.
´´´c
if (texto1 == texto2)
´´´
Se fizer isso, o C vai comparar os "endereços de memória" onde eles estão guardados, e não as letras (ACABAAAA PELO AMOR DE DEEUS). Nesse sentido, usamos a função strcmp (String Compare) da biblioteca <string.h>.

- Ela lê as duas palavras letra por letra.

- devolve 0: se as palavras forem idênticas.

- devolve > 0: se a palavra do meio for maior (ou seja, vem depois no alfabeto) que o alvo.

- devolve < 0: se a palavra do meio for menor (vem antes no alfabeto) que o alvo.

- Por isso fazemos a pergunta: if (strcmp(...) == 0). Se a resposta for zero, significa que encontramos um IMITADORA!

### *qtd_dicionario (Linha 79)
**Porquê o asterisco ?**

Porque esta função vai precisar de alterar a quantidade total de palavras que existe lá no main. Se passássemos apenas int qtd, a função faria uma cópia do número, aumentaria a cópia, e o main nunca ficaria a saber. Ao usar o ponteiro, estamos a dizer: "Vá ao endereço de memória da variável original e altere o valor lá".

