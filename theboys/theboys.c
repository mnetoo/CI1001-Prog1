/*

--------- PROGRAMA PRINCIPAL - TRABALHO FINAL DE PROGRAMAÇÃO 1 ---------
-------- ALUNO: MARCUS SEBASTIÃO ADRIANO ROCHA NETO - 202407103 --------
----------------- PROFESSOR DAVID MENOTTI GOMES ------------------------

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
#define TIPO_HEROI 0
#define TIPO_BASE 1
#define TIPO_MISSAO 3
#define EVENTO_MORTE 1
#define EVENTO_ADIADO 2




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
        mundo.vetor_herois[i].id_heroi = i;
        mundo.vetor_herois[i].habilidade = cjto_cria(N_HABILIDADES);
        mundo.vetor_herois[i].paciencia = aleat(0, 100);
        mundo.vetor_herois[i].velocidade = aleat(50, 5000);
        mundo.vetor_herois[i].experiencia = 0;
        mundo.vetor_herois[i].base = -1;
        mundo.vetor_herois[i].vida = 1;

        int aleatorio = aleat(1, 3);

        for (int j = 0; j < aleatorio; j++)
        {
            int nova_habilidade;
            do
            {
                nova_habilidade = aleat(0, N_HABILIDADES - 1);
            } while (cjto_pertence(mundo.vetor_herois[i].habilidade, nova_habilidade));
            cjto_insere(mundo.vetor_herois[i].habilidade, nova_habilidade);
        }
    }

    // INICIALIZANDO AS MISSOES
    for (int i = 0; i < N_MISSOES; i++)
    {
        mundo.vetor_missoes[i].id_missao = i;
        mundo.vetor_missoes[i].habilidade = cjto_cria(N_HABILIDADES);
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
            } while (cjto_pertence(mundo.vetor_missoes[i].habilidade, nova));
            cjto_insere(mundo.vetor_missoes[i].habilidade, nova);
        }
    }

    // INICIALIZANDO AS BASES
    for (int i = 0; i < N_BASES; i++)
    {
        mundo.vetor_bases[i].id_base = i;
        mundo.vetor_bases[i].lotacao = aleat(3, 10);
        mundo.vetor_bases[i].presentes = cjto_cria(mundo.vetor_bases[i].lotacao);
        mundo.vetor_bases[i].espera = fprio_cria();
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
        cjto_destroi(mundo->vetor_herois[i].habilidade);

    for (int i = 0; i < N_BASES; i++)
    {
        cjto_destroi(mundo->vetor_bases[i].presentes);
        fprio_destroi(mundo->vetor_bases[i].espera);
    }

    for (int i = 0; i < N_MISSOES; i++)
        cjto_destroi(mundo->vetor_missoes[i].habilidade);
}


/*=====================================================================================================================================*/


//  TIRA O HEROI DO CONJUNTO DE PRESENTES DA BASE 
//  RETORNA UMA BASE ALEATÓRIA
int sai (int tempo, struct mundo *mundo, int h, int b)
{
    int base_aleat;

    cjto_retira(mundo->vetor_bases[b].presentes, mundo->vetor_herois[h].id_heroi);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, mundo->vetor_herois[h].id_heroi,
           mundo->vetor_bases[b].id_base, cjto_card(mundo->vetor_bases[b].presentes),
           mundo->vetor_bases[b].lotacao);

    /// Define uma base destino aleatória
    // (DO WHILE) Impede que o  herói vá para a mesa base
    do
        base_aleat = aleat(0, N_BASES - 1);
    while (base_aleat == mundo->vetor_bases[b].id_base);

    return base_aleat;
}


/*=====================================================================================================================================*/


//  CALCULA O TPB E RETORNA O VALOR
int entra (int tempo, struct mundo *mundo, int h, int b)
{
    int TPB;

    TPB = 15 + (mundo->vetor_herois[h].paciencia * aleat(1, 20));

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, mundo->vetor_herois[h].id_heroi,
           mundo->vetor_bases[b].id_base, cjto_card(mundo->vetor_bases[b].presentes),
           mundo->vetor_bases[b].lotacao, tempo + TPB);

    return TPB;
}


