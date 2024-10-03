#include <stdio.h>
#include <stdlib.h>
#include "racional.h"



long aleat(long min, long max) 
{
    return min + rand() % (max - min + 1);
}

/* Máximo Divisor Comum entre a e b */
long mdc(long a, long b)
{
  if (b == 0)
    return a;

  return mdc(b, a % b);
}

/* Mínimo Múltiplo Comum entre a e b */
long mmc(long a, long b)
{
  return (a * b) / mdc(a, b);
}

/* Recebe um número racional e o simplifica. */
struct racional simplifica_r(struct racional r)
{
  int divisor;

  if (r.den == 0)
    return r;

  divisor = mdc(r.num, r.den);

  r.num /= divisor;
  r.den /= divisor;

  return r;
}

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r(long numerador, long denominador)
{
  struct racional r;

  r.num = numerador;
  r.den = denominador;

  return r;
}

/* Retorna 1 se o racional r for válido ou 0 se for inválido. */
int valido_r(struct racional r)
{
  if (r.den == 0)
    return 0;

  return 1;
}

/* Retorna um número racional aleatório na forma simplificada. */
struct racional sorteia_r(long min, long max)
{
  struct racional r;

  r.num = aleat(min, max);
  r.den = aleat(min, max);

  return simplifica_r(r);
}

void imprime_r(struct racional r)
{
    if (!valido_r(r) || r.den == 0) // Se o racional for inválido, imprime INVALIDO
    {
        printf(" NaN");
        return;
    }

    // Simplifica o racional
    r = simplifica_r(r);

    if (r.num == 0) // SE (numerador = 0) ENTÃO imprime 0
    {
        printf(" 0");
        return;
    }

    if (r.den == 1) // SE (denominador = 1) ENTÃO imprime numerador
    {
        printf(" %ld", r.num);
        return;
    }

    if (r.num == r.den) // SE (numerador = denominador) ENTÃO imprime 1
    {
        printf("1");
        return;
    }

    if (r.num < 0 && r.den < 0) // SE (numerdador < 0 && denominador < 0) ENTÃO racional > 0
    {
        r.num = -r.num;
        r.den = -r.den;
    }

    if (r.num > 0 && r.den < 0) // SE (denominador < 0) ENTÃO racional < 0
    {
        printf(" -%ld/%ld", r.num, -r.den);
        return;
    }

    // Caso geral, imprime "num/den"
    printf(" %ld/%ld", r.num, r.den);
}

/* Compara dois racionais r1 e r2.
 * Retorno: -2 se r1 ou r2 for inválido,
 * -1 se r1 < r2,
 * 0 se r1 = r2,
 * 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2)
{
    if (!valido_r(r1) || !valido_r(r2)) // Verifica se r1 ou r2 são inválidos
        return -2;  

    // Traz os racionais para o mesmo denominador
    long denominador_comum = mmc(r1.den, r2.den);

    // Ajusta os numeradores com base no denominador comum
    long numerador1 = r1.num * (denominador_comum / r1.den);
    long numerador2 = r2.num * (denominador_comum / r2.den);

    if (numerador1 < numerador2) // Compara os numeradores
        return -1; 
    else if (numerador1 > numerador2) 
        return 1;
    else 
        return 0;
}

/* Retorna a soma dos racionais r1 e r2 e armazena o resultado em r3 */
int soma_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2)) /* Verifica se r1 ou r2 são inválidos */
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

/* Retorna a subtração dos racionais r1 e r2 e armazena o resultado em r3 */
int subtrai_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2)) /* Verifica se r1 ou r2 são inválidos */
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

/* Retorna a multiplicação dos racionais r1 e r2 e armazena o resultado em r3 */
int multiplica_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2)) /* Verifica se r1 ou r2 são inválidos */
    return 0;                         // Retorna 0 indicando erro

  /* Multiplica os numeradores e os denominadores e armazena em r3 */
  r3->num = r1.num * r2.num;
  r3->den = r1.den * r2.den;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}

/* Retorna a divisão dos racionais r1 e r2 e armazena o resultado em r3 */
int divide_r(struct racional r1, struct racional r2, struct racional *r3)
{
  if (!valido_r(r1) || !valido_r(r2) || r2.num == 0) /* Verifica se r1 é inválido ou se o numerador de r2 é zero */
    return 0;                                        // Retorna 0 indicando erro (não pode dividir por zero)

  /* Multiplica o primeiro racional pelo inverso do segundo e armazena em r3 */
  r3->num = r1.num * r2.den;
  r3->den = r1.den * r2.num;

  /* Simplifica o resultado em r3 */
  *r3 = simplifica_r(*r3);

  return 1; // Retorna 1 indicando sucesso
}