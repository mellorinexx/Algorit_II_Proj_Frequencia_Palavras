#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 1000
#define TAM_MAX_PALAVRA 50

/* Estrutura para armazenar a palavra e a sua frequencia */
struct Palavra {
    char texto[TAM_MAX_PALAVRA];
    int frequencia;
};

/* Funcao para ler a proxima palavra valida de um ficheiro (ignora pontuacoes e espacos) */
/* Retorna 1 se leu uma palavra, 0 se chegou ao fim do ficheiro */
int ler_proxima_palavra(FILE *arquivo, char *palavra_lida) {

    /*
    FILE *arquivo: pega o CONTEÚDO de arquivo armazenado na memória
    char *palavra_lida: É onde a função vai "escrever" a palavra que acabou de ler, devolvendo-a para o main.
    */

    int caractere;
    int i = 0; /* contador que descreve qual "casinha" do vetor palavra_lida devemos colocar a próxima letra.*/

    /* Pula qualquer caractere que nao seja letra (espacos, numeros, pontuacoes) */
    while ((caractere = fgetc(arquivo)) != EOF && !isalpha(caractere)) {

        /* se o caracter lido no ficheiro não for o final(EOF) e NÃO(!) for uma letra entre A e Z */

        /* O laço fica rodando e "comendo" espaços e pontuações 
        até esbarrar na primeira letra válida de uma palavra (ou até o arquivo acabar).*/
    }

    if (caractere == EOF) {
        return 0; /* Fim do ficheiro */
    }

    /* Le as letras ate encontrar um espaco ou pontuacao (usando apenas while) */
    /*Enquanto não for o fim do arquivo E FOR uma letra*/
    while (caractere != EOF && isalpha(caractere)) {
        palavra_lida[i] = tolower(caractere);
        i++;
        caractere = fgetc(arquivo); /* Lê o próximo caractere do ficheiro para que o while possa testá-lo 
        na próxima volta. Se for um espaço ou pontuação, o laço para de rodar, 
        significando que a palavra acabou. */
    }
    
    palavra_lida[i] = '\0'; /* Finaliza a string */
    return 1;  /*Avisa o programa principal que a missão foi cumprida 
    com sucesso e que a palavra guardada em palavra_lida está pronta para ser processada!*/
}

/* Verifica se uma palavra esta na lista de stop words (Busca Sequencial Simples) */
int eh_stop_word(char stop_words[][TAM_MAX_PALAVRA], int qtd_stop_words, char *palavra) {
    int i;
    for (i = 0; i < qtd_stop_words; i++) {
        if (strcmp(stop_words[i], palavra) == 0) {
            return 1; /* E stop word */
        }
    }
    return 0; /* Nao e stop word */
}

/* OBRIGATORIO: Busca Binaria Recursiva */
/* A função vai devolver um número: o índice (a posição) onde a palavra foi encontrada no vetor. Se a palavra não existir, ela devolve -1. */
int busca_binaria_recursiva(struct Palavra dicionario[], int inicio, int fim, char *alvo) {
    /* No C90, as variaveis tem de ser declaradas no topo da funcao */
    int meio;
    int comparacao;

    if (inicio > fim) {
        return -1; /* Condicao de paragem: nao encontrou */
    }

    meio = inicio + (fim - inicio) / 2;
    comparacao = strcmp(dicionario[meio].texto, alvo);

    if (comparacao == 0) {
        return meio; /* Encontrou a palavra */
    } else if (comparacao > 0) {
        /* A palavra alvo vem antes do meio no alfabeto */
        return busca_binaria_recursiva(dicionario, inicio, meio - 1, alvo);
    } else {
        /* A palavra alvo vem depois do meio no alfabeto */
        return busca_binaria_recursiva(dicionario, meio + 1, fim, alvo);
    }
}

