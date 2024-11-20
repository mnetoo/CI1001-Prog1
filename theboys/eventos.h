#ifndef EVENTOS
#define EVENTOS

#include "fprio.h"
#include "conjunto.h"
#include "lista.h"
#include "entidades.h"


#define T_INICIO          0
#define T_FIM_DO_MUNDO    525600
#define N_TAMANHO_MUNDO   20000
#define N_HABILIDADES     10
#define N_HEROIS          (N_HABILIDADES * 5)
#define N_BASES           (N_HEROIS / 5)
#define N_MISSOES         (T_FIM_DO_MUNDO / 100)

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





// Função para gerar número aleatório em um intervalo
int aleat(int min, int max);

//  FUNÇÃO PARA CRIAR MUNDO - INICIALIZA HERÓIS, BASES, MISSÕES
struct mundo cria_mundo();

//  FUNÇÃO PARA DESTRUIR O MUNDO CRIADO
void destroi_mundo(struct mundo *mundo);

//  TIRA O HEROI DO CONJUNTO DE PRESENTES DA BASE 
//  RETORNA UMA BASE ALEATÓRIA
int sai (int tempo, struct mundo *mundo, int h, int b);

//  CALCULA O TPB E RETORNA O VALOR
int entra (int tempo, struct mundo *mundo, int h, int b);

//  FUNÇÃO QUE AVISA O ESTADO DE UMA BASE PARA O PORTEIRO
void avisa (int tempo, struct mundo *mundo, int b);

//  FUNÇÃO PARA ADICIONAR O HEROI NA FILA DE ESPERA DA BASE
void espera (int tempo, struct mundo *mundo, int h, int b);

//  FUNÇÃO QUE CALCULA DISTÂNCIA DO HERIO ATEÉ A OUTRA BASE E RETORNA A DURAÇÃO
int viaja (int tempo, struct mundo *mundo, int h, int base_destino);

//  DEFINE UMA BASE DE DESTINO ALEATÓRIA
int desiste (int tempo, struct mundo *mundo, int h, int b);

//  EVENTO CHEGA
//  HEROI (H) CHEGA NA BASE (B) NO TEMPO (T), (H) ANALISA O TAM_FILA E DECIDE SE ESPERA
int chega (int tempo, struct mundo *mundo, int h, int b);

// FUNÇÃO EVENTO FIM
void fim (int tempo, struct mundo *mundo, int missao_concluida, int missao_adiada);

int compara (const void* a, const void* b) ;

//  Evento MORRE 
//  MARCA A MORTE DE UM HEROI, LIBERA A VAGA E AVISA O PORTEIRO DA BASE
void morre (int tempo, struct mundo *mundo, struct fprio_t *f ,int h, int b);

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2);

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e);

int missao (int tempo, int indice_missao, struct mundo *mundo);

void *fprio_retira(struct fprio_t *f, int *tipo, int *prio);




#endif