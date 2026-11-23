#include "team.h"
#include "teamlist.h"
#include "match.h"
#include "matchlist.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Cria as listas vazias para guardar os times e as partidas
    TeamList *teams = tl_create();
    MatchList *matches = ml_create();
    char buffer[256]; // Guarda temporariamente cada linha lida dos CSVs
    
    // 1. Le os times cadastrados no times.csv
    FILE *f_times = fopen("times.csv", "rt");
    if (f_times) {
        fgets(buffer, sizeof(buffer), f_times); // Pula a primeira linha (cabecalho)
        int id;
        char name[50];
        while (fgets(buffer, sizeof(buffer), f_times)) {
            // Separa o ID e o nome ate a quebra de linha e joga na lista de times
            if (sscanf(buffer, "%d,%49[^\n]", &id, name) == 2) {
                tl_append(teams, team_create(id, name));
            }
        }
        fclose(f_times);
    }

    // 2. Le os resultados das partidas no partidas.csv
    FILE *f_partidas = fopen("partidas.csv", "rt");
    if (f_partidas) {
        fgets(buffer, sizeof(buffer), f_partidas); // Pula o cabecalho do arquivo
        int id, t1, t2, g1, g2;
        while (fgets(buffer, sizeof(buffer), f_partidas)) {
            // Le os 5 campos da linha (id do jogo, id dos times e placar)
            if (sscanf(buffer, "%d,%d,%d,%d,%d", &id, &t1, &t2, &g1, &g2) == 5) {
                ml_append(matches, match_create(id, t1, t2, g1, g2));
            }
        }
        fclose(f_partidas);
    }

    // 3. Passa por todas as partidas e atualiza os pontos e gols de cada time
    int total_matches = ml_size(matches);
    for (int i = 0; i < total_matches; i++) {
        Match *m = ml_get(matches, i);
        Team *team1 = tl_find_by_id(teams, match_t1(m));
        Team *team2 = tl_find_by_id(teams, match_t2(m));
        if (team1 && team2) {
            // Soma o resultado nos dois times (para o time 2, inverte gols feitos e sofridos)
            team_add_result(team1, match_g1(m), match_g2(m));
            team_add_result(team2, match_g2(m), match_g1(m));
        }
    }

    // 4. Monta uma nova lista com os times ja ordenados pelos criterios de desempate
    TeamList *sorted = tl_create();
    for (int i = 0; i < tl_size(teams); i++) {
        tl_insert_ordered(sorted, tl_get(teams, i));
    }

    // 5. Imprime a tabela no terminal e grava ao mesmo tempo no classificacao.csv
    printf("Pos ID Time            V  E  D  GM GS S  PG\n");
    FILE *f_classificacao = fopen("classificacao.csv", "w");
    if (f_classificacao) {
        fprintf(f_classificacao, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n");
    }

    for (int i = 0; i < tl_size(sorted); i++) {
        Team *t = tl_get(sorted, i);
        
        printf("%-3d %-2d %-15s %-2d %-2d %-2d %-2d %-2d %-2d %d\n",
               i + 1, team_id(t), team_name(t),
               team_wins(t), team_draws(t), team_losses(t),
               team_goals_scored(t), team_goals_conceded(t),
               team_goal_diff(t), team_points(t));
        if (f_classificacao) {
            fprintf(f_classificacao, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                    i + 1, team_id(t), team_name(t),
                    team_wins(t), team_draws(t), team_losses(t),
                    team_goals_scored(t), team_goals_conceded(t),
                    team_goal_diff(t), team_points(t));
        }
    }
    if (f_classificacao) fclose(f_classificacao);
    // 6. Exibe o resumo estatistico do campeonato
    printf("\n===== Resumo do campeonato =====\n");
    printf("Partidas disputadas: %d de 45\n", total_matches);
    if (total_matches > 0) {
        Team *leader = tl_get(sorted, 0);
        // Com 10 times em turno unico sao 45 jogos; se completou tudo, ja e campeao
        if (total_matches == 45) { 
            printf("Campeao: %s (%d pontos)\n", team_name(leader), team_points(leader));
        } else {
            printf("Lider: %s (%d pontos)\n", team_name(leader), team_points(leader));
        }
        // Comeca pegando o primeiro da tabela como referencia para comparar ataque e defesa
        Team *best_attack = tl_get(sorted, 0);
        Team *best_defense = tl_get(sorted, 0);
        for (int i = 1; i < tl_size(sorted); i++) {
            Team *t = tl_get(sorted, i);
            if (team_goals_scored(t) > team_goals_scored(best_attack)) best_attack = t;
            if (team_goals_conceded(t) < team_goals_conceded(best_defense)) best_defense = t;
        }
        printf("Melhor ataque: %s (%d gols marcados)\n", team_name(best_attack), team_goals_scored(best_attack));
        printf("Melhor defesa: %s (%d gols sofridos)\n", team_name(best_defense), team_goals_conceded(best_defense));

        // Procura a partida com a maior diferenca de gols (goleada)
        Match *biggest_win = ml_get(matches, 0);
        int g1_inicial = match_g1(biggest_win);
        int g2_inicial = match_g2(biggest_win);
        int max_diff = g1_inicial > g2_inicial ? (g1_inicial - g2_inicial) : (g2_inicial - g1_inicial);

        for (int i = 1; i < ml_size(matches); i++) {
            Match *m = ml_get(matches, i);
            int m_g1 = match_g1(m);
            int m_g2 = match_g2(m);   
            // Calcula a diferenca absoluta do placar da partida atual
            int diff = m_g1 > m_g2 ? (m_g1 - m_g2) : (m_g2 - m_g1);
            
            if (diff > max_diff) { 
                max_diff = diff;
                biggest_win = m;
            }
        }   
        // Busca os nomes dos dois times pelo ID para imprimir o placar da goleada
        Team *t1 = tl_find_by_id(teams, match_t1(biggest_win));
        Team *t2 = tl_find_by_id(teams, match_t2(biggest_win));
        printf("Maior goleada: %s %d x %d %s\n", team_name(t1), match_g1(biggest_win), match_g2(biggest_win), team_name(t2));
    }
    // 7. Libera toda a memoria alocada antes de fechar o programa
    tl_free(teams, 1);   // Passa 1 para apagar os nos e tambem os times
    tl_free(sorted, 0);  // Passa 0 para apagar so os nos (os times ja foram apagados acima)
    ml_free(matches);
    
    return 0;
}
