#include "teamlist.h"
#include <stdlib.h>

// No da lista duplamente encadeada (aponta para o proximo e para o anterior)
struct node {
    Team *team;
    struct node *next;
    struct node *prev;
};
// Estrutura principal da lista, guardando o inicio, o fim e o tamanho
struct teamlist {
    struct node *head;
    struct node *tail;
    int size;
};
// Cria uma lista de times vazia
TeamList *tl_create(void) {
    TeamList *tl = (TeamList *)malloc(sizeof(struct teamlist));
    if (tl != NULL) {
        tl->head = tl->tail = NULL;
        tl->size = 0;
    }
    return tl;
}

// Adiciona um time no final da lista (usado na leitura inicial do times.csv)
void tl_append(TeamList *tl, Team *t) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (!n) return;
    n->team = t;
    n->next = NULL;
    n->prev = tl->tail;
    // Se ja tiver alguem no fim, liga nele; senao, esse no vira o primeiro (head)
    if (tl->tail) tl->tail->next = n;
    else tl->head = n;
    tl->tail = n;
    tl->size++;
}

// Compara equipas de acordo com os 5 critérios de desempate indicados
// Retorna negativo se t1 fica na frente, positivo se t2 fica na frente
static int compare_teams(Team *t1, Team *t2) {
    if (team_points(t1) != team_points(t2)) return team_points(t2) - team_points(t1);
    if (team_wins(t1) != team_wins(t2)) return team_wins(t2) - team_wins(t1);
    if (team_goal_diff(t1) != team_goal_diff(t2)) return team_goal_diff(t2) - team_goal_diff(t1);
    if (team_goals_scored(t1) != team_goals_scored(t2)) return team_goals_scored(t2) - team_goals_scored(t1);
    return team_id(t1) - team_id(t2); // Menor ID ganha
}
// Insere o time ja na posicao certa da tabela usando os criterios de desempate
void tl_insert_ordered(TeamList *tl, Team *t) {
    struct node *n = (struct node *)malloc(sizeof(struct node));
    n->team = t;
    n->next = n->prev = NULL;
    // Caso 1: lista ainda esta vazia, o novo no vira inicio e fim
    if (!tl->head) {
        tl->head = tl->tail = n;
        tl->size++;
        return;
    }
    // Anda na lista enquanto o time atual (curr) estiver na frente do novo time (t)
    struct node *curr = tl->head;
    while (curr && compare_teams(t, curr->team) > 0) {
        curr = curr->next;
    }
    // Encaixa o novo no na posicao encontrada (fim, inicio ou entre dois nos)
    if (!curr) { // Inserir no fim
        n->prev = tl->tail;
        tl->tail->next = n;
        tl->tail = n;
    } else if (curr == tl->head) { // Inserir no início
        n->next = tl->head;
        tl->head->prev = n;
        tl->head = n;
    } else { // Inserir no meio
        n->prev = curr->prev;
        n->next = curr;
        curr->prev->next = n;
        curr->prev = n;
    }
    tl->size++;
}

// Procura um time pelo ID percorrendo a lista do comeco ao fim
Team *tl_find_by_id(TeamList *tl, int id) {
    for (struct node *curr = tl->head; curr; curr = curr->next) {
        if (team_id(curr->team) == id) return curr->team;
    }
    return NULL;
}

int tl_size(TeamList *tl) { return tl->size; }

// Pega o time da posicao 'i' andando 'i' passos a partir do inicio (igual indice de vetor)
Team *tl_get(TeamList *tl, int i) {
    if (i < 0 || i >= tl->size) return NULL;
    struct node *curr = tl->head;
    for (int j = 0; j < i; j++) curr = curr->next;
    return curr->team;
}
// flag 'free_teams' previne o double-free ao apagar uma lista que partilha ponteiros
void tl_free(TeamList *tl, int free_teams) {
    if (tl != NULL) {
        struct node *curr = tl->head;
        while (curr) {
            struct node *next = curr->next; // Guarda o proximo antes de apagar o atual
            if (free_teams) team_free(curr->team);
            free(curr);
            curr = next;
        }
        free(tl);
    }
}
