#ifndef TEAM_H
#define TEAM_H

// Struct do time (os campos ficam escondidos no team.c)
typedef struct team Team;

// Criar e apagar o time
Team *team_create(int id, const char *name);
void team_free(Team *t);

// Funcoes para pegar os dados do time
int team_id(Team *t);
const char *team_name(Team *t);
int team_wins(Team *t);
int team_draws(Team *t);
int team_losses(Team *t);
int team_goals_scored(Team *t);
int team_goals_conceded(Team *t);

// Funções novas para calcular resultados e saldo
int team_points(Team *t);
int team_goal_diff(Team *t);
// Soma os gols e atualiza vitoria, empate ou derrota depois de uma partida
void team_add_result(Team *t, int goals_scored, int goals_conceded);

#endif
