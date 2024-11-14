/*

--------- PROGRAMA PRINCIPAL - TRABALHO FINAL DE PROGRAMAÇÃO 1 ---------
-------- ALUNO: MARCUS SEBASTIÃO ADRIANO ROCHA NETO - 202407103 ---------

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"





#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

#define CHEGADA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRADA 4
#define SAIDA 5
#define VIAGEM 6
#define MISSAO 7
#define FIM 8
#define PRIORIDADE 1



/*=====================================================================================================================================*/

// Função para gerar número aleatório em um intervalo
int aleat(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/*=====================================================================================================================================*/






struct MUNDO cria_mundo()
{
    struct MUNDO mundo;

    mundo.NHerois = N_HEROIS;
    mundo.NBases = N_BASES;
    mundo.NMissoes = N_MISSOES;
    mundo.TAM_MUNDO = N_TAMANHO_MUNDO;
    mundo.TEMPO = 0;
    mundo.NHabilidades = N_HABILIDADES;

    // INICIALIZANDO OS HERÓIS
    for (int i = 0; i < N_HEROIS; i++)
    {
        mundo.HEROIS_MUNDO[i].ID = i;
        mundo.HEROIS_MUNDO[i].habilidade = cria_cjt(N_HABILIDADES);
        mundo.HEROIS_MUNDO[i].paciencia = aleat(0, 100);
        mundo.HEROIS_MUNDO[i].velocidade = aleat(50, 5000);
        mundo.HEROIS_MUNDO[i].experiencia = 0;
        mundo.HEROIS_MUNDO[i].base = -1;

        int aleatorio = aleat(1, 3);

        for (int j = 0; j < aleatorio; j++)
        {
            int nova_habilidade;
            do
            {
                nova_habilidade = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(mundo.HEROIS_MUNDO[i].habilidade, nova_habilidade));
            insere_cjt(mundo.HEROIS_MUNDO[i].habilidade, nova_habilidade);
        }
    }

    // INICIALIZANDO AS MISSOES
    for (int i = 0; i < N_MISSOES; i++)
    {
        mundo.MISSOES_MUNDO[i].ID = i;
        mundo.MISSOES_MUNDO[i].habilidade = cria_cjt(N_HABILIDADES);
        mundo.MISSOES_MUNDO[i].perigo = aleat(0, 100);
        mundo.MISSOES_MUNDO[i].local_missaoX = aleat(0, N_TAMANHO_MUNDO - 1);
        mundo.MISSOES_MUNDO[i].local_missaoY = aleat(0, N_TAMANHO_MUNDO - 1);

        int aleatorio = aleat(6, 10);

        for (int j = 0; j < aleatorio; j++)
        {
            int nova;
            do
            {
                nova = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(mundo.MISSOES_MUNDO[i].habilidade, nova));
            insere_cjt(mundo.MISSOES_MUNDO[i].habilidade, nova);
        }
    }

    // INICIALIZANDO AS BASES
    for (int i = 0; i < N_BASES; i++)
    {
        mundo.BASES_MUNDO[i].ID = i;
        mundo.BASES_MUNDO[i].lotacao = aleat(3, 10);
        mundo.BASES_MUNDO[i].presentes = cria_cjt(mundo.BASES_MUNDO[i].lotacao);
        mundo.BASES_MUNDO[i].espera = cria_fprio();
        mundo.BASES_MUNDO[i].local_baseX = aleat(0, N_TAMANHO_MUNDO - 1);
        mundo.BASES_MUNDO[i].local_baseY = aleat(0, N_TAMANHO_MUNDO - 1);
    }

    return mundo;
}






/*=====================================================================================================================================*/







void destroi_mundo(struct MUNDO *mundo)
{
    for (int i = 0; i < N_HEROIS; i++)
        destroi_cjt(mundo->HEROIS_MUNDO[i].habilidade);

    for (int i = 0; i < N_BASES; i++)
    {
        destroi_cjt(mundo->BASES_MUNDO[i].presentes);
        destroi_fprio(mundo->BASES_MUNDO[i].espera);
    }

    for (int i = 0; i < N_MISSOES; i++)
        destroi_cjt(mundo->MISSOES_MUNDO[i].habilidade);
}








/*=====================================================================================================================================*/







/* retira o heroi do conjunto de presentes da base e retorna uma base aleatoria */
int evento_sai(int tempo, struct MUNDO *mundo, int h, int b)
{
    int base_aleat;

    retira_cjt(mundo->BASES_MUNDO[b].presentes, mundo->HEROIS_MUNDO[h].ID);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, mundo->HEROIS_MUNDO[h].ID,
           mundo->BASES_MUNDO[b].ID, cjto_card(mundo->BASES_MUNDO[b].presentes),
           mundo->BASES_MUNDO[b].lotacao);

    /* define uma base destino aleatoria */
    /* um do while para que o heroi nao va para a mesma base */
    do
        base_aleat = aleat(0, N_BASES - 1);
    while (base_aleat == mundo->BASES_MUNDO[b].ID);

    return base_aleat;
}







/*=====================================================================================================================================*/







