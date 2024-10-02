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
        scanf("%ld/%ld", &vetor[i].num, &vetor[i].den);

    // Imprime os números racionais do vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++)
        imprime_r(vetor[i]);  // Passa o racional do vetor diretamente para a função
    printf("\n");
    
    return 0;
}
