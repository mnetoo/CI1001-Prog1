/*

    Este arquivo contém o main com a implementação do trabalho para definir e manipular números racionais.
    Utiliza as funções do arquivo "racional.c" que contém a implementação das funções que foram prototipadas
    em um arquivo header "racional.h"

*/

#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

// Função para trocar dois ponteiros de racionais
void troca(struct racional **a, struct racional **b)
{
    struct racional *temp = *a;
    *a = *b;
    *b = temp;
}

// Função que ordena vetor de ponteiros de racionais usando Selection Sort
void ordena_racional(struct racional *vetor[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (compara_r(vetor[j], vetor[min]) < 0)
                min = j;

        // Troca o menor elemento encontrado com o primeiro elemento do subvetor não ordenado
        if (min != i)
            troca(&vetor[i], &vetor[min]);
    }
}

/* PROGRAMA PRINCIPAL */
int main()
{
    int n;

    // Leitura do valor n até estar no intervalo [1, 99]
    do
    {
        scanf("%d", &n);
    } while (n <= 0 || n >= 100);

    // Alocação do vetor de ponteiros para racionais
    struct racional **vetor = malloc(n * sizeof(struct racional *));
    if (vetor == NULL)
    {
        printf("Erro ao alocar memória.\n");
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
    printf("VETOR =");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf(" ");
    printf("\n");

    // Elimina os racionais inválidos
    int novo_tamanho = n;
    for (int i = 0; i < novo_tamanho; i++) 
    {
        if (!valido_r(vetor[i])) 
        {
            destroi_r(vetor[i]); // Libera a memória do racional inválido
            vetor[i] = vetor[novo_tamanho - 1]; // Substitui o inválido pelo último elemento válido
            novo_tamanho--; // Reduz o tamanho do vetor
            i--; // Reavalia a posição atual após a substituição
        }
    }
    n = novo_tamanho;

    // Imprimindo o vetor resultante
    printf("VETOR =");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf(" ");
    printf("\n");

    // Ordena vetor de racionais
    ordena_racional(vetor, n);

    // Imprimindo o vetor ordenado
    printf("VETOR =");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf(" ");
    printf("\n");

    // Calculando a soma
    struct racional *soma = cria_r(0, 1); // Inicializa soma como 0
    for (int i = 0; i < n; i++)
        soma_r(soma, vetor[i], soma);

    // Imprimindo a soma
    printf("SOMA =");
    imprime_r(soma);
    printf("\n");

    // Liberando a memória dos racionais no vetor
    for (int i = 0; i < n; i++)
    {
        destroi_r(vetor[i]);
        vetor[i] = NULL; // Define o ponteiro como NULL após liberar
    }

    // Imprimindo o vetor com os ponteiros já nulos
    printf("VETOR =");
    for (int i = 0; i < n; i++)
    {
        if (vetor[i] == NULL)
            printf(" NULL"); // Imprime "NULL" se o ponteiro foi liberado
        else
            imprime_r(vetor[i]); // Imprime o racional se o ponteiro não é NULL
    }
    printf(" ");
    printf("\n");

    // Liberando o vetor de ponteiros
    free(vetor);
    vetor = NULL;

    // Liberando a soma
    destroi_r(soma);
    soma = NULL;

    return 0;
}
