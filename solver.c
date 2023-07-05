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

typedef struct no {
  int x;
  int y;
  struct no *proximo;
} No;

void encontra_vizinhos_diagonal(char **T, Posicao no_atual, int n,
                                char *vizinhos) {
  // Resetar vizinhos com I de inválido
  for (int i = 0; i < 8; i++) {
    vizinhos[i] = 'I';
  }

  int x = no_atual.x;
  int y = no_atual.y;

  // Verificar os vizinhos em cima
  if (x - 1 >= 0) {
    if (y - 1 >= 0) {
      vizinhos[0] = T[x - 1][y - 1];
    }
    vizinhos[3] = T[x - 1][y];
    if (y + 1 < n) {
      vizinhos[5] = T[x - 1][y + 1];
    }
  }

  // Verificar vizinhos na mesma linha
  if (y - 1 >= 0) {
    vizinhos[1] = T[x][y - 1];
  }
  if (y + 1 < n) {
    vizinhos[6] = T[x][y + 1];
  }

  // Verificar vizinhos abaixo
  if (x + 1 < n) {
    if (y - 1 >= 0) {
      vizinhos[2] = T[x + 1][y - 1];
    }
    vizinhos[4] = T[x + 1][y];
    if (y + 1 < n) {
      vizinhos[7] = T[x + 1][y + 1];
    }
  }
}

void encontra_vizinhos(int n, int **T_int, int x, int y, Posicao vizinhos[3],
                       int **visitados) {
  int indice_vizinho = 0;
  if (x - 1 >= 0 && visitados[x - 1][y] == 0 && T_int[x - 1][y] != 70 &&
      T_int[x - 1][y] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x - 1, y};

  if (y - 1 >= 0 && visitados[x][y - 1] == 0 && T_int[x][y - 1] != 70 &&
      T_int[x][y - 1] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x, y - 1};

  if (x + 1 < n && visitados[x + 1][y] == 0 && T_int[x + 1][y] != 70 &&
      T_int[x + 1][y] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x + 1, y};

  if (y + 1 < n && visitados[x][y + 1] == 0 && T_int[x][y + 1] != 70 &&
      T_int[x][y + 1] != 84)
    vizinhos[indice_vizinho++] = (Posicao){x, y + 1};
}

void reescreve_tabuleiro_inteiros(int n, int **T_int, char **T_char) {
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

void imprime_tabuleiro_int(int n, int tabuleiro[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", tabuleiro[j][i]);
    }
    printf("\n");
  }
}

int encontra_menor_pontuacao(int n, int **T_int) {
  int pontos = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (T_int[i][j] < 0) {
        pontos += T_int[i][j];
      }
    }
  }
  return pontos;
}

void reseta_vizinhos(Posicao vizinhos[3]) {
  for (int i = 0; i < 3; i++) {
    vizinhos[i] = (Posicao){0, 0};
  }
}

int calcula_score(int n, int **T_int, Posicao caminho_atual[n * n]) {
  int score = 0;
  for (int i = 0; i < n * n; i++) {
    if (caminho_atual[i].x == -1) {
      break;
    }
    score += T_int[caminho_atual[i].x][caminho_atual[i].y];
  }
  score += T_int[0][0];
  return score;
}

void printa_caminho(int n, Posicao caminho[n * n]) {
  for (int i = 0; i < n * n; i++) {
    if (caminho[i].x == -1) {
      break;
    }
    printf("(%d, %d) ", caminho[i].x, caminho[i].y);
  }
  printf("\n");
}

void dfs(int n, int **T_int, int **visitados, Posicao caminho_atual[n * n],
         int x, int y, int indice_caminho, int score_perfeito,
         Posicao caminho_melhor[n * n], int *score_melhor) {
  if (x == n - 1 && y == n - 1) {  // Chegou no final
    int score_atual = calcula_score(n, T_int, caminho_atual);
    if (score_atual < *score_melhor) {
      *score_melhor = score_atual;
      for (int i = 0; i < n * n; i++) {
        caminho_melhor[i] = caminho_atual[i];
      }
    }
    printa_caminho(n, caminho_atual);
    printf("Score atual: %d\n", score_atual);
  }

  int **aux_visitados;
  memcpy(aux_visitados, visitados, sizeof(visitados));
  Posicao vizinhos[3];
  reseta_vizinhos(vizinhos);
  encontra_vizinhos(n, T_int, x, y, vizinhos, visitados);

  aux_visitados[x][y] = 1;

  for (int i = 0; i < 3; i++) {
    if ((vizinhos[i].x == 0 && vizinhos[i].y == 0) ||  // Se vizinho invalido
        aux_visitados[vizinhos[i].x][vizinhos[i].y] == 1) {  // Ou já visitado
      continue;
    }
    caminho_atual[indice_caminho] = vizinhos[i];
    dfs(n, T_int, aux_visitados, caminho_atual, vizinhos[i].x, vizinhos[i].y,
        indice_caminho + 1, score_perfeito, caminho_melhor, score_melhor);
  }
  caminho_atual[indice_caminho] = (Posicao){-1, -1};
}

void zera_visitados(int n, int **visitados) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      visitados[i][j] = 0;
    }
  }
}

void zera_caminho(int n, Posicao caminho_atual[n * n]) {
  for (int i = 0; i < n * n; i++) {
    caminho_atual[i] = (Posicao){-1, -1};
  }
}

int encontra_caminho_exato(char **T_char, int n, char *caminho) {
  int tamanho_caminho = 0;
  int score_perfeito = 0;
  int score_melhor = INT_MAX;

  Posicao caminho_atual[n * n];
  Posicao caminho_melhor[n * n];
  int **T_int = (int **)malloc(n * sizeof(int *));
  int **visitados = (int **)malloc(n * sizeof(int *));

  for (int i = 0; i < n; i++) {
    T_int[i] = (int *)malloc(n * sizeof(int));
    visitados[i] = (int *)malloc(n * sizeof(int));
  }

  zera_caminho(n, caminho_atual);
  zera_caminho(n, caminho_melhor);
  zera_visitados(n, visitados);
  reescreve_tabuleiro_inteiros(n, T_int, T_char);
  score_perfeito = encontra_menor_pontuacao(n, T_int);
  // imprime_tabuleiro_int(n, T_int);

  dfs(n, T_int, visitados, caminho_atual, 0, 0, 0, score_perfeito,
      caminho_melhor, &score_melhor);

  return tamanho_caminho;
}
// ################# FIM SOLVER ########################