/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/


#include <stdio.h>
#include <stdlib.h>
#include "racional.h"




int main ()
{
    int n;

    // Leitura do valor n
    do {
        printf("Digite um valor n (0 < n < 100): ");
        scanf("%d", &n);
    } while (n <= 0 || n >= 100);

    // Alocação do vetor de ponteiros para racionais
    struct racional **vetor = malloc(n * sizeof(struct racional *));
    if (vetor == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }

    // Preenchendo o vetor com números racionais
    for (int i = 0; i < n; i++) {
        long num, den;
        printf("Digite o numerador e denominador do racional %d: ", i + 1);
        scanf("%ld %ld", &num, &den);
        vetor[i] = cria_r(num, den);
    }

    // Imprimindo o vetor
    printf("VETOR = ");
    for (int i = 0; i < n; i++) {
        imprime_r(vetor[i]);
    }
    printf("\n");

    // Eliminando racionais inválidos
    int contador_valido = 0;
    for (int i = 0; i < n; i++) {
        if (valido_r(vetor[i])) {
            vetor[contador_valido++] = vetor[i];
        } else {
            destroi_r(vetor[i]);  // Libera memória dos racionais inválidos
        }
    }

    // Imprimindo o vetor resultante
    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++) {
        imprime_r(vetor[i]);
    }
    printf("\n");

    // Ordenando o vetor em ordem crescente
    for (int i = 0; i < contador_valido - 1; i++) {
        for (int j = 0; j < contador_valido - 1 - i; j++) {
            if (compara_r(vetor[j], vetor[j + 1]) > 0) {
                struct racional *temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }

    // Imprimindo o vetor ordenado
    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++) {
        imprime_r(vetor[i]);
    }
    printf("\n");

    // Calculando a soma dos racionais válidos
    struct racional *soma = cria_r(0, 1); // Inicializa soma como 0
    for (int i = 0; i < contador_valido; i++) {
        soma_r(soma, vetor[i], soma);
    }

    // Imprimindo a soma
    printf("SOMA = ");
    imprime_r(soma);
    printf("\n");

    // Liberando a memória dos racionais
    for (int i = 0; i < contador_valido; i++) {
        destroi_r(vetor[i]);
    }

    printf("VETOR = ");
    for (int i = 0; i < contador_valido; i++) {
        imprime_r(vetor[i]); // Imprime os racionais ainda apontados, se houver
    }
    printf("\n");

    // Liberando o vetor de ponteiros
    free(vetor);
    // Liberando a soma
    destroi_r(soma);

    return 0;
}

