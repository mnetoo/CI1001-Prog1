#include <stdio.h>
#include "racional.h"


long aleat(long min, long max) 
{
    return min + rand() % (max - min + 1);
}

/* Máximo Divisor Comum entre a e b */
long mdc (long a, long b)
{
  if (b == 0)
    return a;

  return mdc(b, a % b);
}

/* Mínimo Múltiplo Comum entre a e b */
long mmc (long a, long b)
{   
  return (a * b) / mdc(a, b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r)
{
  int divisor;

  if (r.den == 0) return r;

  divisor = mdc(r.num, r.den);

  r.num /= divisor;
  r.den /= divisor;

  return r;
}

/* Imprime um racional */
void imprime_r(struct racional r)
{
  if (!valido_r(r)) // Se o racional for inválido, imprime INVALIDO
  {        
    printf(" INVALIDO");
    return;
  }
  
  r = simplifica_r(r);
    
  if (r.num == 0)// Se (numerador = 0) então imprime 0
  {
    printf(" 0");
    return;
  }

  if (r.den == 0) // Se (denominador = 0) então imprime invalido
    printf(" INVALIDO");

  else if (r.num == r.den) // Se (numerador = denominador) então imprime 1
    printf(" 1");
    
  else if (r.den == 1) // Se (denominador = 1) então imprime o numerador
    printf(" %ld", r.num);
    
  else if (r.num > 0 && r.den < 0) // Se (denominador < 0) então imprime -numerador/denominador
    printf(" -%ld/%ld", r.num, (r.den * -1));

  else if (r.num < 0 && r.den < 0) // Se (numerador < 0 && denominador < 0) então imprime racional positivo
    printf(" %ld/%ld", (r.num * -1), (r.den * -1)); 

  else // Imprime o racional normalmente
    printf(" %ld/%ld ", r.num, r.den);
}

/* Retorna a soma dos racionais r1 e r2. */
struct racional soma_r(struct racional r1, struct racional r2)
{ 
  struct racional r;

  if (!valido_r(r1) || !valido_r(r2)) /* Verifica se r1 ou r2 são inválidos */
    return cria_r(0, 0);

  long denominador_comum = mmc(r1.den, r2.den); /* Calcula o mmc dos denominadores */

  /* Ajusta os numeradores com base no denominador comum */
  long numerador1 = r1.num * (denominador_comum / r1.den);    
  long numerador2 = r2.num * (denominador_comum / r2.den);

  /* Soma os numeradores */
  r.num = numerador1 + numerador2;
  r.den = denominador_comum;

  return simplifica_r(r);
}
