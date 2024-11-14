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


//FUNÇÃO PARA CRIAR MUNDO - INICIALIZA HERÓIS, BASES, MISSÕES
struct mundo cria_mundo()
{
    struct mundo mundo;

    mundo.NHerois = N_HEROIS;
    mundo.NBases = N_BASES;
    mundo.NMissoes = N_MISSOES;
    mundo.tamanho_mundo = N_TAMANHO_MUNDO;
    mundo.tempo = 0;
    mundo.NHabilidades = N_HABILIDADES;

    // INICIALIZANDO OS HERÓIS
    for (int i = 0; i < N_HEROIS; i++)
    {
        mundo.vetor_herois[i].ID = i;
        mundo.vetor_herois[i].habilidade = cria_cjt(N_HABILIDADES);
        mundo.vetor_herois[i].paciencia = aleat(0, 100);
        mundo.vetor_herois[i].velocidade = aleat(50, 5000);
        mundo.vetor_herois[i].experiencia = 0;
        mundo.vetor_herois[i].base = -1;

        int aleatorio = aleat(1, 3);

        for (int j = 0; j < aleatorio; j++)
        {
            int nova_habilidade;
            do
            {
                nova_habilidade = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(mundo.vetor_herois[i].habilidade, nova_habilidade));
            insere_cjt(mundo.vetor_herois[i].habilidade, nova_habilidade);
        }
    }

    // INICIALIZANDO AS MISSOES
    for (int i = 0; i < N_MISSOES; i++)
    {
        mundo.vetor_missoes[i].ID = i;
        mundo.vetor_missoes[i].habilidade = cria_cjt(N_HABILIDADES);
        mundo.vetor_missoes[i].perigo = aleat(0, 100);
        mundo.vetor_missoes[i].local_missaoX = aleat(0, N_TAMANHO_MUNDO - 1);
        mundo.vetor_missoes[i].local_missaoY = aleat(0, N_TAMANHO_MUNDO - 1);

        int aleatorio = aleat(6, 10);

        for (int j = 0; j < aleatorio; j++)
        {
            int nova;
            do
            {
                nova = aleat(0, N_HABILIDADES - 1);
            } while (pertence_cjt(mundo.vetor_missoes[i].habilidade, nova));
            insere_cjt(mundo.vetor_missoes[i].habilidade, nova);
        }
    }

    // INICIALIZANDO AS BASES
    for (int i = 0; i < N_BASES; i++)
    {
        mundo.vetor_bases[i].ID = i;
        mundo.vetor_bases[i].lotacao = aleat(3, 10);
        mundo.vetor_bases[i].presentes = cria_cjt(mundo.vetor_bases[i].lotacao);
        mundo.vetor_bases[i].espera = cria_fprio();
        mundo.vetor_bases[i].local_baseX = aleat(0, N_TAMANHO_MUNDO - 1);
        mundo.vetor_bases[i].local_baseY = aleat(0, N_TAMANHO_MUNDO - 1);
    }

    return mundo;
}


/*=====================================================================================================================================*/


//FUNÇÃO PARA DESTRUIR O MUNDO CRIADO
void destroi_mundo(struct mundo *mundo)
{
    for (int i = 0; i < N_HEROIS; i++)
        destroi_cjt(mundo->vetor_herois[i].habilidade);

    for (int i = 0; i < N_BASES; i++)
    {
        destroi_cjt(mundo->vetor_bases[i].presentes);
        destroi_fprio(mundo->vetor_bases[i].espera);
    }

    for (int i = 0; i < N_MISSOES; i++)
        destroi_cjt(mundo->vetor_missoes[i].habilidade);
}


/*=====================================================================================================================================*/


//  TIRA O HEROI DO CONJUNTO DE PRESENTES DA BASE 
//  RETORNA UMA BASE ALEATÓRIA
int evento_sai(int tempo, struct mundo *mundo, int h, int b)
{
    int base_aleat;

    retira_cjt(mundo->vetor_bases[b].presentes, mundo->vetor_herois[h].ID);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, mundo->vetor_herois[h].ID,
           mundo->vetor_bases[b].ID, cjto_card(mundo->vetor_bases[b].presentes),
           mundo->vetor_bases[b].lotacao);

    /// Define uma base destino aleatória
    // (DO WHILE) Impede que o  herói vá para a mesa base
    do
        base_aleat = aleat(0, N_BASES - 1);
    while (base_aleat == mundo->vetor_bases[b].ID);

    return base_aleat;
}


/*=====================================================================================================================================*/


//  CALCULA O TPB E RETORNA O VALOR
int evento_entra(int tempo, struct mundo *mundo, int h, int b)
{
    int TPB;

    TPB = 15 + (mundo->vetor_herois[h].paciencia * aleat(1, 20));

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, mundo->vetor_herois[h].ID,
           mundo->vetor_bases[b].ID, cjto_card(mundo->vetor_bases[b].presentes),
           mundo->vetor_bases[b].lotacao, tempo + TPB);

    return TPB;
}


