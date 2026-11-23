#ifndef MATCH_H
#define MATCH_H

// Struct que guarda os dados de uma partida (times e placar)
typedef struct match Match;

// Criar e apagar a partida
Match *match_create(int id, int t1, int t2, int g1, int g2);
void match_free(Match *m);

// Pegar o id da partida, os ids dos dois times e os gols de cada um
int match_id(Match *m);
int match_t1(Match *m);
int match_t2(Match *m);
int match_g1(Match *m);
int match_g2(Match *m);

#endif
