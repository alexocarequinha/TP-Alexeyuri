```markdown
# TP-Alex-Medeiros-de-Carvalho

**Trabalho de Programação: Fechando o Campeonato**  
**Alunos:
** Alex Medeiros de Carvalho (`20252bsi0410`)

Sistema de apuração do Campeonato Computacional de Futebol desenvolvido em **C**. O programa lê os dados dos times e os resultados das partidas a partir de arquivos CSV, atualiza as estatísticas de cada clube, ordena a classificação segundo os critérios do campeonato, exibe o relatório no terminal e salva a tabela final em arquivo.

---

## Estrutura do Repositório

```text
├── times.csv                 # Base de dados dos times inscritos (ID, Nome)
├── partidas.csv              # Resultados das partidas disputadas
├── classificacao.csv         # Tabela de classificação gerada na execução (saída)
├── Makefile                  # Automação de compilação, execução e limpeza
├── main.c                    # Leitura dos CSVs, apuração, ordenação e relatório
├── team.c / team.h           # TAD Team (estatísticas e cálculos do time)
├── teamlist.c / teamlist.h   # TAD TeamList (lista duplamente encadeada de times)
├── match.c / match.h         # TAD Match (dados de uma partida)
└── matchlist.c / matchlist.h # TAD MatchList (lista encadeada simples de partidas)

```

---

## Descrição dos Arquivos

* **`times.csv` e `partidas.csv`:** arquivos de entrada localizados no diretório principal do projeto.


* `times.csv`: formato `ID,Nome`.


* `partidas.csv`: formato `ID,Time1,Time2,GolsTime1,GolsTime2`.




* **`classificacao.csv`:** arquivo de saída gerado automaticamente pelo programa com a classificação final ordenada.


* **`Makefile`:** automatiza o build e a limpeza do projeto:


* `make` ou `make all`: compila os módulos e gera o executável.
* `make run`: compila (se necessário) e executa o programa.
* `make clean`: remove arquivos objeto (`.o`), o executável e `classificacao.csv`.


* **`main.c`:** carrega os arquivos CSV, cruza os IDs das partidas com os times cadastrados, monta a lista ordenada, imprime a tabela e o resumo do campeonato no terminal e grava `classificacao.csv`.


* **`team.c` / `team.h`:** implementa o TAD `Team` (criação, liberação, *getters*, registro de resultados e cálculo de saldo de gols e pontos).


* **`teamlist.c` / `teamlist.h`:** implementa o TAD `TeamList` em lista duplamente encadeada (inserção no final, inserção ordenada e liberação controlada por flag).


* **`match.c` / `match.h`:** implementa o TAD `Match` com os dados de cada partida (`id`, `t1`, `t2`, `g1`, `g2`).


* **`matchlist.c` / `matchlist.h`:** implementa o TAD `MatchList` em lista encadeada simples para armazenar as partidas lidas.



---

## Tipos Abstratos de Dados (TADs)

### 1. `Team` (`team.c` / `team.h`)

Representa um clube com encapsulamento da `struct` na implementação:

* `id` (`int`): identificador do time.


* `name` (`char*`): nome alocado dinamicamente.


* `wins` (`int`), `draws` (`int`), `losses` (`int`): vitórias, empates e derrotas.


* `goals_scored` (`int`) e `goals_conceded` (`int`): gols marcados (`GM`) e sofridos (`GS`).



> **Observação:** Saldo de gols (`S`) e pontos ganhos (`PG`) são calculados por funções a partir dos campos armazenados, evitando redundância de estado.
> 
> 

### 2. `TeamList` (`teamlist.c` / `teamlist.h`)

Lista duplamente encadeada para gerenciar a coleção de times:

* **Nó:** ponteiro para `Team`, `next` e `prev`.


* **Lista:** `head`, `tail` e `size`.



### 3. `Match` (`match.c` / `match.h`)

Encapsula os dados de uma partida (`id`, `t1`, `t2`, `g1`, `g2`).

### 4. `MatchList` (`matchlist.c` / `matchlist.h`)

Lista encadeada simples para armazenar as partidas lidas do arquivo de entrada.

---

## Como Compilar e Executar

### Pré-requisitos

* Compilador `gcc` e utilitário `make` (Linux, MSYS2/MinGW no Windows ou ambiente similar).


* Arquivos `times.csv` e `partidas.csv` presentes na mesma pasta do projeto.



### Comandos no terminal

```bash
# Compilar e executar
make run

# Apenas compilar
make

# Limpar arquivos gerados (.o, executável e classificacao.csv)
make clean

```

---

## Decisões de Implementação

* **Encapsulamento com TADs:** as definições internas das `struct`s ficam ocultas nos arquivos `.c`, expondo apenas ponteiros e operações nos cabeçalhos `.h`.


* **Listas encadeadas distintas:** lista duplamente encadeada para times (facilitando inserção ordenada) e lista encadeada simples para partidas.


* **Inserção ordenada (`tl_insert_ordered`):** os times são inseridos em uma nova lista já na posição correspondente aos critérios do campeonato.


* **Controle de posse de memória (`tl_free`):** como a lista original e a lista ordenada compartilham os mesmos ponteiros `Team*`, a flag `free_teams` controla quando liberar os structs `Team` e quando liberar apenas os nós da lista auxiliar, evitando *double-free* e vazamentos de memória.


* **Leitura de linhas com `fgets` e `sscanf`:** cada linha dos arquivos CSV é lida em buffer e interpretada com validação do retorno de `sscanf`.
