// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria ()
{
    struct lista_t *lista = (struct lista_t *) malloc(sizeof(struct lista_t));

    if (lista == NULL)
        return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->tamanho = 0;

    return lista;
}


// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst)
{
    if (lst == NULL)
        return NULL;

    struct item_t*atual = lst->prim;

    while (atual != NULL)
    {
        struct item_t * temp = atual;
        atual = atual->prox;
        free(temp);
    }

    free(lst);

    return NULL;
}



// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos)
{
    if (lst == NULL)
        return -1; 

    struct item_t *novo = (struct item_t *)malloc(sizeof(struct item_t));
    if (novo == NULL)
        return -1; 

    novo->valor = item;
    novo->ant = NULL;
    novo->prox = NULL;

    // Caso a posição seja -1 ou maior que o tamanho atual, insere no final
    if (pos < 0 || pos >= lst->tamanho)
    {
        // Inserir no final
        if (lst->ult != NULL) // Se a lista não está vazia
        {
            lst->ult->prox = novo; // O último item aponta para o novo item
            novo->ant = lst->ult;  // O novo item aponta para o antigo último
            lst->ult = novo;        // Atualiza o ponteiro do último item
        }
        else // Se a lista está vazia
        {
            lst->prim = novo; // O novo item é o primeiro
            lst->ult = novo;  // O novo item é o último
        }
    }
    else
    {
        // Inserir na posição especificada
        struct item_t *atual = lst->prim;

        // Percorre até a posição desejada
        for (int i = 0; i < pos && atual != NULL; i++)
        {
            atual = atual->prox; // Move para o próximo item
        }

        // Insere no início
        if (atual == lst->prim)
        {
            novo->prox = lst->prim; // O novo item aponta para o antigo primeiro
            lst->prim->ant = novo;   // O antigo primeiro aponta para o novo item
            lst->prim = novo;        // Atualiza o ponteiro do primeiro item
        }
        else
        {
            // Insere no meio ou no fim
            if (atual != NULL) // Se não for o último
            {
                novo->prox = atual; // O novo item aponta para o item atual
                novo->ant = atual->ant; // O novo item aponta para o anterior do atual
                atual->ant->prox = novo; // O item anterior do atual aponta para o novo
                atual->ant = novo;       // O atual aponta para o novo
            }
            else // Se a posição é igual ao tamanho, insere no fim
            {
                lst->ult->prox = novo; // O último item aponta para o novo item
                novo->ant = lst->ult;   // O novo item aponta para o último
                lst->ult = novo;        // Atualiza o ponteiro do último item
            }
        }
    }

    lst->tamanho++; // Incrementa o tamanho da lista
    return lst->tamanho; // Retorna o número de itens na lista
}



// Retira o item da lista da posição indicada.
// se a posição for além do fim da lista ou for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira(struct lista_t *lst, int *item, int pos)
{
    if (lst == NULL || item == NULL) // Verifica se a lista ou o ponteiro do item é nulo
        return -1;

    if (lst->tamanho == 0) // Verifica se a lista está vazia
        return -1;

    struct item_t *atual;

    // Caso a posição seja -1 ou maior ou igual ao tamanho, retira do final
    if (pos < 0 || pos >= lst->tamanho)
    {
        atual = lst->ult; // Ponto para o último item

        if (atual == NULL) // Verifica se o último item é nulo
            return -1;

        *item = atual->valor; // Armazena o valor do item a ser retirado

        if (atual->ant != NULL) // Se não for o primeiro item
        {
            lst->ult = atual->ant; // Atualiza o ponteiro do último item
            lst->ult->prox = NULL;  // O novo último item não aponta para nenhum próximo
        }
        else // Se for o único item na lista
        {
            lst->prim = NULL; // A lista fica vazia
            lst->ult = NULL;  // O último também fica nulo
        }
    }
    else
    {
        // Retira o item na posição especificada
        atual = lst->prim;

        // Percorre até a posição desejada
        for (int i = 0; i < pos && atual != NULL; i++)
        {
            atual = atual->prox; // Move para o próximo item
        }

        if (atual == NULL) // Se não encontrou o item
            return -1;

        *item = atual->valor; // Armazena o valor do item a ser retirado

        // Atualiza os ponteiros do item anterior e próximo
        if (atual->ant != NULL) // Se não for o primeiro item
            atual->ant->prox = atual->prox;
        else // Se for o primeiro item
            lst->prim = atual->prox; // O próximo item se torna o primeiro

        if (atual->prox != NULL) // Se não for o último item
            atual->prox->ant = atual->ant;
        else // Se for o último item
            lst->ult = atual->ant; // O anterior se torna o último

        // Libera a memória do item retirado
        free(atual);
    }

    lst->tamanho--; // Decrementa o tamanho da lista
    return lst->tamanho; // Retorna o número de itens na lista após a operação
}



// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for além do fim da lista ou for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta(struct lista_t *lst, int *item, int pos)
{
    if (lst == NULL || item == NULL) // Verifica se a lista ou o ponteiro do item é nulo
        return -1;

    if (lst->tamanho == 0) // Verifica se a lista está vazia
        return -1;

    struct item_t *atual;

    // Caso a posição seja -1 ou maior ou igual ao tamanho, consulta o último item
    if (pos < 0 || pos >= lst->tamanho)
    {
        atual = lst->ult; // Ponto para o último item

        if (atual == NULL) // Verifica se o último item é nulo
            return -1;

        *item = atual->valor; // Armazena o valor do item a ser consultado
    }
    else
    {
        // Consulta o item na posição especificada
        atual = lst->prim;

        // Percorre até a posição desejada
        for (int i = 0; i < pos && atual != NULL; i++)
        {
            atual = atual->prox; // Move para o próximo item
        }

        if (atual == NULL) // Se não encontrou o item
            return -1;

        *item = atual->valor; // Armazena o valor do item a ser consultado
    }

    return lst->tamanho; // Retorna o número de itens na lista
}



// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura(struct lista_t *lst, int valor)
{
    if (lst == NULL) // Verifica se a lista é nula
        return -1;

    struct item_t *atual = lst->prim; // Ponteiro para percorrer a lista
    int posicao = 0; // Inicializa a posição em 0

    // Percorre a lista até encontrar o valor ou até o final
    while (atual != NULL)
    {
        if (atual->valor == valor) // Verifica se o valor do item é igual ao valor procurado
            return posicao; // Retorna a posição se o valor for encontrado

        atual = atual->prox; // Move para o próximo item
        posicao++; // Incrementa a posição
    }

    return -1; // Retorna -1 se o valor não for encontrado
}




// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho(struct lista_t *lst)
{
    if (lst == NULL) // Verifica se a lista é nula
        return -1;

    return lst->tamanho; // Retorna o tamanho da lista
}



// Imprime o conteúdo da lista do início ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime(struct lista_t *lst)
{
    if (lst == NULL || lst->prim == NULL) // Verifica se a lista é nula ou vazia
    {
        return; // Não imprime nada se a lista estiver vazia
    }

    struct item_t *atual = lst->prim; // Ponteiro para percorrer a lista
    while (atual != NULL) // Enquanto não chegar ao final da lista
    {
        printf("%d", atual->valor); // Imprime o valor do item

        atual = atual->prox; // Move para o próximo item

        if (atual != NULL) // Se não for o último item, imprime um espaço
        {
            printf(" ");
        }
    }
}