/* OBRIGATORIO: Insercao Ordenada (Shift array) */
void inserir_ordenado(struct Palavra dicionario[], int *qtd_dicionario, char *nova_palavra) {
    /* Variaveis no topo */
    int i;
    int indice;

    /* 1. Verifica se a palavra ja existe usando a Busca Binaria Recursiva */
    indice = busca_binaria_recursiva(dicionario, 0, *qtd_dicionario - 1, nova_palavra);

    if (indice != -1) {
        dicionario[indice].frequencia++;
        /*Se a palavra já existir, a busca devolve a posição exata dela (o indice). 
        O if é ativado e nós simplesmente vamos a essa posição e somamos +1 na frequencia.
        Não precisamos de mexer em mais nada.*/

    } else {
        /* A palavra nao existe, precisamos inserir na posicao correta (ordem alfabetica) */

        i = *qtd_dicionario - 1; /* olhamos para a última palavra do dicionário.*/
        /* Se a busca devolver -1, significa que a palavra é nova. Agora entra o else.
        Imagine que tem uma prateleira de livros organizados alfabeticamente e quer inserir um livro novo. 
        O que você faz? Começa pelo final da prateleira, empurrando os livros que vêm depois do seu 
        um espaço para a direita, até abrir um "buraco" no local exato onde o seu livro deve entrar. 
        É exatamente isto que o código faz.*/

        /* Desloca as palavras "maiores" uma posicao para a direita para abrir espaco */

        /* lógica de deslocamento: Insertion Sort*/
        while (i >= 0 && strcmp(dicionario[i].texto, nova_palavra) > 0) {
            dicionario[i + 1] = dicionario[i];  /*Pega na palavra que está na posição atual (i) e copia-a para a "casinha" imediatamente à direita (i + 1).*/
            i--;
        }
        /*Significa: "A palavra que estou a olhar agora (dicionario[i]) 
        vem DEPOIS da nova_palavra no alfabeto?" * Se a resposta for sim (maior que 0), 
        significa que ela está no caminho. Tem de ser empurrada.

        Se for não (menor que 0), significa que achámos o lugar!*/

        /* Insere a nova palavra no espaco que foi aberto */
        strcpy(dicionario[i + 1].texto, nova_palavra);
        dicionario[i + 1].frequencia = 1;
        (*qtd_dicionario)++; /* Aumenta o tamanho do dicionario */
    }
}

int main(int argc, char *argv[]) {
    /* No C90, ABSOLUTAMENTE TODAS as variaveis do main devem ficar aqui no topo */
    char stop_words[MAX_PALAVRAS][TAM_MAX_PALAVRA];
    int qtd_stop_words = 0;
    char buffer_palavra[TAM_MAX_PALAVRA];
    FILE *arq_stopwords;
    struct Palavra dicionario[MAX_PALAVRAS];
    int qtd_dicionario = 0;
    FILE *arq_texto;
    int i;

    /* Verifica se os argumentos da linha de comandos foram passados corretamente */
    if (argc != 3) {
        printf("Uso correto: ./a.out <arquivo_stopwords> <arquivo_texto>\n");
        return 1;
    }

    /* 1. LER O FICHEIRO DE STOP WORDS */
    arq_stopwords = fopen(argv[1], "r");
    if (arq_stopwords == NULL) {
        printf("Erro ao abrir o ficheiro de stop words: %s\n", argv[1]);
        return 1;
    }

    while (ler_proxima_palavra(arq_stopwords, buffer_palavra)) {
        strcpy(stop_words[qtd_stop_words], buffer_palavra);
        qtd_stop_words++;
    }
    fclose(arq_stopwords);

    /* 2. LER O FICHEIRO DE TEXTO E MONTAR O DICIONARIO */
    arq_texto = fopen(argv[2], "r");
    if (arq_texto == NULL) {
        printf("Erro ao abrir o ficheiro de texto: %s\n", argv[2]);
        return 1;
    }

    while (ler_proxima_palavra(arq_texto, buffer_palavra)) {
        /* Ignora a palavra se ela for uma stop word */
        if (!eh_stop_word(stop_words, qtd_stop_words, buffer_palavra)) {
            /* Insere de forma ordenada (ou atualiza a frequencia) */
            inserir_ordenado(dicionario, &qtd_dicionario, buffer_palavra);
        }
    }
    fclose(arq_texto);

    /* 3. IMPRIMIR OS RESULTADOS */
    for (i = 0; i < qtd_dicionario; i++) {
        printf("%s, %d\n", dicionario[i].texto, dicionario[i].frequencia);
    }
    printf("total de palavras diferentes no dicionario = %d\n", qtd_dicionario);

    return 0;
}