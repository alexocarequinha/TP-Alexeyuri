#ifndef MATCHLIST_H
#define MATCHLIST_H
#include "match.h"

// Lista simples para guardar as partidas lidas do arquivo
typedef struct matchlist MatchList;

// Criar a lista, adicionar partidas, consultar e apagar tudo no final
MatchList *ml_create(void);
void ml_append(MatchList *ml, Match *m);
int ml_size(MatchList *ml);
Match *ml_get(MatchList *ml, int i);
void ml_free(MatchList *ml);

#endif
