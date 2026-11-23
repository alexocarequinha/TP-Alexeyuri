#include "matchlist.h"
#include <stdlib.h>

// No da lista encadeada simples (so aponta para a proxima partida)
struct mnode {
    Match *match;
    struct mnode *next;
};

// Estrutura da lista de partidas com ponteiro para o inicio e para o fim
struct matchlist {
    struct mnode *head;
    struct mnode *tail;
    int size;
};
// Cria uma lista de partidas vazia
MatchList *ml_create(void) {
    MatchList *ml = (MatchList *)malloc(sizeof(struct matchlist));
    if (ml != NULL) {
        ml->head = ml->tail = NULL;
        ml->size = 0;
    }
    return ml;
}
// Adiciona uma nova partida no final da lista usando o ponteiro tail
void ml_append(MatchList *ml, Match *m) {
    struct mnode *n = (struct mnode *)malloc(sizeof(struct mnode));
    if (!n) return;
    n->match = m;
    n->next = NULL;
    if (ml->tail) ml->tail->next = n;
    else ml->head = n;
    ml->tail = n;
    ml->size++;
}
int ml_size(MatchList *ml) { return ml->size; }

// Pega a partida da posicao 'i' percorrendo a lista desde o inicio
Match *ml_get(MatchList *ml, int i) {
    if (i < 0 || i >= ml->size) return NULL;
    struct mnode *curr = ml->head;
    for (int j = 0; j < i; j++) curr = curr->next;
    return curr->match;
}
// Percorre a lista apagando cada partida, depois o no e por ultimo a struct da lista
void ml_free(MatchList *ml) {
    if (ml != NULL) {
        struct mnode *curr = ml->head;
        while (curr) {
            struct mnode *next = curr->next; // Salva o proximo no para nao perder a referencia
            match_free(curr->match);
            free(curr);
            curr = next;
        }
        free(ml);
    }
}
