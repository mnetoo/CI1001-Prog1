#include <stdio.h>
#include <stdlib.h>
#include "racional.h"


int main() 
{
    int n;
    struct racional r1, r2;
    struct racional r_soma;
    struct racional vetor[100];


    // Lê um valor n tal que 0 < n < 100
    do {
        scanf("%d", &n);
    } while (n <= 0 || n >= 100);












    // Preenche o vetor com n números racionais lidos da entrada
    for (int i = 0; i < n; i++) 
    {
        long num, den;
        scanf("%ld %ld", &num, &den);
        vetor[i] = cria_r(num, den);
    }









    // Imprime os números racionais do vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);  // Passa o racional do vetor diretamente para a função
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




    return 0;
}
