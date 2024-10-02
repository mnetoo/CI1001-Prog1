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








/* Imprime um racional r, respeitando estas regras:
   - o racional deve ser impresso na forma simplificada;
   - não imprima espaços em branco e não mude de linha;
   - a saída deve ter o formato "num/den", a menos dos casos abaixo:
     - se o racional for inválido, imprime a mensagem "NaN" (Not a Number);
     - se o numerador for 0, imprime somente "0";
     - se o denominador for 1, imprime somente o numerador;
     - se o numerador e denominador forem iguais, imprime somente "1";
     - se o racional for negativo, o sinal "-" vem antes do numerador;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r(struct racional r)
{
    // Verifica se o racional é inválido (denominador 0 ou estrutura inválida)
    if (!valido_r(r) || r.den == 0)
    {
        printf("NaN");
        return;
    }

    // Simplifica o racional
    r = simplifica_r(r);

    // Caso o numerador seja 0, imprime apenas "0"
    if (r.num == 0)
    {
        printf("0");
        return;
    }

    // Caso o denominador seja 1, imprime apenas o numerador
    if (r.den == 1)
    {
        printf("%ld", r.num);
        return;
    }

    // Caso o numerador e denominador sejam iguais, imprime apenas "1"
    if (r.num == r.den)
    {
        printf("1");
        return;
    }

    // Se ambos numerador e denominador forem negativos, o racional é positivo
    if (r.num < 0 && r.den < 0)
    {
        r.num = -r.num;
        r.den = -r.den;
    }

    // Se o numerador for positivo e o denominador negativo, imprime o racional negativo
    if (r.num > 0 && r.den < 0)
    {
        printf("-%ld/%ld", r.num, -r.den);
        return;
    }

    // Se o numerador for negativo e o denominador positivo, imprime normalmente
    if (r.num < 0 && r.den > 0)
    {
        printf("%ld/%ld", r.num, r.den);
        return;
    }

    // Caso geral, imprime "num/den"
    printf("%ld/%ld", r.num, r.den);
}













/* Compara dois racionais r1 e r2.
 * Retorno: -2 se r1 ou r2 for inválido,
 * -1 se r1 < r2,
 * 0 se r1 = r2,
 * 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2)
{
    // Verifica se r1 ou r2 são inválidos
    if (!valido_r(r1) || !valido_r(r2)) {
        return -2;
    }

    // Traz os racionais para o mesmo denominador
    long denominador_comum = mmc(r1.den, r2.den);

    // Ajusta os numeradores com base no denominador comum
    long numerador1 = r1.num * (denominador_comum / r1.den);
    long numerador2 = r2.num * (denominador_comum / r2.den);

    // Compara os numeradores
    if (numerador1 < numerador2) {
        return -1;
    } else if (numerador1 > numerador2) {
        return 1;
    } else {
        return 0; // São iguais
    }
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
