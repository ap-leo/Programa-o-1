#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
struct fila *fila_cria ()
{
   struct fila *f = malloc(sizeof(struct fila));   /* aloca memoria para a nova fila */
   if (f == NULL)
     return NULL;

   f->ini = NULL;   
   f->tamanho = 0;   /* inicia com tamanho 0 */

   return f;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (struct fila **fila)
{
   struct nodo *f, *aux;
   f = (*fila)->ini;   /* ponteiro aponta para o inicio da fila */
   while (f != NULL)   /* sai do laço quando inicio apontar para NULL, ou seja, não tem próximo elemento na fila */ 
   {
     aux = f;   /* auxiliar é criado para que a memoria possa ser destruida mas 'prox' não seja perdido */
     f = f->prox;   /* p, que é inicio, aponta para o próximo nodo da lista */
     free(aux);   /* desalocamos aux que aponta para o antigo inicio */ 
   }

   free(*fila);
   *fila = NULL;
     
   return;
}

/* 
 * Insere dado na fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (struct fila *fila, int dado)
{
   if (fila == NULL)   /* retorna 0 em caso de fila vazia */
     return 0;

   struct nodo *novoNodo = malloc(sizeof(struct nodo));   /* verifica se existe memoria para um novo nodo */

   if (novoNodo == NULL)   /* retorna 0 em caso de não ter espaço para nodo */
     return 0;

   if (fila->tamanho == 0)  /* caso não exista nodo */ 
   {
     fila->ini = novoNodo;  /* inicio e fim apontaram para o mesmo nodo adicionado */
     fila->fim = novoNodo;
     novoNodo->chave = dado;
     fila->fim->prox = NULL;  /* prox prox será null para fim e ini, já q estão no mesmo nodo */
     fila->ini->prox = NULL;
     fila->tamanho++;

    return 1;
   }
   
   fila->fim->prox = novoNodo;  /* caso a fila contenha nodo */
   novoNodo->chave = dado;  
   fila->fim = novoNodo;   /* novo nodo será o ultimo da fila */
   fila->fim->prox = NULL;   /* ele apontará para null (ele é o ultimo da fila) */
   fila->tamanho++;
   
   return 1;
}

/* 
 * Remove dado da fila (politica FIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (struct fila *fila, int *dado)
{
   if (fila->tamanho == 0)  /* caso a fila não tenha nodos, retorna 0 */
     return 0;
   
   struct nodo *f = fila->ini;  /* cria-se um novo para apontar para inicio */
   *dado = f->chave;   /* dado será a chave do ponteiro para o qual f aponta */
   fila->ini = fila->ini->prox; /* o segundo elemento da fila se torna o primeiro */
   fila->tamanho--;
   free(f); /* desaloca o primeiro elemento */

   return 1;
}
 
/* Retorna o numero de elementos da fila, que pode ser 0. */
int fila_tamanho (struct fila *fila)
{
   return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (struct fila *fila)
{
   if (fila->tamanho == 0) 
      return 1;
   else 
      return 0;
}

