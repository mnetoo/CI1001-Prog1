/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/


#include <stdio.h>
#include <stdlib.h>
#include "racional.h"



/* Função que ordena vetor de ponteiros em ordem crescente */
void ordena_racional(struct racional *vetor[], int contador_valido) 
{
    for (int i = 0; i < contador_valido - 1; i++) 
    {
        for (int j = 0; j < contador_valido - 1 - i; j++) 
        {
            // Usando a função compara_r para comparar os racionais
            if (compara_r(vetor[j], vetor[j + 1]) > 0) 
            {
                // Fazendo a troca dos ponteiros se o anterior for maior que o próximo
                struct racional *temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
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

