#include <stdio.h>
#include "racional.h"

#define TAM 100

int main()
{
  int n;
  struct racional r1, r2;
  struct racional r_soma;
  struct racional vetor[TAM];


  /* Realiza a leitura ate o valor de "n" estar no intervalo [1, 99] */
  do
  {
    scanf("%d", &n);
  } while (n <= 0 || n >= 100);


  /* Preenche o vetor com os valores de entrada */
  for(int i = 0; i < n; i++)
  {
    scanf("%ld/%ld", r1.num, r1.den);
  }

















  return (0);
}