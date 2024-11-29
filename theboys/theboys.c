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
#include "fila.h"
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
#define MORRE 8
#define FIM 9

#define PRIORIDADE 1



int main ()
{
    srand(1);


    struct mundo m = cria_mundo();
    struct evento_t *evento;
    struct evento_t *novo_evento;

    // mundo.simulacao = fprio_cria(); - FEITO NO cria_mundo() - (RECOMENDAÇÃO MENOTTI)


    int xtipo, xtempo;





    //  CHEGADA DE CADA HERÓIS NUMA BASE ALEATÓRIA
    for (int i = 0; i < N_HEROIS; i++) 
    {
        int tempo = aleat(0, 4320);
        int baseAleat= aleat(0, N_BASES- 1);
     	
        evento = cria_evento(tempo, CHEGADA, m.vetor_herois[i].id_heroi, m.vetor_bases[baseAleat].id_base);

	    if (evento == NULL) 
        {
	        fprintf(stderr, "Erro ao criar evento de CHEGADA!\n");
	        exit(EXIT_FAILURE);
        }

        fprio_insere(m.simulacao, evento, CHEGADA, tempo);
    }


    //DEBUG
    for (int i= 0; i < N_HEROIS; i++)
    {

        printf("herói %d na base:   %d\n", i, m.vetor_herois[i].base);
        printf("---- experiencia do heroi %d:   %d\n", i , m.vetor_herois[i].experiencia);
        printf("---- paciencia do heroi %d:   %d\n", i , m.vetor_herois[i].paciencia);
        printf("---- habilidades do heroi %d:   ", i);
        printf("---- ");
        cjto_imprime(m.vetor_herois[i].habilidade);
        printf("\n");
        printf("---- velocidade do heroi %d:   %d\n", i , m.vetor_herois[i].velocidade);
        
        if (m.vetor_herois[i].vida)
            printf("---- herói %d vivo\n", i);

        printf("\n");

    }






    //  EVENTOS INICIAIS PARA AS MISSOES
    for (int i = 0; i < N_MISSOES; i++) 
    {
        int tempo = aleat(0, T_FIM_DO_MUNDO);

        evento = cria_evento(tempo, MISSAO, m.vetor_missoes[i].id_missao, 0);
 	    
        if (evento == NULL) 
        {
	        fprintf(stderr, "Erro ao criar evento de MISSAO!\n");
	        exit(EXIT_FAILURE);
        }
       
       fprio_insere(m.simulacao, evento, MISSAO, tempo);
    }

    //  DEBUG
    for (int i = 0; i < N_MISSOES; i++)
    {
        printf("Missão %d inserida na fila de prioridade.\n", m.vetor_missoes[i].id_missao);
        printf("Tempo: %d\n", aleat(0, T_FIM_DO_MUNDO));
    }








    //  ADICIONA O EVENTO FINAL PARA O FIM DA SIMULAÇÃO
    int tempo_fim = T_FIM_DO_MUNDO;

    evento = cria_evento(tempo_fim, FIM, 0, 0);
    
    if (evento == NULL) 
    {
        fprintf(stderr, "Erro ao criar evento de FIM!\n");
        exit(EXIT_FAILURE);
    }

    fprio_insere(m.simulacao, evento, FIM, tempo_fim);

    //  DEBUG
    printf("Evento final inserido na fila de prioridade.\n");
    printf("Tipo: FIM, Tempo: %d\n", tempo_fim);







    evento = fprio_retira(m.simulacao, &xtipo, &xtempo);

    if (evento != NULL) 
    {
        printf("Evento retirado:\n");
        printf("Tipo: %d, Prioridade: %d\n", xtipo, xtempo);
        // Caso o evento tenha campos adicionais, como ID do herói ou da missão:
        if (xtipo == CHEGADA) 
        {
            printf("ID do Herói: %d, Base: %d\n", evento->dado1, evento->dado2);
        } 
        else if (xtipo == MISSAO) 
        {
            printf("ID da Missão: %d\n", evento->dado2);
        } 
        else if (xtipo == FIM) 
        {
            printf("Evento de término da simulação.\n");
        } 
        else 
        {
            printf("Outro tipo de evento.\n");
        }
    } 
    else 
        printf("Fila de prioridade vazia ou erro ao retirar evento.\n");



    while (evento->tempo <= T_FIM_DO_MUNDO)
    {
        m.tempo = evento->tempo;
        printf("tempo do evento %d\n", evento->tempo);

        switch (evento->tipo) 
        {
            case CHEGADA:

                int decisao = chega(evento->tempo, m, evento->dado1, evento->dado2);
                if (decisao == 1) 
                {
                    novo_evento = cria_evento(m.tempo, ESPERA, evento->dado1, evento->dado2);
                    fprio_insere(m.simulacao, novo_evento, ESPERA, m.tempo);
                } 
                else
                {
                    novo_evento = cria_evento(m.tempo, DESISTE, evento->dado1, evento->dado2);
                    fprio_insere(m.simulacao, novo_evento, DESISTE, m.tempo);
                }
                break;





            case ESPERA:

                espera(evento->tempo, m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, evento->dado2);
                fprio_insere(m.simulacao, novo_evento, AVISA, m.tempo);
                break;







            case DESISTE:

                int destino = desiste(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                fprio_insere(m.simulacao, novo_evento, VIAGEM, m.tempo);
                break;







            case AVISA:

                avisa(evento->tempo, m, evento->dado2);
                int heroiRemovido;
                while (m.vetor_bases[evento->dado2].lotacao > cjto_card(m.vetor_bases[evento->dado2].presentes) && fila_tamanho(m.vetor_bases[evento->dado2].espera) != 0) 
                {
                    fila_retira(m.vetor_bases[evento->dado2].espera, &heroiRemovido);
                    //destroi_evento()
                    
                    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d \n", evento->tempo, m.vetor_bases[evento->dado2].id_base, heroiRemovido);
                    cjto_insere(m.vetor_bases[evento->dado2].presentes, heroiRemovido);
                    novo_evento = cria_evento(m.tempo, ENTRADA, heroiRemovido, evento->dado2);
                    fprio_insere(m.simulacao, novo_evento, ENTRADA, m.tempo);
                }
                break;











            case ENTRADA:

                int tpb = entra(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + tpb, SAIDA, evento->dado1, evento->dado2);
                fprio_insere(m.simulacao, novo_evento, SAIDA, m.tempo + tpb);
                break;






            case SAIDA:

                int aux = evento->dado2;
                destino = sai(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo, VIAGEM, evento->dado1, destino);
                fprio_insere(m.simulacao, novo_evento, VIAGEM, m.tempo);
                
                novo_evento = cria_evento(m.tempo, AVISA, evento->dado1, aux);
                fprio_insere(m.simulacao, novo_evento, AVISA, m.tempo);
                break;







            case VIAGEM:

                int duracao = viaja(evento->tempo, &m, evento->dado1, evento->dado2);
                novo_evento = cria_evento(m.tempo + duracao, CHEGADA, evento->dado1, evento->dado2);
                fprio_insere(m.simulacao, novo_evento, CHEGADA, m.tempo);
                break;






            case MISSAO:
                int missao_concluida = 0;
                int missao_adiada = 0;

                int concluida = missao(evento->tempo, evento->dado1, &m);
                if (concluida == 1)
                {
                    missao_concluida++;
                } 
                else 
                {
                    missao_adiada++;
                    novo_evento = cria_evento(m.tempo + 24 * 60, MISSAO, evento->dado1, 0);
                    fprio_insere(m.simulacao, novo_evento, MISSAO, novo_evento->tempo);
                }
                break;







            case MORRE:

                morre(evento->tempo, &m, m.simulacao, evento->dado1, evento->dado2);
                break;







            case FIM:

                fim(evento->tempo, &m, missao_concluida, missao_adiada);
                destroi_evento(evento); // return 0 bellow
                m.simulacao = fprio_destroi(m.simulacao); // menotti - destruir dentro do mundo
                destroi_mundo(&m);
                return 0;
        }
        destroi_evento(evento);

        evento = fprio_retira(m.simulacao, &xtipo, &xtempo);
    }

    // caso 
    m.simulacao = fprio_destroi(m.simulacao); // menotti - destruir dentro do mundo
    destroi_mundo(&m);

    return 0;
}