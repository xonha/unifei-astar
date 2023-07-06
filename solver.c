//  Esse sera o arquivo que voce vai submeter,
// aqui voce pode criar novas funcoes, structs
// a funcao encontra_caminho_exato que eh
// chamada pela main deve devolver o tamanho do
// caminho e preencher o caminho com as direcoes
// que devem ser tomadas.
//  O caminho jah estah alocado (porem a memoria
// nao eh limpa) e eh uma seq de caracteres (chars)
//'N', 'S', 'L', 'O' (de Oeste). O final do
// caminho pode ter qualquer caractere.

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabuleiro.h"

// ################# INICIO SOLVER #####################
typedef struct {
  int x;
  int y;
} Posicao;

void encontra_vizinhos_diagonal(char **T, Posicao no_atual, int n,
                                char *vizinhos) {
  // Resetar vizinhos com I de inválido
  for (int i = 0; i < 8; i++) vizinhos[i] = 'I';

  int x = no_atual.x;
  int y = no_atual.y;

  // Verificar os vizinhos em cima
  if (x - 1 >= 0) {
    vizinhos[3] = T[x - 1][y];
    if (y - 1 >= 0) vizinhos[0] = T[x - 1][y - 1];
    if (y + 1 < n) vizinhos[5] = T[x - 1][y + 1];
  }
  // Verificar vizinhos na mesma linha
  if (y - 1 >= 0) vizinhos[1] = T[x][y - 1];
  if (y + 1 < n) vizinhos[6] = T[x][y + 1];
  // Verificar vizinhos abaixo
  if (x + 1 < n) {
    vizinhos[4] = T[x + 1][y];
    if (y - 1 >= 0) vizinhos[2] = T[x + 1][y - 1];
    if (y + 1 < n) vizinhos[7] = T[x + 1][y + 1];
  }
}

void encontra_vizinhos(int n, int **T_int, int x, int y, Posicao vizinhos[3],
                       int visitados[n][n]) {
  int indice_vizinho = 0;
  if (y + 1 < n && visitados[x][y + 1] == 0 && T_int[x][y + 1] != 70 &&
      T_int[x][y + 1] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x, y + 1};

  if (x + 1 < n && visitados[x + 1][y] == 0 && T_int[x + 1][y] != 70 &&
      T_int[x + 1][y] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x + 1, y};

  if (x - 1 >= 0 && visitados[x - 1][y] == 0 && T_int[x - 1][y] != 70 &&
      T_int[x - 1][y] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x - 1, y};

  if (y - 1 >= 0 && visitados[x][y - 1] == 0 && T_int[x][y - 1] != 70 &&
      T_int[x][y - 1] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x, y - 1};
}

void monta_tabuleiro_inteiros(int n, int **T_int, char **T_char) {
  char vizinhos[8];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      char ponto = T_char[i][j];
      if (ponto == 70) {  // F - Fonte
        T_int[i][j] = 70;
        continue;
      }
      if (ponto == 84) {  // T - Torre
        T_int[i][j] = 84;
        continue;
      }
      encontra_vizinhos_diagonal(T_char, (Posicao){i, j}, n, vizinhos);
      T_int[i][j] = calcula_dano(T_char, n, i, j) / 10;
    }
  }
}

void imprime_tabuleiro_int(int n, int **tabuleiro) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) printf("%d ", tabuleiro[j][i]);
    printf("\n");
  }
}

int encontra_pontuacao_perfeita(int n, int **T_int) {
  int pontos = 0;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (T_int[i][j] < 0) pontos += T_int[i][j];

  return pontos;
}

void reseta_vizinhos(Posicao vizinhos[3]) {
  for (int i = 0; i < 3; i++) vizinhos[i] = (Posicao){0, 0};
}

int calcula_score(int n, int **T_int, Posicao caminho_atual[n * n]) {
  int score = 0;

  for (int i = 0; i < n * n; i++) {
    if (caminho_atual[i].x == -1) break;

    score += T_int[caminho_atual[i].x][caminho_atual[i].y];
  }
  score += T_int[0][0];
  return score;
}

int calcula_score_negativo(int n, int **T_int, Posicao caminho_atual[n * n]) {
  int score = 0;

  for (int i = 0; i < n * n; i++) {
    if (caminho_atual[i].x == -1) break;

    if (T_int[caminho_atual[i].x][caminho_atual[i].y] < 0)
      score += T_int[caminho_atual[i].x][caminho_atual[i].y];
  }

  if (T_int[0][0] < 0) score += T_int[0][0];
  return score;
}