/*=====================================================================================================================================*/


//  FUNÇÃO QUE AVISA O ESTADO DE UMA BASE PARA O PORTEIRO
void evento_avisa(int tempo, struct mundo *mundo, int b)
{
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, mundo->vetor_bases[b].ID,
           cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);

    imprime_cjt(mundo->vetor_bases[b].presentes);

    return;
}


/*=====================================================================================================================================*/


//  FUNÇÃO PARA ADICIONAR O HEROI NA FILA DE ESPERA DA BASE
void evento_espera(int tempo, struct mundo *mundo, int h, int b)
{   

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, mundo->vetor_herois[h].ID,
           mundo->vetor_bases[b].ID, fprio_tamanho(mundo->vetor_bases[b].espera));

    fprio_insere(mundo->vetor_bases[b].espera, mundo->vetor_herois[h].ID, mundo->vetor_herois, PRIORIDADE);
}


/*=====================================================================================================================================*/


//  FUNÇÃO QUE CALCULA DISTÂNCIA DO HERIO ATEÉ A OUTRA BASE E RETORNA A DURAÇÃO
int evento_viaja(int tempo, struct mundo *mundo, int h, int base_destino)
{
    //  POSIÇÕES DAS BASES DE DESTINO E ORIGEM
    int pos_x_origem = mundo->vetor_bases[mundo->vetor_herois[h].base].local_baseX;
    int pos_y_origem = mundo->vetor_bases[mundo->vetor_herois[h].base].local_baseY;
    int pos_x_destino = mundo->vetor_bases[base_destino].local_baseX;
    int pos_y_destino = mundo->vetor_bases[base_destino].local_baseY;

    //  CALCULA A DISTÂNCIA
    double distancia_x = pow(pos_x_destino - pos_x_origem, 2);
    double distancia_y = pow(pos_y_destino - pos_y_origem, 2);
    int distancia = sqrt(distancia_x + distancia_y);

    int duracao = (int)ceil(distancia / mundo->vetor_herois[h].velocidade);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, mundo->vetor_herois[h].ID,
           mundo->vetor_herois[h].base, base_destino, distancia,
           mundo->vetor_herois[h].velocidade, tempo + duracao);

    return duracao;
}


/*=====================================================================================================================================*/


//  DEFINE UMA BASE DE DESTINO ALEATÓRIA
int evento_desiste(int tempo, struct mundo *mundo, int h, int b)
{
    int base_destino;

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, mundo->vetor_herois[h].ID,
           mundo->vetor_bases[b].ID);

    base_destino = aleat(0, N_BASES - 1);

    return base_destino;
}


/*=====================================================================================================================================*/


//  EVENTO CHEGA
//  HEROI (H) CHEGA NA BASE (B) NO TEMPO (T), (H) ANALISA O TAM_FILA E DECIDE SE ESPERA
int evento_chega(int tempo, struct mundo *mundo, int h, int b)
{
    int vagas, fila_espera_vazia;
    int espera = 0;

    mundo->vetor_herois[h].base = mundo->vetor_bases[b].ID;

    //  VERIFICAÇÃO SE HÁ VAGAS NA BASE
    vagas = mundo->vetor_bases[b].lotacao > cjto_card(mundo->vetor_bases[b].presentes);

    //  VERIFICAÇÃO SE A FILA DE ESPERA ESTÁ VAZIA
    fila_espera_vazia = fprio_tamanho(mundo->vetor_bases[b].espera);

    
    //  DECISÃO DO HEROI (H)
    if (vagas && fila_espera_vazia)
        espera = 1; // ESPERA

    else if (mundo->vetor_herois[h].paciencia > (10 * fprio_tamanho(mundo->vetor_bases[b].espera)))
        espera = 1; // ESPERA

    else
        espera = 0; // NÃO ESPERA



    if (espera)
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA \n", tempo, mundo->vetor_herois[h].ID,
               mundo->vetor_bases[b].ID, cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);
        return 1; // ADD LEF (EVENTO ESPERA)
    }

    else
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE \n", tempo, mundo->vetor_herois[h].ID,
               mundo->vetor_bases[b].ID, cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);
        return 0; // ADD LEF (EVENTO DESISTE)
    }
}


/*=====================================================================================================================================*/


int comparar(const void* a, const void* b) 
{
    return (*(struct distancia_base*)a).distancia - (*(struct distancia_base*)b).distancia;
}


/*=====================================================================================================================================*/


int evento_missao(int tempo, int indice_missao, struct mundo *mundo);


/*=====================================================================================================================================*/


void evento_fim(int tempo, struct mundo *mundo, int missao_concluida, int missao_adiada);


/*=====================================================================================================================================*/


// Evento MORRE - Marca a morte de um herói, libera vaga na base e avisa o porteiro
void evento_morre(int tempo, struct mundo *mundo, int h, int b);


/*=====================================================================================================================================*/
