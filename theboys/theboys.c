// programa principal do projeto "The Boys - 2024/2"
// Autor: Marcus Sebastião Adriano Rocha Neto, GRR 20240710 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "conjunto.h"  
#include "fprio.h"
#include "lista.h"
#include "entidades.h"

#define T_INICIO          0
#define T_FIM_DO_MUNDO    525600
#define N_TAMANHO_MUNDO   20000
#define N_HABILIDADES     10
#define N_HEROIS          (N_HABILIDADES * 5)
#define N_BASES           (N_HEROIS / 5)
#define N_MISSOES         (T_FIM_DO_MUNDO / 100)




/*=====================================================================================================================================*/

// Função para gerar número aleatório em um intervalo
int gerarNumeroAleatorio(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

/*=====================================================================================================================================*/

// Função para inicializar heróis
void inicializarHerois(struct HEROI *herois, int nHerois) 
{
    for (int i = 0; i < nHerois; i++) 
    {
        herois[i].ID = i;
        herois[i].experiencia = 0;
        herois[i].paciencia = gerarNumeroAleatorio(0, 100);
        herois[i].velocidade = gerarNumeroAleatorio(50, 5000);

        // Definindo habilidades aleatórias
        int numHabilidades = gerarNumeroAleatorio(1, 3);  // Quantidade de habilidades do herói
        herois[i].habilidade = 0; // Inicializando habilidade com 0

        for (int j = 0; j < numHabilidades; j++) 
        {
            int habilidadeAleatoria = gerarNumeroAleatorio(1, N_HABILIDADES);
            herois[i].habilidade |= (1 << habilidadeAleatoria); // Marca a habilidade no conjunto
        }
    }
}

/*=====================================================================================================================================*/

// Função para inicializar bases
void inicializarBases(struct BASE *bases, int nBases) 
{
    for (int i = 0; i < nBases; i++) {
        bases[i].ID = i;
        bases[i].lotacao = gerarNumeroAleatorio(3, 10);  // Lotação entre 3 e 10
        bases[i].localX = gerarNumeroAleatorio(0, N_TAMANHO_MUNDO - 1);
        bases[i].localY = gerarNumeroAleatorio(0, N_TAMANHO_MUNDO - 1);

        // Inicializar conjunto vazio para os heróis presentes na base (com capacidade para a lotação)
        bases[i].presentes = *cjto_cria(bases[i].lotacao);  // Criando o conjunto com a lotação da base

        // Inicializar fila vazia para os heróis em espera
        fprio_cria(&bases[i].espera);  // Inicializa a fila de prioridade vazia
    }
}

/*=====================================================================================================================================*/

// Função para inicializar missões
void inicializarMissoes(struct MISSAO *missoes, int nMissoes) 
{
    for (int i = 0; i < nMissoes; i++) {
        missoes[i].ID = i;  // ID da missão sequencial de 0 a N_MISSOES-1
        missoes[i].localX = gerarNumeroAleatorio(0, N_TAMANHO_MUNDO - 1);  // Coordenada X aleatória
        missoes[i].localY = gerarNumeroAleatorio(0, N_TAMANHO_MUNDO - 1);  // Coordenada Y aleatória

        // Gerar conjunto de habilidades com tamanho aleatório entre 6 e 10
        int numHabilidades = gerarNumeroAleatorio(6, 10);  // Tamanho do conjunto de habilidades
        missoes[i].habilidade = cjto_cria(numHabilidades);  // Criando o conjunto de habilidades com a quantidade gerada

        // Atribuir o nível de perigo (número aleatório entre 0 e 100)
        missoes[i].perigo = gerarNumeroAleatorio(0, 100);  // Nível de perigo
    }
}

/*=====================================================================================================================================*/











int main() 
{
    srand(time(NULL));

    // Inicializar o mundo, criar heróis, bases, missões e demais elementos
    struct HEROI herois[N_HEROIS];
    struct BASE bases[N_BASES];
    struct MISSAO missoes[N_MISSOES];
    struct Evento filaEventos[1000];  // Tamanho da fila de eventos (pode ser ajustado)
    int nEventos = 0;

    inicializarHerois(herois, N_HEROIS);
    inicializarBases(bases, N_BASES);
    inicializarMissoes(missoes, N_MISSOES);


    return 0;
}