#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <math.h>
#include "conjunto.h"
#include "fila.h"
#include "lef.h"
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

/* 0 fim */
/* 1 chega */
/* 2 missoes */
/* 3 espera */
/* 4 desiste */
/* 5 avisa */
/* 6 viaja */
/* 7 entra */
/* 8 sai */

struct mundo{
    int NHerois;
    struct hero *Herois;
    int Nbases;
    struct base *Bases;
    int NMissoes;
    struct missao *Missoes;
    int NHabilidades;
    struct par *TamanhoMundo; 
    int Relogio;
};

struct par{
    int x;
    int y;
};

struct hero{
    int id;
    int exp;
    int pac;
    int vel;
    struct conjunto *hab;
    int base;
};

struct base{
    int id;
    struct par *local;
    int lot;
    struct conjunto *pres;
    struct fila *esp;
};

struct missao{
    int id;
    struct par *local;
    struct conjunto *hab;
};

struct mundo *i_mundo (){
    struct mundo *m = malloc(sizeof(struct mundo));
    if (m == NULL){
        printf("erro ao iniciar hero");

        return NULL;
    }

    struct par *aux = malloc(sizeof(struct par));
    if (aux == NULL){
        printf("erro ao iniciar par");

        return NULL;
    }

    aux->x = N_TAMANHO_MUNDO;
    aux->y = N_TAMANHO_MUNDO;

    m->NHerois = N_HEROIS;
    m->Nbases = N_BASES;
    m->NMissoes = N_MISSOES;
    m->NHabilidades = N_HABILIDADES;
    m->TamanhoMundo = aux; 
    m->Relogio = T_INICIO;

    return m;
}


struct hero *i_herois (struct mundo *m){
    struct hero *v = malloc(sizeof(struct hero) * m->NHerois);
    if (v == NULL){
        printf("erro ao iniciar hero");

        return NULL;
    }
    int i;
    for(i = 0; i < m->NHerois; i++){
        v[i].id = i;
        v[i].exp = 0;
        v[i].pac = (rand() % 100) + 1;
        v[i].vel = (rand() % 4951) + 50;
        v[i].hab = cria_cjt((rand() % 3) + 1);

        while ((cardinalidade_cjt(v[i].hab) < v[i].hab->max)){
            insere_cjt(v[i].hab, rand() % m->NHabilidades + 1);
        }
    }

    return v;
}

struct base *i_bases (struct mundo *m){
    struct base *v = malloc(sizeof(struct base) * m->Nbases);
    if (v == NULL){
        printf("erro ao iniciar base");

        return NULL;
    }

    int i;
    for (i = 0; i < m->Nbases; i++){
        v[i].id = i;
        v[i].local = malloc(sizeof(struct par));
        v[i].local-> x = rand() % m->TamanhoMundo->x;
        v[i].local->y = rand() % m->TamanhoMundo->y;
        v[i].lot = (rand() % 8) + 3;
        v[i].pres = cria_cjt(v[i].lot);
        v[i].esp = fila_cria();
    }

    return v;
}

struct missao *i_missoes (struct mundo *m){
    struct missao *v = malloc(sizeof(struct missao) * m->NMissoes);
    if (v == NULL){
        printf("erro ao iniciar missao");

        return NULL;
    }

    int i;
    for (i = 0; i < m->NMissoes; i++){
        v[i].id = i;
        v[i].local = malloc(sizeof(struct par));
        v[i].local->x = rand() % m->TamanhoMundo->x;
        v[i].local->y = rand() % m->TamanhoMundo->y;
        v[i].hab = cria_cjt(rand() % 4 + 6);

        while (cardinalidade_cjt(v[i].hab) < v[i].hab->max){
            insere_cjt(v[i].hab, rand() % m->NHabilidades + 1);
        }
    }

    return v;
}

