#include <stdio.h>
#include <stdlib.h>
#include "lef.h"

/* Atribui os parametros para os valores da struct evento_t e retorna o ponteiro para esse evento.*/
/* Em caso de erro, retorna NULL */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
	struct evento_t *evento;
	if (! (evento = malloc(sizeof(struct evento_t))))
		return NULL;

	evento->tempo = tempo;
	evento->tipo = tipo;
	evento->dado1 = dado1;
	evento->dado2 = dado2;

	return evento;	
}

/* Desaloca o evento recebido no parametro */
struct evento_t *destroi_evento (struct evento_t *e){
    free(e);

    return NULL;
}

/* Cria uma lef: retorna NULL caso erro e o ponteiro da lef em caso de sucesso */
struct lef_t *cria_lef (){
    struct lef_t *lef;

    if (!(lef = malloc(sizeof(struct lef_t))))
        return NULL;
    
    lef->primeiro = NULL;

    return lef;
}

/* Destroi toda a lef e todo evento alocado. Caso a lef esteja vazia, ou tenha desalocado toda ela */
/* retorna NULL */
struct lef_t *destroi_lef(struct lef_t *l) {
    if (l == NULL)
        return NULL;

    while (! vazia_lef(l)) {
    	struct nodo_lef_t *aux = l->primeiro->prox; /* para nao perder 'primeiro' */
        destroi_evento(l->primeiro->evento);
        l->primeiro = aux;
    }

    free(l); /* destroi lef */

    return NULL;
}

/* Aloca memoria um nodo que aponta para o evento recebido no parametro, então insere ele na posição correta da lef */
int insere_lef(struct lef_t *l, struct evento_t *e) {
    struct nodo_lef_t *aux = l->primeiro;
    struct nodo_lef_t *novo;

    if (!(novo = malloc(sizeof(struct nodo_lef_t))))
        return 0;

    /* o nodo crido aponta para o evento recebido */
    novo->evento = e;
    novo->prox = NULL; 

    /* caso a lef esteja vazia, nodo criado é o primeiro */
    if (vazia_lef(l)) {
        l->primeiro = novo;

        return 1;
    }

    /* encontra a posição correta */
    while (aux->prox != NULL && aux->prox->evento->tempo <= e->tempo) {
        aux = aux->prox;
    }

    /* insere o nodo na lef */
    novo->prox = aux->prox;
    aux->prox = novo;

    return 1;
}

/* retira o primeiro nodo da lef e retorna o evento dele */
struct evento_t *retira_lef (struct lef_t *l){
	if (l->primeiro == NULL) 
    	return NULL;
   
	/* primeira posição passa para a segunda, caso exista */
	struct evento_t *aux = l->primeiro->evento; 
 	struct nodo_lef_t *temp = l->primeiro; 
 	l->primeiro = l->primeiro->prox;
    
 	free(temp);

    return aux;
}

/* verifica se a lef está fazia: retorna 1 caso esteja e 0 caso não */
int vazia_lef (struct lef_t *l){
    if (l == NULL || l->primeiro == NULL)
        return 1;

    return 0;
}

/* imprime a lef e suas informação, além de informar quantos eventos tem */
void imprime_lef(struct lef_t *l) {
	int aux = 0;
	printf("LEF:\n");

    /* caso lef vazia */
    if (l == NULL || l->primeiro == NULL) {
        printf("  Total %d eventos\n", aux);

        return;
    }

    struct nodo_lef_t *a = l->primeiro;

    while (a != NULL) {
    	aux++;
        printf("  tempo %d tipo %d d1 %d d2 %d\n", a->evento->tempo, a->evento->tipo, a->evento->dado1, a->evento->dado2);
        a = a->prox;
    }

    printf("  Total %d eventos\n", aux);

    return;
}
