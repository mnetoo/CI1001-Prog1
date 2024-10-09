/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/



#include <stdio.h>
#include <stdlib.h>
#include "racional.h"


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





/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 muda para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int simplifica_r(struct racional *r) 
{
    if (r == NULL || r->den == 0)
        return 0; // Retorna 0 em caso de erro: ponteiro nulo ou denominador zero

    int divisor = mdc(r->num, r->den);

    r->num /= divisor;
    r->den /= divisor;

    // Se o denominador for negativo, ajusta os sinais
    if (r->den < 0) 
    {
        r->num = -r->num;
        r->den = -r->den;
    }

    return 1; // Retorna 1 em sucesso
}






/* Cria um número racional com o numerador e denominador indicados
 * e retorna um ponteiro que aponta para ele.
 * A memória para o número racional deve ser alocada dinamicamente
 * por esta função. Retorna NULL se não conseguiu alocar a memória. */
struct racional *cria_r(long numerador, long denominador) 
{
    // Aloca memória para um novo número racional
    struct racional *r = malloc(sizeof(struct racional));
    
    // Verifica se a alocação foi bem-sucedida
    if (r == NULL) 
        return NULL; // Retorna NULL se não conseguiu alocar memória
    

    // Atribui os valores ao novo número racional
    r->num = numerador;
    r->den = denominador;

    // Simplifica o número racional
    simplifica_r(r);

    return r; // Retorna o ponteiro para o número racional
}






/* Libera a memória alocada para o racional apontado por r */
void destroi_r(struct racional *r) 
{
    if (r != NULL)
        free(r); // Libera a memória alocada para o número racional
}





/* Retorna 1 se o racional r for válido ou 0 se for inválido. Um racional
 * é inválido se o denominador for zero ou se ele não tiver sido alocado. */
int valido_r(struct racional *r) 
{
    if (r == NULL)
        return 0; // Retorna 0 se o ponteiro for nulo

    return (r->den != 0); // Retorna 1 se o denominador não for zero, caso contrário retorna 0
}






/* Imprime um racional r, respeitando estas regras:
   - o racional deve estar na forma simplificada;
   - não use espacos em branco e não mude de linha;
   - o formato de saída deve ser "num/den", a menos dos casos abaixo;
     - se o ponteiro for nulo, imprime a mensagem "NULL";
     - se o racional for inválido, imprime a mensagem "NaN" (Not a Number);
     - se o numerador for 0, imprime somente "0";
     - se o denominador for 1, imprime somente o numerador;
     - se o numerador e denominador forem iguais, imprime somente "1";
     - se o racional for negativo, o sinal é impresso antes do número;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r(struct racional *r) 
{
    if (r == NULL) 
    {
        printf("NULL");
        return;
    }
    

    if (!valido_r(r)) 
    {
        printf("NaN");
        return;
    }


    // Se o numerador for 0, imprime "0"
    if (r->num == 0) 
    {
        printf("0");
    } 

    // Se o denominador for 1, imprime somente o numerador
    else if (r->den == 1) 
        printf("%ld", r->num);


    // Se o numerador e o denominador forem iguais, imprime "1"
    else if (r->num == r->den) 
        printf("1");
  

    // Formato padrão "num/den"
    else 
    {
        // Imprime o sinal negativo se o numerador for negativo
        if (r->num < 0 || r->den < 0)
            printf("%ld/%ld", -abs(r->num), abs(r->den));
        else
            printf("%ld/%ld", r->num, r->den);
    }
}





/* Compara dois números racionais r1 e r2.
 * Retorna -2 se r1 ou r2 for inválido ou se o respectivo ponteiro for nulo.
 * Retorna -1 se r1 < r2; 0 se r1 = r2; 1 se r1 > r2.
 * Atenção: faça a comparação normalizando os denominadores pelo MMC.
 * Fazer a comparação baseado na divisão do numerador pelo denominador
 * pode gerar erro de arredondamento e falsear o resultado. */