/* calcula o TPB e retorna o valor */
int evento_entra(int tempo, struct MUNDO *mundo, int h, int b)
{
    int TPB;

    TPB = 15 + (mundo->HEROIS_MUNDO[h].paciencia * aleat(1, 20));

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, mundo->HEROIS_MUNDO[h].ID,
           mundo->BASES_MUNDO[b].ID, cjto_card(mundo->BASES_MUNDO[b].presentes),
           mundo->BASES_MUNDO[b].lotacao, tempo + TPB);

    return TPB;
}







/*=====================================================================================================================================*/







void evento_avisa(int tempo, struct MUNDO *mundo, int b)
{
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, mundo->BASES_MUNDO[b].ID,
           cjto_card(mundo->BASES_MUNDO[b].presentes), mundo->BASES_MUNDO[b].lotacao);

    imprime_cjt(mundo->BASES_MUNDO[b].presentes);

    return;
}







/*=====================================================================================================================================*/















/* adiciona o ID do heroi na fila de espera da base */
void evento_espera(int tempo, struct MUNDO *mundo, int h, int b)
{   

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, mundo->HEROIS_MUNDO[h].ID,
           mundo->BASES_MUNDO[b].ID, fprio_tamanho(mundo->BASES_MUNDO[b].espera));

    fprio_insere(mundo->BASES_MUNDO[b].espera, mundo->HEROIS_MUNDO[h].ID, mundo->HEROIS_MUNDO, PRIORIDADE);
}













/*=====================================================================================================================================*/












/* calcula a distancia do heroi ate a outra base e a retorna */
int evento_viaja(int tempo, struct MUNDO *mundo, int h, int base_destino)
{
    /*Obtemos as posições das bases de origem e destino*/
    int pos_x_origem = mundo->BASES_MUNDO[mundo->HEROIS_MUNDO[h].base].local_baseX;
    int pos_y_origem = mundo->BASES_MUNDO[mundo->HEROIS_MUNDO[h].base].local_baseY;
    int pos_x_destino = mundo->BASES_MUNDO[base_destino].local_baseX;
    int pos_y_destino = mundo->BASES_MUNDO[base_destino].local_baseY;

    /* Calcula a distância cartesiana*/
    double distancia_x = pow(pos_x_destino - pos_x_origem, 2);
    double distancia_y = pow(pos_y_destino - pos_y_origem, 2);
    int distancia = sqrt(distancia_x + distancia_y);

    int duracao = (int)ceil(distancia / mundo->HEROIS_MUNDO[h].velocidade);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, mundo->HEROIS_MUNDO[h].ID,
           mundo->HEROIS_MUNDO[h].base, base_destino, distancia,
           mundo->HEROIS_MUNDO[h].velocidade, tempo + duracao);

    /* retorna o tempo de duracao da viajem */
    return duracao;
}













/*=====================================================================================================================================*/










/* define uma base destino aleatoria*/
int evento_desiste(int tempo, struct MUNDO *mundo, int h, int b)
{
    int base_destino;

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, mundo->HEROIS_MUNDO[h].ID,
           mundo->BASES_MUNDO[b].ID);

    base_destino = aleat(0, N_BASES - 1);

    return base_destino;
}










/*=====================================================================================================================================*/






/*
Evento CHEGA -
Representa um herói H chegando em uma base B no instante T. Ao chegar,
o herói analisa o tamanho da fila e decide se espera para entrar ou desiste
*/

int evento_chega(int tempo, struct MUNDO *mundo, int h, int b)
{
    int vagas, fila_espera_vazia;
    int espera = 0;

    mundo->HEROIS_MUNDO[h].base = mundo->BASES_MUNDO[b].ID;

    /*Verifica se há vagas na base*/
    vagas = mundo->BASES_MUNDO[b].lotacao > cjto_card(mundo->BASES_MUNDO[b].presentes);

    /*Verifica se a fila de espera está vazia*/
    fila_espera_vazia = fila_vazia(mundo->BASES_MUNDO[b].espera);

    /*Determina se o herói decide esperar ou desiste*/
    if (vagas && fila_espera_vazia)
        espera = 1; /*O herói espera se há vagas e a fila está vazia*/

    else if (mundo->HEROIS_MUNDO[h].paciencia > (10 * fprio_tamanho(mundo->BASES_MUNDO[b].espera)))
        espera = 1;

    else
        espera = 0;

    if (espera)
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA \n", tempo, mundo->HEROIS_MUNDO[h].ID,
               mundo->BASES_MUNDO[b].ID, cjto_card(mundo->BASES_MUNDO[b].presentes), mundo->BASES_MUNDO[b].lotacao);
        return 1;
    }

    else
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE \n", tempo, mundo->HEROIS_MUNDO[h].ID,
               mundo->BASES_MUNDO[b].ID, cjto_card(mundo->BASES_MUNDO[b].presentes), mundo->BASES_MUNDO[b].lotacao);
        return 0;
    }
}




/*=====================================================================================================================================*/






int comparar(const void* a, const void* b) 
{
    return (*(struct distancia_base*)a).distancia - (*(struct distancia_base*)b).distancia;
}





/*=====================================================================================================================================*/