struct lef_t *criar_lef(struct mundo *m){
    struct lef_t *aux = cria_lef();
    int i;
    for(i = 0; i < m->NHerois; i++){
        insere_lef(aux, cria_evento(rand() % 4321, 1, i, rand() % m->Nbases));
    }

    for(i = 0; i < m->NMissoes; i++){
        insere_lef(aux, cria_evento(rand() % T_FIM_DO_MUNDO, 2, i, 0));
    }

   
    insere_lef(aux, cria_evento(T_FIM_DO_MUNDO, 0, 0, 0));

    return aux;
}

void imprime_cjtAux(struct conjunto *c){
    int temp;
    inicia_iterador_cjt(c);
    printf(" [");
    while(incrementa_iterador_cjt(c, &temp)){
        printf(" %d ", temp);
    }
    printf("]\n");

    return;
}

int chega(struct lef_t *l, struct evento_t *e, struct mundo *m){
    bool espera = true;
    int h = e->dado1; 
    int b = e->dado2;
    m->Herois[h].base = b;

    if((! fila_vazia(m->Bases[b].esp)) || (cardinalidade_cjt(m->Bases[b].pres) == m->Bases[b].pres->max)){
        espera = m->Herois[h].pac > (10 * fila_tamanho(m->Bases[b].esp));
    }

    if(espera){
        insere_lef(l, cria_evento(e->tempo, 3, h, b));

        return 1;
    }

    insere_lef(l, cria_evento(e->tempo, 4, h, b));
    return 0;
}

/* calcula a distancia entre dois pontos */
int dist(struct par *cord, struct par *cord2){
    int x = cord->x - cord2->x;
    int y = cord->y - cord2->y;
    int dist = (x * x) + (y * y); 
    dist = sqrt(dist);

    return dist;
}

/* verifica se o conjunto de habilidades de uma base é suficiente para a missao */
int can(struct evento_t *e, struct mundo *m, int k){
    int j = 0;
    inicia_iterador_cjt(m->Bases[k].pres);
    struct conjunto *uniao = cria_cjt(m->NHabilidades);

    struct conjunto *temp;
    while (incrementa_iterador_cjt(m->Bases[k].pres, &j)) {
        temp = uniao_cjt(uniao, m->Herois[j].hab);
        free(uniao->v);
        free(uniao);
        uniao = temp;
        printf("%6d: MISSAO %d HAB HEROI %2d:", m->Relogio, e->dado1, j);
        imprime_cjtAux(m->Herois[j].hab);

        printf("%6d: MISSAO %d UNIAO HAB BASE %d:", m->Relogio, e->dado1, k);
        imprime_cjtAux(uniao);
    }

    int cont = contido_cjt(m->Missoes[e->dado1].hab, uniao);
    free(uniao->v);
    free(uniao);

    return cont;
}

/* verifica se alguma base é valida para a missao */
/* se existir uma válida, retorna 1; 0 se nenhuma */
int mis(struct evento_t *e, struct lef_t *lef, struct mundo *m){
    int b = 0;
    int perto = dist(m->Missoes[e->dado1].local, m->Bases[b].local);
    int temp = 0;
    int j = 0;
    int k = 0;

    printf("%6d: MISSAO %d TENT %d HAB REQ:", m->Relogio, e->dado1, e->dado2);
    imprime_cjtAux(m->Missoes[e->dado1].hab);
    
    if (!vazio_cjt(m->Bases[b].pres)) {
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS", m->Relogio, e->dado1, 0, perto);
        imprime_cjtAux(m->Bases[b].pres);

        if (can(e, m, 0)) {
            temp = 1;
        }
    }

    for (k = 1; k < N_BASES; k++){
        if (!vazio_cjt(m->Bases[k].pres)) {
            j = dist(m->Missoes[e->dado1].local, m->Bases[k].local);
            printf("%6d: MISSAO %d BASE %d DIST %d HEROIS", m->Relogio, e->dado1, k, j);
            imprime_cjtAux(m->Bases[k].pres);
            if (can(e, m, k) && j < perto) {
                perto = j;
                b = k;
                temp = 1;
            }
        }
    }

    if (temp){
        inicia_iterador_cjt(m->Bases[b].pres);
        printf("%6d: MISSAO %d CUMPRIDA BASE %d", m->Relogio, e->dado1, b);
        int inc;
        while (incrementa_iterador_cjt(m->Bases[b].pres, &inc)) {
            m->Herois[inc].exp++;
        }

        return 1;
    }

    printf("%6d: MISSAO %d IMPOSSIVEL\n", m->Relogio, e->dado1);
    insere_lef(lef, cria_evento(e->tempo + (24 * 60), 2, e->dado1, e->dado2 + 1));

    return 0;
}