/*=====================================================================================================================================*/


//  FUNÇÃO QUE AVISA O ESTADO DE UMA BASE PARA O PORTEIRO
void avisa (int tempo, struct mundo *mundo, int b)
{
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, mundo->vetor_bases[b].id_base,
           cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);

    cjto_imprime(mundo->vetor_bases[b].presentes);

    return;
}


/*=====================================================================================================================================*/


//  FUNÇÃO PARA ADICIONAR O HEROI NA FILA DE ESPERA DA BASE
void espera (int tempo, struct mundo *mundo, int h, int b)
{   

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, mundo->vetor_herois[h].id_heroi,
           mundo->vetor_bases[b].id_base, fprio_tamanho(mundo->vetor_bases[b].espera));

    fprio_insere(mundo->vetor_bases[b].espera, &mundo->vetor_herois[h], TIPO_HEROI, PRIORIDADE);
}


/*=====================================================================================================================================*/


//  FUNÇÃO QUE CALCULA DISTÂNCIA DO HERIO ATEÉ A OUTRA BASE E RETORNA A DURAÇÃO
int viaja (int tempo, struct mundo *mundo, int h, int base_destino)
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

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, mundo->vetor_herois[h].id_heroi,
           mundo->vetor_herois[h].base, base_destino, distancia,
           mundo->vetor_herois[h].velocidade, tempo + duracao);

    return duracao;
}


/*=====================================================================================================================================*/


//  DEFINE UMA BASE DE DESTINO ALEATÓRIA
int desiste (int tempo, struct mundo *mundo, int h, int b)
{
    int base_destino;

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, mundo->vetor_herois[h].id_heroi,
           mundo->vetor_bases[b].id_base);

    base_destino = aleat(0, N_BASES - 1);

    return base_destino;
}


/*=====================================================================================================================================*/


//  EVENTO CHEGA
//  HEROI (H) CHEGA NA BASE (B) NO TEMPO (T), (H) ANALISA O TAM_FILA E DECIDE SE ESPERA
int chega (int tempo, struct mundo *mundo, int h, int b)
{
    int vagas, fila_espera_vazia;
    int espera = 0;

    mundo->vetor_herois[h].base = mundo->vetor_bases[b].id_base;

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
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA \n", tempo, mundo->vetor_herois[h].id_heroi,
               mundo->vetor_bases[b].id_base, cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);
        return 1; // ADD LEF (EVENTO ESPERA)
    }

    else
    {
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE \n", tempo, mundo->vetor_herois[h].id_heroi,
               mundo->vetor_bases[b].id_base, cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);
        return 0; // ADD LEF (EVENTO DESISTE)
    }
}


/*=====================================================================================================================================*/


// FUNÇÃO EVENTO FIM
void fim (int tempo, struct mundo *mundo, int missao_concluida, int missao_adiada)
{
    int n_missoes = N_MISSOES;
    int i;
   
    printf("%6d: FIM \n", tempo);

    //  IMPRIME OS HEROIS
    for (i = 0;  i < N_HEROIS; i++)
    {
         printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", mundo->vetor_herois[i].id_heroi,mundo->vetor_herois[i].paciencia, 
                mundo->vetor_herois[i].velocidade, mundo->vetor_herois[i].experiencia);
         cjto_imprime(mundo->vetor_herois[i].habilidade);
       }
   
   printf("%d/%d MISSOES CONCLUIDAS  (%.2f%%) MEDIA %.2F TENTATIVAS/MISSAO \n", missao_concluida, N_MISSOES, 
            ((double)missao_concluida/n_missoes)*100.0, ( (double)missao_concluida+(double)missao_adiada)/n_missoes);
    
    return;
}


/*=====================================================================================================================================*/


int compara (const void* a, const void* b) 
{
    return (*(struct distancia_base*)a).distancia - (*(struct distancia_base*)b).distancia;
}


/*=====================================================================================================================================*/


