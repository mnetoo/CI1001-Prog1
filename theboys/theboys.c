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
#include "eventos.h"



#define CHEGADA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRADA 4
#define SAIDA 5
#define VIAGEM 6
#define MISSAO 7
#define FIM 8
#define MORRE 9
#define PRIORIDADE 1



int main ()
{
    srand(0);
    int espera_ou_desiste = 0, tpb = 0;
    int destino = 0, duracao = 0, aux = 0;
    int concluida = 0, missao_concluida = 0, missao_adiada = 0;
    int xtipo, xtempo;

    struct fprio_t *simulacao = fprio_cria();
    struct evento_t *evento;
    struct evento_t *novo_evento;
    struct mundo m = cria_mundo(); 

    //  EVENTOS INICIAIS PARA A CHEGADA DOS HEROIS
    for (int i = 0; i < N_HEROIS; i++) 
    {
        int tempo = aleat(1, 4321);
	//fprintf(stderr,"inserindo heroi %d no tempo %d\n", i, tempo);
        evento = cria_evento(tempo, CHEGADA, m.vetor_herois[i].id_heroi, m.vetor_bases[aleat(0, N_BASES-1)].id_base);
	if (evento == NULL)
	  exit(0);
        fprio_insere(simulacao, evento, 0, tempo);
    }


    //  EVENTOS INICIAIS PARA AS MISSOES
    for (int i = 0; i < N_MISSOES; i++) 
    {
        int tempo = aleat(0, T_FIM_DO_MUNDO);
        evento = cria_evento(tempo, MISSAO, m.vetor_missoes[i].id_missao, 0);
 	if (evento == NULL)
	  exit(0);
       fprio_insere(simulacao, evento, 0, tempo);
    }


    //  ADICIONA O EVENTO FINAL PARA O FIM DA SIMULAÇÃO
    evento = cria_evento(T_FIM_DO_MUNDO, FIM, 0, 0);
    if (evento == NULL)
      exit(0);
    fprio_insere(simulacao, evento, 0, T_FIM_DO_MUNDO);

    evento = fprio_retira(simulacao, &xtipo, &xtempo);
    while (evento->tempo <= T_FIM_DO_MUNDO)
    { 
	//fprintf(stderr,"fprio: %d / evento: %d\n", simulacao->num, evento->tipo);
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


            case ESPERA:
                espera(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, evento->dado2);
                fprio_insere(simulacao, novo_evento, AVISA, m.tempo);
                break;


            case DESISTE:
                destino = desiste(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                fprio_insere(simulacao, novo_evento, VIAGEM, m.tempo);
                break;


            case AVISA:
                avisa(evento->tempo, &m, evento->dado2);
                int heroiRemovido, prioridade;
                while (m.vetor_bases[evento->dado2].lotacao > cjto_card(m.vetor_bases[evento->dado2].presentes) && fprio_tamanho(m.vetor_bases[evento->dado2].espera) != 0) 
                {
                    fprio_retira(m.vetor_bases[evento->dado2].espera, &heroiRemovido, &prioridade);
                    
                    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n", evento->tempo, m.vetor_bases[evento->dado2].id_base, heroiRemovido);
                    cjto_insere(m.vetor_bases[evento->dado2].presentes, heroiRemovido);
                    novo_evento = cria_evento(m.tempo, ENTRADA, heroiRemovido, evento->dado2);
                    fprio_insere(simulacao, novo_evento, ENTRADA, m.tempo);
                }
                break;


            case ENTRADA:
                tpb = entra(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + tpb, SAIDA, evento->dado1, evento->dado2);
                fprio_insere(simulacao, novo_evento, SAIDA, m.tempo);
                break;


            case SAIDA:
                aux = evento->dado2;
                destino = sai(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                fprio_insere(simulacao, novo_evento, VIAGEM, m.tempo);
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, aux);
                fprio_insere(simulacao, novo_evento, AVISA, m.tempo);
                break;


            case VIAGEM:
            duracao = viaja(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + duracao, CHEGADA, evento->dado1, evento->dado2);
                fprio_insere(simulacao, novo_evento, CHEGADA, m.tempo);
                break;


            case MISSAO:
                concluida = missao(evento->tempo, evento->dado1, &m);
                if (concluida == 1)
                {
                    missao_concluida++;
                } 
                else 
                {
                    missao_adiada++;
                    novo_evento = cria_evento(m.tempo + 24 * 60, MISSAO, evento->dado1, 0);
                    fprio_insere(simulacao, novo_evento, MISSAO, m.tempo);
                }
                break;


            case MORRE:
                morre(evento->tempo, &m, simulacao, evento->dado1, evento->dado2);
                break;


            case FIM:
                fim(evento->tempo, &m, missao_concluida, missao_adiada);
                destroi_evento(evento);
                fprio_destroi(simulacao);
                destroi_mundo(&m);
                return 0;
        }
        destroi_evento(evento);

        evento = fprio_retira(simulacao, &xtipo, &xtempo);
    }
    return 0;
}
