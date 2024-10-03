#include <stdio.h>
#include <stdlib.h>
#include "racional.h"



/* Ordenação usando Bubble Sort */
void ordena_racionais(struct racional vetor[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (compara_r(vetor[j], vetor[j + 1]) > 0) {
                // Troca os elementos
                struct racional temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

int main() 
{
    int n;
    struct racional vetor[100];
    struct racional soma = {0, 1};


    // Lê um valor n tal que 0 < n < 100
    do {
        scanf("%d", &n);
    } while (n <= 0 || n >= 100);









/*-------------------CARREGANDO O VETOR---------------------------------*/

    // Preenche o vetor com n números racionais lidos da entrada
    for (int i = 0; i < n; i++) 
    {
        long num, den;
        scanf("%ld %ld", &num, &den);
        vetor[i] = cria_r(num, den);
    }








/*----------IMPRIMINDO O VETOR E O CONTEÚDO DO VETOR LIDO----------*/

    // Imprime os números racionais do vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf("\n");
    






    // Retira os elementos inválido do vetor
    int i = 0;
    while (i < n) 
    {
        if (!valido_r(vetor[i])) 
        {
            // Desloca os elementos à esquerda para sobrescrever o inválido
            for (int j = i; j < n - 1; j++)
                vetor[j] = vetor[j + 1];

            n--; // Reduz o tamanho do vetor
        } 
        else 
            i++; // Avança para a próxima posição apenas se o atual for válido
    }


    // Imprime os números racionais válidos do vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf("\n");


/*----------IMPRIMINDO O VETOR SEM OS ELEMENTOS INVÁLIDOS----------*/

    // Retira os elementos inválido do vetor
    int i = 0;
    while (i < n) 
    {
        if (!valido_r(vetor[i])) 
        {
            for (int j = i; j < n - 1; j++) /* Desloca os elementos à esquerda para para sobrescrever o inválido */
                vetor[j] = vetor[j + 1];          

            n--; // Diminui o tamanho do vetor
        } 
        else 
            i++; // Se o atual for válido, avança para a próxima posição
    }

    // Imprime os números racionais válidos do vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf("\n");



/*----------ORDENANDO O VETOR E IMPRIMINDO O VETOR ORDENADO----------*/

    // Ordena o vetor de racionais válidos
    ordena_racionais(vetor, n);

    // Imprime o vetor com os números racionais ordenados
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);
    printf("\n");



/*----------CALCULANDO A SOMA DE TODOS OS ELEMENTOS DO VETOR----------*/

    for (int i = 0; i < n; i++)
    {
        struct racional temp;  // Variável temporária para armazenar o resultado da soma
        if (soma_r(soma, vetor[i], &temp)) // Tenta somar o número racional atual (vetor[i]) com a soma acumulada
            soma = temp;  // Atualiza 'soma' com o resultado
    }

    printf("SOMA = ");
    imprime_r(soma);
    printf("\n");
    return 0;
}