//  Evento MORRE 
//  MARCA A MORTE DE UM HEROI, LIBERA A VAGA E AVISA O PORTEIRO DA BASE
void morre (int tempo, struct mundo *mundo, struct fprio_t *f ,int h, int b)
{
    // RETIRA O HEROI DO CONJUNTO DE HEROIS PRESENTES NA BASE
    cjto_retira(mundo->vetor_bases[b].presentes, mundo->vetor_herois[h].id_heroi);

    // ATUALIZA HEROI COMO MORTO
    mundo->vetor_herois[h].vida = -1;

    // IMPRIME MENSAGEM DE HEROI MORTO E LIBERA A VAGA
    printf("%6d: MORRE  HEROI %2d BASE %d (%2d/%2d)\n", tempo, mundo->vetor_herois[h].id_heroi, mundo->vetor_bases[b].id_base, 
           cjto_card(mundo->vetor_bases[b].presentes), mundo->vetor_bases[b].lotacao);

    // Cria e insere na LEF o evento AVISA, informando a base B sobre a nova vaga
    fprio_insere(f , &mundo->vetor_bases, AVISA, PRIORIDADE);

    return;
}


/*=====================================================================================================================================*/


/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2)
{
    struct evento_t *e;

    if(!(e = malloc(sizeof(struct evento_t))))
        return NULL;

    e->tempo = tempo;
    e->tipo = tipo;
    e->dado1 = dado1;
    e->dado2 = dado2;

    return e;
}


/*=====================================================================================================================================*/


int missao (int tempo, int indice_missao, struct mundo *mundo)
{
    int h, b;
    double distancia[N_BASES];
    struct distancia_base distanciasBases[N_BASES];


    //  CALCULA A DISTÂNCIA DAS BASES PARA A MISSÃO
    for(int i = 0; i < N_BASES; i++) 
    {
        distancia[i] = sqrt(pow(mundo->vetor_bases[i].local_baseX - mundo->vetor_missoes[indice_missao].local_missaoX, 2) +
                            pow(mundo->vetor_bases[i].local_baseY - mundo->vetor_missoes[indice_missao].local_missaoY, 2));

        distanciasBases[i].distancia = distancia[i];
        distanciasBases[i].id_base = i;
    }


    //  ORDENA AS BASES USANDO QUICKSORT
    qsort(distanciasBases, N_BASES, sizeof(struct distancia_base), compara);


    printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, mundo->vetor_missoes[indice_missao].id_missao, 1);
    cjto_imprime(mundo->vetor_missoes[indice_missao].habilidade);


    bool achou = false;
    int base_localizada = -1;


    //  PROCURA A BASE MAIS PRÓXIMA QUE OS HEROIS POSSUEM AS HABILIDADES NECESSARIAS
    for (b = 0; b < N_BASES && !achou; b++) 
    {
        struct cjto_t *cjto_habilidades_base = cjto_cria(N_HABILIDADES);

        for (h = 0; h < N_HEROIS; h++) 
        {
            if (cjto_pertence(mundo->vetor_bases[distanciasBases[b].id_base].presentes, mundo->vetor_herois[h].id_heroi)) 
            {    
                struct cjto_t *temp = cjto_uniao(cjto_habilidades_base, mundo->vetor_herois[h].habilidade);
                cjto_destroi(cjto_habilidades_base);
                cjto_habilidades_base = temp;
            }
        }


        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS: ", tempo, mundo->vetor_missoes[indice_missao].id_missao,
                mundo->vetor_bases[distanciasBases[b].id_base].id_base, (int)distanciasBases[b].distancia);
        cjto_imprime(mundo->vetor_bases[distanciasBases[b].id_base].presentes);

        printf("%6d: MISSAO %d HAB HEROI ", tempo, mundo->vetor_missoes[indice_missao].id_missao);


        for (h = 0; h < N_HEROIS; h++) 
        {
            if (cjto_pertence(mundo->vetor_bases[distanciasBases[b].id_base].presentes, mundo->vetor_herois[h].id_heroi)) 
            {
                printf("%2d: ", mundo->vetor_herois[h].id_heroi);
                cjto_imprime(mundo->vetor_herois[h].habilidade);
            }
        }


        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", tempo, mundo->vetor_missoes[indice_missao].id_missao,
               mundo->vetor_bases[distanciasBases[b].id_base].id_base);
        cjto_imprime(cjto_habilidades_base);

        if (cjto_contem(cjto_habilidades_base, mundo->vetor_missoes[indice_missao].habilidade)) 
        {
            base_localizada = distanciasBases[b].id_base;
            achou = true;
        }

        cjto_destroi(cjto_habilidades_base);


        // SE ENCONTROU A BASE
        if (achou) 
        {
            // ITERA SOBRE OS HEROIS PARA CALCULAR O RISCO
            // DETERMINA SE INCREMENTA A EXPERIENCIA OU ADICIONA EVENTO MORRE
            for (int i = 0; i < N_HEROIS; i++) 
            {
                if (cjto_pertence(mundo->vetor_bases[base_localizada].presentes, mundo->vetor_herois[i].id_heroi)) 
                {
                    double paciencia = mundo->vetor_herois[i].paciencia;
                    double experiencia = mundo->vetor_herois[i].experiencia;
                    double risco = mundo->vetor_missoes[indice_missao].perigo / (paciencia + experiencia + 1.0);
                    
                    int aleatorio = aleat(0, 30);

                    if (risco > aleatorio)
                        cria_evento(tempo, EVENTO_MORTE, mundo->vetor_herois[i].id_heroi, indice_missao);  // Ajustado com tipo e dado2
                    else 
                        mundo->vetor_herois[i].experiencia += 1;
                }
            }

            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", tempo, mundo->vetor_missoes[indice_missao].id_missao,
                mundo->vetor_bases[base_localizada].id_base);
            cjto_imprime(mundo->vetor_bases[base_localizada].presentes);
            return 1;
        }
        else
        {
            cria_evento(tempo + 24 * 60, EVENTO_ADIADO, indice_missao, 0);  // Ajustado com tipo e dado2
            printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, mundo->vetor_missoes[indice_missao].id_missao);
            return 0;
        }
    }
    return 0;
}