int compara_r(struct racional *r1, struct racional *r2) 
{
    // Verifica se algum dos ponteiros é nulo
    if (r1 == NULL || r2 == NULL) 
        return -2; // Retorna -2 se algum ponteiro for nulo

    // Verifica a validade dos racionais
    if (!valido_r(r1) || !valido_r(r2))
        return -2; // Retorna -2 se algum racional não for válido

    // Calcula o MMC dos denominadores
    long denominador_mmc = mmc(r1->den, r2->den);
    
    // Normaliza os numeradores
    long numerador1 = r1->num * (denominador_mmc / r1->den);
    long numerador2 = r2->num * (denominador_mmc / r2->den);

    // Compara os numeradores normalizados
    if (numerador1 < numerador2)
        return -1; // r1 é menor que r2

    if (numerador1 > numerador2)
        return 1; // r1 é maior que r2

    return 0; // r1 é igual a r2
}





/* Coloca em *r3 a soma simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int soma_r(struct racional *r1, struct racional *r2, struct racional *r3) 
{
    // Verifica se r1, r2 são inválidos ou se r3 é nulo
    if (r1 == NULL || r2 == NULL || r3 == NULL || !valido_r(r1) || !valido_r(r2))
        return 0; // Retorna 0 indicando erro

    long denominador_comum = mmc(r1->den, r2->den);

    long numerador1 = r1->num * (denominador_comum / r1->den);
    long numerador2 = r2->num * (denominador_comum / r2->den);

    r3->num = numerador1 + numerador2;
    r3->den = denominador_comum;

    // Simplifica o resultado e armazena de volta em r3
    simplifica_r(r3); // Correção: deve passar o ponteiro

    return 1; // Retorna 1 indicando sucesso
}





/* Coloca em *r3 a diferença simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int subtrai_r(struct racional *r1, struct racional *r2, struct racional *r3) 
{
    // Verifica se r1 ou r2 são inválidos ou se r3 é nulo
    if (r1 == NULL || r2 == NULL || r3 == NULL || !valido_r(r1) || !valido_r(r2)) 
        return 0; // Retorna 0 indicando erro

    long denominador_comum = mmc(r1->den, r2->den);

    long numerador1 = r1->num * (denominador_comum / r1->den);
    long numerador2 = r2->num * (denominador_comum / r2->den);

    r3->num = numerador1 - numerador2;
    r3->den = denominador_comum;

    // Simplifica o resultado e armazena de volta em r3
    simplifica_r(r3); // Chama a função para simplificar

    return 1; // Retorna 1 indicando sucesso
}






/* Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
/* Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int multiplica_r(struct racional *r1, struct racional *r2, struct racional *r3) 
{
    // Verifica se r1 ou r2 são nulos ou se r3 é nulo
    if (r1 == NULL || r2 == NULL || r3 == NULL || !valido_r(r1) || !valido_r(r2)) 
        return 0; // Retorna 0 indicando erro

    r3->num = r1->num * r2->num;
    r3->den = r1->den * r2->den;

    // Simplifica o resultado e armazena de volta em r3
    simplifica_r(r3); // Chama a função para simplificar

    return 1; // Retorna 1 indicando sucesso
}






/* Coloca em *r3 a divisão simplificada do racional *r1 por *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int divide_r(struct racional *r1, struct racional *r2, struct racional *r3) 
{
    // Verifica se r1 ou r2 são nulos ou se r3 é nulo
    // Também verifica se r2 é válido e não zero
    if (r1 == NULL || r2 == NULL || r3 == NULL || !valido_r(r1) || !valido_r(r2) || r2->num == 0) 
        return 0; // Retorna 0 indicando erro

    r3->num = r1->num * r2->den;
    r3->den = r1->den * r2->num;

    // Simplifica o resultado e armazena de volta em r3
    simplifica_r(r3); // Chama a função para simplificar

    return 1; // Retorna 1 indicando sucesso
}
 
