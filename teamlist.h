#ifndef TEAMLIST_H
#define TEAMLIST_H
#include "team.h"

// Lista dupla encadeada para guardar os times
typedef struct teamlist TeamList;

// Cria a lista e inserir times (no final ou ja ordenado pelos criterios de desempate)
TeamList *tl_create(void);
void tl_append(TeamList *tl, Team *t);
void tl_insert_ordered(TeamList *tl, Team *t);

// Buscar e acessar os times na lista
Team *tl_find_by_id(TeamList *tl, int id);
int tl_size(TeamList *tl);
Team *tl_get(TeamList *tl, int i);

// Apaga a lista. Se free_teams for 1 apaga os times junto, se for 0 apaga so os nos da lista
void tl_free(TeamList *tl, int free_teams);

#endif
