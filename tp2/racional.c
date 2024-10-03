#include <stdio.h>
#include <stdlib.h>
#include "racional.h"


long aleat(long min, long max) 
{
    return min + rand() % (max - min + 1);
}

/*---------------------------------------------------------------------------------------------------------------------------*/
/* Máximo Divisor Comum entre a e b */
long mdc(long a, long b)
{
  if (b == 0)
    return a;

  return mdc(b, a % b);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Mínimo Múltiplo Comum entre a e b */
long mmc(long a, long b)
{
  return (a * b) / mdc(a, b);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Recebe um número racional e o simplifica. */
struct racional simplifica_r(struct racional r)
{
  int divisor;

  if (r.den == 0)
    return r;

  divisor = mdc(r.num, r.den);

  r.num /= divisor;
  r.den /= divisor;

  if(r.den < 0)
  {
    r.num = -r.num;
    r.den = -r.den;
  }

  return r;
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r(long numerador, long denominador)
{
  struct racional r;

  r.num = numerador;
  r.den = denominador;

  return simplifica_r(r);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna 1 se o racional r for válido ou 0 se for inválido. */
int valido_r(struct racional r) 
{
    return (r.den != 0);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna um número racional aleatório na forma simplificada. */
struct racional sorteia_r(long min, long max)
{
  struct racional r;

  r.num = aleat(min, max);
  r.den = aleat(min, max);

  return simplifica_r(r);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Imprime um racional r */
void imprime_r (struct racional r) 
{
   if (!valido_r(r)) // Se o racional for inválido, imprime NaN
   {
       printf("NaN ");
       return;
   }

   if (r.num == 0) // SE (numerador = 0) ENTÃO imprime 0
       printf("0 ");
   else if (r.den == 1) // SE (denominador = 1) ENTÃO imprime numerador
       printf("%ld ", r.num);
   else if (r.num == r.den) // SE (numerador = denominador) ENTÃO imprime 1
       printf("1 ");
   else // Caso geral, imprime "num/den"
       printf("%ld/%ld ", r.num, r.den);
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Compara dois números racionais r1 e r2 */
int compara_r(struct racional r1, struct racional r2) 
{
    // Multiplica o numerador de r1 pelo denominador de r2
    long numerador1 = r1.num * r2.den;

    // Multiplica o numerador de r2 pelo denominador de r1
    long numerador2 = r2.num * r1.den;

    if (!valido_r(r1) || !valido_r(r2)) // Verifica se r1 ou r2 são inválidos (denominador igual a 0) 
        return -2;

    if (numerador1 < numerador2) // Se numerador1 for menor que numerador2, r1 é menor que r2 
        return -1;

    if (numerador1 > numerador2) // Se numerador1 for maior que numerador2, r1 é maior que r2
        return 1;

    // Se os numeradores ajustados forem iguais, r1 é igual a r2
    return 0;
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna a soma dos racionais r1 e r2 e armazena o resultado em r3 */
int soma_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || (r3 == NULL)) /* Verifica se r1 ou r2 são inválidos ou r3 é nulo*/
    return 0;                         // Retorna 0 indicando erro

  long denominador_comum = mmc(r1.den, r2.den); /* Calcula o mmc dos denominadores */

  /* Ajusta os numeradores com base no denominador comum */
  long numerador1 = r1.num * (denominador_comum / r1.den);
  long numerador2 = r2.num * (denominador_comum / r2.den);

  /* Soma os numeradores e armazena em r3 */
  r3->num = numerador1 + numerador2;
  r3->den = denominador_comum;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna a subtração dos racionais r1 e r2 e armazena o resultado em r3 */
int subtrai_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || (r3 == NULL)) /* Verifica se r1 ou r2 são inválidos ou r3 é nulo*/
    return 0;                         // Retorna 0 indicando erro

  long denominador_comum = mmc(r1.den, r2.den); /* Calcula o mmc dos denominadores */

  /* Ajusta os numeradores com base no denominador comum */
  long numerador1 = r1.num * (denominador_comum / r1.den);
  long numerador2 = r2.num * (denominador_comum / r2.den);

  /* Subtrai os numeradores e armazena em r3 */
  r3->num = numerador1 - numerador2;
  r3->den = denominador_comum;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna a multiplicação dos racionais r1 e r2 e armazena o resultado em r3 */
int multiplica_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || (r3 == NULL)) /* Verifica se r1 ou r2 são inválidos ou r3 é nulo*/
    return 0;                         // Retorna 0 indicando erro

  /* Multiplica os numeradores e os denominadores e armazena em r3 */
  r3->num = r1.num * r2.num;
  r3->den = r1.den * r2.den;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}

/*---------------------------------------------------------------------------------------------------------------------------*/

/* Retorna a divisão dos racionais r1 e r2 e armazena o resultado em r3 */
int divide_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || r2.num == 0 || (r3 == NULL)) // Verifica se r1 ou r2 é inválido ou se o numerador de r2 é zero ou se r3 é nulo 
    return 0;                                        // Retorna 0 indicando erro (não pode dividir por zero)

  /* Multiplica o primeiro racional pelo inverso do segundo e armazena em r3 */
  r3->num = r1.num * r2.den;
  r3->den = r1.den * r2.num;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}
