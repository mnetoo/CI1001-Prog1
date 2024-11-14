#ifndef ENTIDADE
#define ENTIDADE


#include "fprio.h"
#include "conjunto.h"
#include "lista.h"


#define T_INICIO          0
#define T_FIM_DO_MUNDO    525600
#define N_TAMANHO_MUNDO   20000
#define N_HABILIDADES     10
#define N_HEROIS          (N_HABILIDADES * 5)
#define N_BASES           (N_HEROIS / 5)
#define N_MISSOES         (T_FIM_DO_MUNDO / 100)



// Definindo as estruturas
struct heroi 
{
    int ID;           
    int habilidade;
    int paciencia;   
    int velocidade;   
    int experiencia;  
    int base;
};

/*=====================================================================================================================================*/

struct base 
{
    int ID;             
    int lotacao;         
    struct cjto_t *presentes;  
    struct fprio_t *espera;  
    int local_baseX;         
    int local_baseY;          
};

/*=====================================================================================================================================*/

struct missao 
{
    int ID;             
    struct cjto_t *habilidade;  
    int perigo;          
    int local_missaoX;          
    int local_missaoY;    
};

/*=====================================================================================================================================*/

struct mundo 
{
    int NHerois;         
    struct heroi vetor_herois[N_HEROIS];    
    int NBases;          
    struct base vetor_bases[N_BASES];     
    int NMissoes;        
    struct missao vetor_missoes[N_MISSOES];  
    int NHabilidades;    
    int tamanho_mundo;         
    int tempo;         
};

/*=====================================================================================================================================*/

struct evento_t 
{
    int tempo;  /* tempo logico do evento */
    int tipo;   /* inteiro para diferenciar os diferentes tipos de evento */
    int dado1;  /* um inteiro para guardar dados do evento */
    int dado2;  /* um inteiro para guardar dados do evento */
};

/*=====================================================================================================================================*/

struct distancia_base 
{
    double distancia;
    int id_base;
};

/*=====================================================================================================================================*/

#endif
