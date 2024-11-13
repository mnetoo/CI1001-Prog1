#ifndef ENTIDADE
#define ENTIDADE


#include "fprio.h"
#include "conjunto.h"
#include "lista.h"


// Definindo as estruturas
struct HEROI 
{
    int ID;           
    int habilidade;
    int paciencia;   
    int velocidade;   
    int experiencia;  
    int base;
};

/*=====================================================================================================================================*/

struct BASE 
{
    int ID;             
    int lotacao;         
    struct cjto_t presentes;  
    struct fprio_t espera;  
    int localX;         
    int localY;          
};

/*=====================================================================================================================================*/

struct MISSAO 
{
    int ID;             
    struct cjto_t *habilidade;  
    int perigo;          
    int localX;          
    int localY;    
};

/*=====================================================================================================================================*/

struct MUNDO 
{
    int NHerois;         
    struct H *herois;    
    int NBases;          
    struct B *bases;     
    int NMissoes;        
    struct M *missoes;  
    int NHabilidades;    
    int TamX;            
    int TamY;           
    int relogio;         
};

/*=====================================================================================================================================*/

struct Evento 
{
    int tempo;     // Tempo do evento (em minutos)
    int heroiID;   // ID do herói
    int baseID;    // ID da base de destino
    char tipo[10]; // Tipo do evento (exemplo: "CHEGA")
};

/*=====================================================================================================================================*/


#endif