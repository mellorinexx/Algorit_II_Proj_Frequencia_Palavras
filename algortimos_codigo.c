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
    int caractere;
    int i = 0;

    /* Pula qualquer caractere que nao seja letra (espacos, numeros, pontuacoes) */
    while ((caractere = fgetc(arquivo)) != EOF && !isalpha(caractere)) {
        /* Apenas avanca pelo ficheiro */
    }

    if (caractere == EOF) {
        return 0; /* Fim do ficheiro */
    }

    /* Le as letras ate encontrar um espaco ou pontuacao (usando apenas while) */
    while (caractere != EOF && isalpha(caractere)) {
        palavra_lida[i] = tolower(caractere);
        i++;
        caractere = fgetc(arquivo); 
       
    }
    
    palavra_lida[i] = '\0'; /* Finaliza a string */
    return 1;
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
/* Retorna o indice da palavra se encontrada, ou -1 se nao existir */
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
        /* A palavra ja existe, apenas incrementa a frequencia */
        dicionario[indice].frequencia++;
    } else {
        /* A palavra nao existe, precisamos inserir na posicao correta (ordem alfabetica) */
        i = *qtd_dicionario - 1;

        /* Desloca as palavras "maiores" uma posicao para a direita para abrir espaco */
        while (i >= 0 && strcmp(dicionario[i].texto, nova_palavra) > 0) {
            dicionario[i + 1] = dicionario[i];
            i--;
        }

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