void printa_caminho(int n, Posicao caminho[n * n]) {
  for (int i = 0; i < n * n; i++) {
    if (caminho[i].x == -1) break;
    printf("(%d, %d) ", caminho[i].x, caminho[i].y);
  }
  printf("\n");
}

void zera_caminho(int n, Posicao caminho_atual[n * n]) {
  for (int i = 0; i < n * n; i++) caminho_atual[i] = (Posicao){-1, -1};
}

void zera_visitados(int n, int visitados[n][n]) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) visitados[i][j] = 0;
}

int monta_caminho(int n, char *caminho, Posicao caminho_melhor[n * n]) {
  if (caminho_melhor[0].x == 1)
    caminho[0] = 'L';
  else if (caminho_melhor[0].x == 0)
    caminho[0] = 'S';

  for (int atual = 0; atual < n * n; atual++) {
    int proximo = atual + 1;

    if (caminho_melhor[atual].x == -1) return atual;

    if (caminho_melhor[atual].x == caminho_melhor[proximo].x &&
        caminho_melhor[atual].y < caminho_melhor[proximo].y)
      caminho[proximo] = 'S';

    if (caminho_melhor[atual].x == caminho_melhor[proximo].x &&
        caminho_melhor[atual].y > caminho_melhor[proximo].y)
      caminho[proximo] = 'N';

    if (caminho_melhor[atual].y == caminho_melhor[proximo].y &&
        caminho_melhor[atual].x < caminho_melhor[proximo].x)
      caminho[proximo] = 'L';

    if (caminho_melhor[atual].y == caminho_melhor[proximo].y &&
        caminho_melhor[atual].x > caminho_melhor[proximo].x)
      caminho[proximo] = 'O';
  }
  return 0;
}

void dfs(int n, int **T_int, int visitados[n][n], Posicao caminho_atual[n * n],
         int x, int y, int indice_caminho, int score_perfeito,
         Posicao caminho_melhor[n * n], int *score_melhor) {
  int score_atual = calcula_score(n, T_int, caminho_atual);
  int score_negativo = calcula_score_negativo(n, T_int, caminho_atual);
  int score_perfeito_restante = score_perfeito - score_negativo;

  if (score_atual + score_perfeito_restante > *score_melhor) return;

  if (x == n - 1 && y == n - 1) {       // Chegou no final
    if (score_atual < *score_melhor) {  // Achou um caminho melhor
      *score_melhor = score_atual;
      zera_caminho(n, caminho_melhor);
      for (int i = 0; i < n * n; i++)  // Atualiza o caminho melhor
        caminho_melhor[i] = caminho_atual[i];
    }
    return;
  }

  int aux_visistado[n][n];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      aux_visistado[i][j] = visitados[i][j];
    }

  Posicao vizinhos[3];
  reseta_vizinhos(vizinhos);
  encontra_vizinhos(n, T_int, x, y, vizinhos, visitados);

  aux_visistado[x][y] = 1;

  for (int i = 0; i < 3; i++) {
    if ((vizinhos[i].x == 0 && vizinhos[i].y == 0) ||  // Se vizinho invalido
        aux_visistado[vizinhos[i].x][vizinhos[i].y] == 1) {  // Ou já visitado
      continue;
    }
    caminho_atual[indice_caminho] = vizinhos[i];
    dfs(n, T_int, aux_visistado, caminho_atual, vizinhos[i].x, vizinhos[i].y,
        indice_caminho + 1, score_perfeito, caminho_melhor, score_melhor);
  }
  caminho_atual[indice_caminho] = (Posicao){-1, -1};
}

int encontra_caminho_exato(char **T_char, int n, char *caminho) {
  int score_perfeito = 0;
  int score_melhor = INT_MAX;

  int visitados[n][n];
  zera_visitados(n, visitados);

  Posicao caminho_atual[n * n];
  zera_caminho(n, caminho_atual);

  Posicao caminho_melhor[n * n];
  zera_caminho(n, caminho_melhor);

  int **T_int = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) T_int[i] = (int *)malloc(n * sizeof(int));
  monta_tabuleiro_inteiros(n, T_int, T_char);

  score_perfeito = encontra_pontuacao_perfeita(n, T_int);
  // imprime_tabuleiro_int(n, T_int);

  dfs(n, T_int, visitados, caminho_atual, 0, 0, 0, score_perfeito,
      caminho_melhor, &score_melhor);

  // printa_caminho(n, caminho_melhor);

  // printf("Melhor score: %d\n", tamanho_caminho);

  return monta_caminho(n, caminho, caminho_melhor);
}
// ################# FIM SOLVER ########################