void espera(struct lef_t *lef, struct evento_t *e, struct mundo *m){
    int t = m->Relogio;
    int h = e->dado1;
    int b = e->dado2;
    int fila = fila_tamanho(m->Bases[b].esp);

    printf("%6d: ESPERA  HEROI %2d BASE %d (%2d)\n", t, h, b, fila);

    if(enqueue(m->Bases[e->dado2].esp, e->dado1)){
        insere_lef(lef, cria_evento(e->tempo, 5, e->dado1, e->dado2));
    }

    return;
}

/* verifica se o a card do conjunto já atingiu o seu max */
/* retorna 1 se sim; 0 se nao */
int conj_cheio(struct conjunto *aux){
    if(cardinalidade_cjt(aux) < aux->max){
        return 0;
    }

    return 1;
}

struct evento_t *desiste(struct evento_t *e, struct mundo *m){
    int t = m->Relogio;
    int h = e->dado1;
    int b = e->dado2;
    int aux = rand() % m->Nbases;
    printf("%6d: DESIST  HEROI %2d BASE %d\n", t, h, b);

    return (cria_evento(t, 6, h, aux));
}

void avisa(struct evento_t *e, struct lef_t *lef, struct mundo *m){
    int t = m->Relogio;
    int b = e->dado2;
    int entrar; 
    int card = cardinalidade_cjt(m->Bases[b].pres);
    int max = m->Bases[b].lot;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", t, b, card, max);
    fila_imprime(m->Bases[b].esp);

    while((! conj_cheio(m->Bases[b].pres)) && (! fila_vazia(m->Bases[b].esp))){
        dequeue(m->Bases[b].esp, &entrar);
        insere_cjt(m->Bases[b].pres, entrar);
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", t, b, entrar);
        insere_lef(lef, cria_evento(t, 7, entrar, b));
    }

    return;
}  

struct evento_t *viaja(struct evento_t *e, struct mundo *m){
    int h = e->dado1;
    int b = e->dado2;
    int vel = m->Herois[h].vel;
    int agr = m->Herois[h].base;
    int d = dist(m->Bases[b].local, m->Bases[agr].local);
    int dur = d / vel;
    int t = m->Relogio + dur;

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", m->Relogio, h, agr, b, d, vel, t);


    return cria_evento(t, 1, h, b);
}

struct evento_t *entra(struct evento_t *e, struct mundo *m){
    int t = m->Relogio;
    int h = e->dado1;
    int b = e->dado2;
    int card = cardinalidade_cjt(m->Bases[b].pres);
    int max = m->Bases[b].lot;
    int tpb = 15 + m->Herois[h].pac * (rand() % 20) + 1; 

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h, b, card, max, t + tpb);


    return cria_evento(t + tpb, 8, h, b);
}

void sai(struct evento_t *e, struct lef_t *lef, struct mundo *m){
    int t = m->Relogio;
    int h = e->dado1;
    int b = e->dado2;
    int max = m->Bases[b].lot;
    retira_cjt(m->Bases[b].pres, h);
    int card = cardinalidade_cjt(m->Bases[b].pres);
    printf("%6d: SAI  HEROI %2d BASE %d (%2d/%2d)\n", t, h, b, card, max);

    int ir = rand() % N_BASES;
    insere_lef(lef, cria_evento(e->tempo, 6, h, ir));
    insere_lef(lef, cria_evento(e->tempo, 5, b, b));

    return;
}  

