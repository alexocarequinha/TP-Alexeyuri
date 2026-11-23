#include "team.h"
#include <stdlib.h>
#include <string.h>

// Guarda as informacoes e estatisticas de cada time no campeonato
struct team {
    int id;
    char *name;
    int wins;
    int draws;
    int losses;
    int goals_scored;
    int goals_conceded;
};

// Aloca memoria para um novo time, copia o nome (+1 para o '\0') e zera os contadores
Team *team_create(int id, const char *name) {
    Team *t = (Team *)malloc(sizeof(struct team));
    if (t != NULL) {
        t->id = id;
        t->name = (char *)malloc(strlen(name) + 1);
        strcpy(t->name, name);
        t->wins = 0;
        t->draws = 0;
        t->losses = 0;
        t->goals_scored = 0;
        t->goals_conceded = 0;
    }
    return t;
}

// Libera primeiro a string do nome e depois a struct do time para nao vazar memoria
void team_free(Team *t) {
    if (t != NULL) {
        free(t->name);
        free(t);
    }
}

// Funcoes para ler os dados do time sem mexer direto na struct
int team_id(Team *t) { return t->id; }
const char *team_name(Team *t) { return t->name; }
int team_wins(Team *t) { return t->wins; }
int team_draws(Team *t) { return t->draws; }
int team_losses(Team *t) { return t->losses; }
int team_goals_scored(Team *t) { return t->goals_scored; }
int team_goals_conceded(Team *t) { return t->goals_conceded; }

// Saldo e pontos calculados na hora, não guardados na struct
int team_points(Team *t) { return 3 * t->wins + t->draws; }
int team_goal_diff(Team *t) { return t->goals_scored - t->goals_conceded; }

// Atualiza os gols marcados/sofridos e soma vitoria, empate ou derrota conforme o placar
void team_add_result(Team *t, int goals_scored, int goals_conceded) {
    t->goals_scored += goals_scored;
    t->goals_conceded += goals_conceded;
    if (goals_scored > goals_conceded) t->wins++;
    else if (goals_scored == goals_conceded) t->draws++;
    else t->losses++;
}
