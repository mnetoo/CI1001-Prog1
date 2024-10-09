/* 

    Este arquivo contém o main com a implementação do trabalho para definir e manipular números racionais.
    Utiliza as funções do arquivo "racional.c" que contém a implementação das funções que foram prototipadas
    em um arquivo header "racional.h"

*/


#include <stdio.h>
#include <stdlib.h>
#include "racional.h"



// Função para trocar dois ponteiros de racionais
void troca(struct racional **a, struct racional **b) {
    struct racional *temp = *a;
    *a = *b;
    *b = temp;
}

// Função para particionar o vetor
int particiona_racional(struct racional *vetor[], int esquerda, int direita) {
    struct racional *pivo = vetor[direita]; // Escolhe o último elemento como pivô
    int i = (esquerda - 1);  // Índice do menor elemento

    for (int j = esquerda; j < direita; j++) {
        // Usando a função compara_r para comparar os racionais
        if (compara_r(vetor[j], pivo) <= 0) {
            i++;  // Incrementa o índice do menor elemento
            troca(&vetor[i], &vetor[j]);
        }
    }
    troca(&vetor[i + 1], &vetor[direita]);
    return (i + 1);  // Retorna o índice de particionamento
}

// Função recursiva de Quick Sort
void quicksort_racional(struct racional *vetor[], int esquerda, int direita) {
    if (esquerda < direita) {
        // Particiona o vetor e encontra o índice do pivô
        int pivo = particiona_racional(vetor, esquerda, direita);

        // Ordena as duas metades recursivamente
        quicksort_racional(vetor, esquerda, pivo - 1);
        quicksort_racional(vetor, pivo + 1, direita);
    }
}

// Função que ordena vetor de ponteiros de racionais usando Quick Sort
void ordena_racional(struct racional *vetor[], int contador_valido) {
    quicksort_racional(vetor, 0, contador_valido - 1);
}


int main ()
{
    int n;

    // Leitura do valor n até estar no intervalo [1, 99]
    do {
        scanf("%d", &n);
    } while (n <= 0 || n >= 100);


    // Alocação do vetor de ponteiros para racionais
    struct racional **vetor = malloc(n * sizeof(struct racional *));
    if (vetor == NULL) 
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }



    // Carregando vetor com números racionais
    for (int i = 0; i < n; i++) 
    {
        long num, den;
        scanf("%ld %ld", &num, &den);
        vetor[i] = cria_r(num, den);
    }

    // Imprimindo o vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++) 
        imprime_r(vetor[i]);
    printf("\n");

    // Eliminando racionais inválidos
    int contador_valido = 0;
    for (int i = 0; i < n; i++) 
    {
        if (valido_r(vetor[i])) 
            vetor[contador_valido++] = vetor[i];
        else
            destroi_r(vetor[i]);  // Libera memória dos racionais inválidos
    }

    // Imprimindo o vetor resultante
    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++)
        imprime_r(vetor[i]);
    printf("\n");



    //Ordena vetor de racionais
    ordena_racional(vetor, contador_valido);

    // Imprimindo o vetor ordenado
    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++)
        imprime_r(vetor[i]);
    printf("\n");



    // Calculando a soma
    struct racional *soma = cria_r(0, 1); // Inicializa soma como 0
    for (int i = 0; i < contador_valido; i++) 
        soma_r(soma, vetor[i], soma);

    // Imprimindo a soma
    printf("SOMA = ");
    imprime_r(soma);
    printf("\n");



    // Liberando a memória dos racionais no vetor
    for (int i = 0; i < contador_valido; i++) 
    {
        destroi_r(vetor[i]);
        vetor[i] = NULL; // Define o ponteiro como NULL após liberar
    }

    // Imprimindo o vetor com os ponteiros já nulos
    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++) 
    {
        if (vetor[i] == NULL) 
            printf("NULL "); // Imprime "NULL" se o ponteiro foi liberado
        else 
            imprime_r(vetor[i]); // Imprime o racional se o ponteiro não é NULL
    }
    printf("\n");

    // Liberando o vetor de ponteiros
    free(vetor);
    vetor = NULL; // Evita ponteiros "pendurados"

    // Liberando a soma
    destroi_r(soma);
    soma = NULL; // Define o ponteiro como NULL após liberar

    return 0;
}