/*=====================================================================================================================================*/


int main ()
{
    srand(0);
    int espera_ou_desiste = 0;

    struct fprio_t *simulacao = fprio_cria();
    struct evento_t *evento;
    struct evento_t *novo_evento;
    struct mundo m = cria_mundo(); 


    //  EVENTOS INICIAIS PARA A CHEGADA DOS HEROIS
    for (int i = 0; i < N_HEROIS; i++) 
    {
        int tempo = aleat(1, 4321);
        evento = cria_evento(tempo, 0, m.vetor_herois[i].id_heroi, m.vetor_bases[aleat(0, N_BASES-1)].id_base);
        fprio_insere(simulacao, evento, 0, PRIORIDADE);
    }


    //  EVENTOS INICIAIS PARA AS MISSOES
    for (int i = 0; i < N_MISSOES; i++) 
    {
        int tmpo = aleat(0, T_FIM_DO_MUNDO);
        evento = cria_evento(tmpo, 7, m.vetor_missoes[i].id_missao, 0);
        fprio_insere(simulacao, evento, 0, PRIORIDADE);
    }


    //  ADICIONA O EVENTO FINAL PARA O FIM DA SIMULAÇÃO
    evento = cria_evento(T_FIM_DO_MUNDO, FIM, 0, 0);
    fprio_insere(simulacao, evento, 0, PRIORIDADE);


    while (fprio_tamanho(simulacao) > 0) 
    {
        evento = fprio_retira(simulacao, &evento->tipo, &evento->tempo);
        m.tempo = evento->tempo;

        switch (evento->tipo) 
        {

            case CHEGADA:
                espera_ou_desiste = chega(evento->tempo, &m, evento->dado1, evento->dado2);
                if (espera_ou_desiste == 1) 
                {
                    novo_evento = cria_evento(m.tempo, ESPERA, evento->dado1, evento->dado2);
                    fprio_insere(simulacao, novo_evento, ESPERA, m.tempo);
                } 
                else
                {
                    novo_evento = cria_evento(m.tempo, DESISTE, evento->dado1, evento->dado2);
                    fprio_insere(simulacao, novo_evento, DESISTE, m.tempo);
                }
                break;
        }
    }


    return 0;
}
