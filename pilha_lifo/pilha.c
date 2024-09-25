#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

/* 
 * Cria e retorna uma nova pilha.
 * Retorna NULL em caso de erro de alocação.
 * O ponteiro 'p' é utilizado para criar novos nodos ao alocar memoria, ou também exlcluir nodos com o vetor auxiliar. 
*/
struct pilha *pilha_cria ()
{      
   struct pilha *p = malloc(sizeof(struct pilha));   /* aloca memoria para a nova pilha */
   if (p == NULL)
      return NULL;
   p->topo = NULL;
   p->tamanho = 0;   /* inicia com tamanho 0 */

   return p;
}

/* Desaloca toda memoria da pilha e faz pilha receber NULL. */
void pilha_destroi (struct pilha **pilha)
{
   struct nodo *p, *aux;
   p = (*pilha)->topo;   /* ponteiro aponta para topo */
   while (p != NULL)   /* queremos desalocar qualquer elemento que tenha sido alocado, a partir de p (topo) */ 
   {
      aux = p;   /* auxiliar é criado para que a memoria possa ser destruida mas 'prox' não seja perdido */
      p = p->prox;   /* p aponta para p-prox */
      free(aux);   /* desalocamos aux que aponta para pilha->topo */ 
   }

   free(*pilha);
   *pilha = NULL;
      
   return;
}

/* 
 * Insere dado na pilha (politica LIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int push (struct pilha *pilha, int dado)
{
   if (pilha == NULL)
      return 0;

   struct nodo *novoNodo = malloc(sizeof(struct nodo));   /* verifica se existe memoria para um novo nodo */
   if (novoNodo == NULL)
      return 0;
   
   novoNodo->chave = dado; 
   novoNodo->prox = pilha->topo;   /* para que possamos apontar topo sem perder o topo atual, inserimos um novo nodo e apontamos ele para o topo */
 
   pilha->topo = novoNodo;   /* topo passa a apontar para o novo nodo */
   pilha->tamanho++;
   
   return 1;
}

/* 
 * Remove o topo (politica LIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da pilha estar vazia.
*/
int pop (struct pilha *pilha, int *dado)
{
   if (pilha->topo == NULL)
      return 0;
   
   *dado = pilha->topo->chave;   /* copiamos a chave do topo pois vamos desalocar ele e perder o conteudo */ 
   pilha->tamanho--;
   struct nodo *p = pilha->topo;   /* p aponta para o topo */
   pilha->topo = pilha->topo->prox;   /* o topo agora é o penultimo elemento, o qual o topo aponta */
   
   free(p);   /* desalocamos o p, que aponta para o topo */

   return 1;
}
 
/* 
 * Similar ao pop, mas retorna o elemento dado sem remove-lo. 
 * A funcao retorna 1 em caso de sucesso e 0 no caso 
 * da pilha estar vazia.
*/
int pilha_topo (struct pilha *pilha, int *dado)
{
   if (pilha->topo == NULL)
      return 0;
   
   *dado = pilha->topo->chave;

   return 1;
}

/* Retorna o numero de elementos da pilha, que pode ser 0. */
int pilha_tamanho (struct pilha *pilha)
{
   return pilha->tamanho;
}

/* Retorna 1 se pilha vazia, 0 em caso contrario. */ 
int pilha_vazia (struct pilha *pilha)
{
   if (pilha->topo == NULL)
      return 1;
   else
      return 0;
}