void imprime_chega(struct evento_t *e, struct mundo *m, int aux){
    int t = m->Relogio;
    int h = e->dado1;
    int b = e->dado2;
    int lotacao = m->Bases[b].lot;
    int card = cardinalidade_cjt(m->Bases[b].pres) + fila_tamanho(m->Bases[b].esp);
    if(aux == 1){
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", t, h, b, card, lotacao);
    }
    else{
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", t, h, b, card, lotacao);
    }

    return;
}

void fim(struct mundo *m, int win, int menor, int maior, int total){
    int i;
    for(i = 0; i < m->NHerois; i++){
        struct hero aux = m->Herois[i];
        printf("HEROI  %d  PAC  %d  VEL  %d  EXP  %d  HABS", aux.id, aux.pac, aux.vel, aux.exp);
        imprime_cjtAux(m->Herois[i].hab);
    }

    float cent = (float)win / total * 100;
    float med =  (float)total / win;
    printf("MISSOES CUMPRIDAS: %d/%d (%.2f%%)\n", win, total, cent);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.2f\n", menor, maior, med);

    return;
}

void free_mundo(struct mundo *m){
    int i;
    for (i = 0; i < m->NHerois; i++) {
        free(m->Herois[i].hab->v);
        free(m->Herois[i].hab);
    }
    free(m->Herois);

    for (i = 0; i < m->Nbases; i++){
        free(m->Bases[i].local);
        free(m->Bases[i].pres->v);
        free(m->Bases[i].pres);
        fila_destroi(&m->Bases[i].esp);
        free(m->Bases[i].esp);
    }
    free(m->Bases);

    for (i = 0; i < m->NMissoes; i++){
        free(m->Missoes[i].local);
        free(m->Missoes[i].hab->v);
        free(m->Missoes[i].hab);
    }
    free(m->Missoes);

    free(m->TamanhoMundo);
    free(m);

    return;
}

/* Simulação The Boys */
/* GRR20232376 */
/* O programa inicia um mundo com eventos em determinado temopo */
/* Os eventos são tratados até o tempo de fim */
int main (){
    srand (0);
    int aux;
    struct mundo *m = i_mundo();
    m->Herois = i_herois(m);
    m->Bases = i_bases(m);
    m->Missoes = i_missoes(m);
    struct lef_t *sim = criar_lef(m);
    struct evento_t *e = retira_lef(sim);
    struct evento_t *temp;
    m->Relogio = e->tempo;
    int tipo = e->tipo;


    /* variaveis para tratar do evento final */
    int win = 0;
    int maior = 0;
    int menor = 0;
    int total = 0;

    while (tipo != 0) {
    m->Relogio = e->tempo;
    switch (tipo) {
        case 1:
            aux = chega(sim, e, m);
            imprime_chega(e, m, aux);
            break;
        case 2:
            win = win + mis(e, sim, m);
            if(e->dado2 > maior){
                maior = e->dado2;
            }
            else{
                menor = e->dado2;
            }
            total++;
            break;
        case 3:
            espera(sim, e, m);
            break;
        case 4:
            insere_lef(sim, desiste(e, m));
            break;
        case 5:
            avisa(e, sim, m);
            break;
        case 6:
            insere_lef(sim, viaja(e, m));
            break;
        case 7:
            insere_lef(sim, entra(e, m));
            break;
        case 8:
            sai(e, sim, m);
            break;
        default:
            printf("Tipo desconhecido: %d\n", tipo);
            break;
    }

    temp = retira_lef(sim);
    destroi_evento(e);
    e = temp; 
    tipo = e->tipo;
}

    free(e);
    destroi_lef(sim);
    fim(m, win, menor, maior, total);
    free_mundo(m);
    
    return 0;
}
