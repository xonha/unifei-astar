#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **aloca_tabuleiro(int n) {
  char **T = (char **)malloc(n * sizeof(char *));
  for (int x = 0; x < n; x++) {
    T[x] = (char *)malloc(n * sizeof(char *));
  }
  return T;
}

void libera_tabuleiro(char **T, int n) {
  for (int x = 0; x < n; x++) {
    free(T[x]);
  }
  free(T);
  return;
}

char **copia_tabuleiro(char **T, int n) {
  char **Tn = aloca_tabuleiro(n);
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      Tn[x][y] = T[x][y];
    }
  }
  return Tn;
}

char **le_tabuleiro(int *n_) {
  int n;
  scanf("%d\n", &n);
  *n_ = n;
  char **T = aloca_tabuleiro(n);

  // Tabela alocada
  for (int y = 0; y < n; y++) {
    char c;
    for (int x = 0; x < n; x++) {
      scanf("%c", &c);
      T[x][y] = c;
    }
    // lenda a quebra de linha
    scanf("%c", &c);
  }
  return T;
}

void imprime_tabuleiro(char **T, int n) {
  printf("%d\n", n);
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      printf("%c", T[x][y]);
    }
    printf("\n");
  }
}

int calcula_dano(char **T, int n, int x, int y) {
  // verificando as 8 casas adjacentes Fonte e Torre
  int dano = 0;
  if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < n && y - 1 < n &&
      T[x - 1][y - 1] == 'F')
    dano -= 10;
  if (x >= 0 && y - 1 >= 0 && x < n && y - 1 < n && T[x][y - 1] == 'F')
    dano -= 10;
  if (x + 1 >= 0 && y - 1 >= 0 && x + 1 < n && y - 1 < n &&
      T[x + 1][y - 1] == 'F')
    dano -= 10;
  if (x - 1 >= 0 && y >= 0 && x - 1 < n && y < n && T[x - 1][y] == 'F')
    dano -= 10;
  if (x + 1 >= 0 && y >= 0 && x + 1 < n && y < n && T[x + 1][y] == 'F')
    dano -= 10;
  if (x - 1 >= 0 && y + 1 >= 0 && x - 1 < n && y + 1 < n &&
      T[x - 1][y + 1] == 'F')
    dano -= 10;
  if (x >= 0 && y + 1 >= 0 && x < n && y + 1 < n && T[x][y + 1] == 'F')
    dano -= 10;
  if (x + 1 >= 0 && y + 1 >= 0 && x + 1 < n && y + 1 < n &&
      T[x + 1][y + 1] == 'F')
    dano -= 10;

  if (x - 1 >= 0 && y - 1 >= 0 && x - 1 < n && y - 1 < n &&
      T[x - 1][y - 1] == 'T')
    dano += 10;
  if (x >= 0 && y - 1 >= 0 && x < n && y - 1 < n && T[x][y - 1] == 'T')
    dano += 10;
  if (x + 1 >= 0 && y - 1 >= 0 && x + 1 < n && y - 1 < n &&
      T[x + 1][y - 1] == 'T')
    dano += 10;
  if (x - 1 >= 0 && y >= 0 && x - 1 < n && y < n && T[x - 1][y] == 'T')
    dano += 10;
  if (x + 1 >= 0 && y >= 0 && x + 1 < n && y < n && T[x + 1][y] == 'T')
    dano += 10;
  if (x - 1 >= 0 && y + 1 >= 0 && x - 1 < n && y + 1 < n &&
      T[x - 1][y + 1] == 'T')
    dano += 10;
  if (x >= 0 && y + 1 >= 0 && x < n && y + 1 < n && T[x][y + 1] == 'T')
    dano += 10;
  if (x + 1 >= 0 && y + 1 >= 0 && x + 1 < n && y + 1 < n &&
      T[x + 1][y + 1] == 'T')
    dano += 10;

  return dano;
}

int verifica_caminho_aux(char **T, int n, char *caminho, int tamanho_caminho,
                         char **visitado, int *dano_) {
  int x = 0;
  int y = 0;

  int dano = calcula_dano(T, n, x, y);
  visitado[x][y] = '1';

  for (int i = 0; i < tamanho_caminho; i++) {
    switch (caminho[i]) {
      case 'N':
        y--;
        break;
      case 'S':
        y++;
        break;
      case 'L':
        x++;
        break;
      case 'O':
        x--;
        break;
      default:
        printf("Seu caminho tem uma direcao nao reconhecida!\n");
        return 0;
    }
    if (T[x][y] == 'F' || T[x][y] == 'T') {
      printf("Seu caminho bateu em uma fonte ou torre em [%d][%d]!\n", x, y);
      return 0;
    }
    if (visitado[x][y] == '1') {
      printf("Seu caminho passa por uma casa repetida!\n");
      return 0;
    }
    visitado[x][y] = '1';
    dano += calcula_dano(T, n, x, y);
  }

  if (x != n - 1 || y != n - 1) {
    printf("Seu caminho nao chega no final, parou em [%d][%d]\n", x, y);
    return 0;
  }

  // printf("Tomou %d de dano!\n", dano);
  *dano_ = dano;
  return 1;
}

int verifica_caminho(char **T, int n, char *caminho, int tamanho_caminho,
                     int *dano) {
  // alocando um tabuleiro auxiliar para verificar se o caminho nao passa
  // duas vezes no mesmo quadrado, sem mexer no T
  char **visitado = aloca_tabuleiro(n);
  for (int x = 0; x < n; x++) {
    for (int y = 0; y < 0; y++) {
      visitado[x][y] = '0';
    }
  }

  int res =
      verifica_caminho_aux(T, n, caminho, tamanho_caminho, visitado, dano);

  libera_tabuleiro(visitado, n);
  return res;
}

void desenha_caminho(char **T, int n, char *caminho, int tamanho_caminho) {
  int x = 0;
  int y = 0;

  for (int i = 0; i < tamanho_caminho; i++) {
    switch (caminho[i]) {
      case 'N':
        T[x][y] = '^';
        y--;
        break;
      case 'S':
        T[x][y] = 'v';
        y++;
        break;
      case 'L':
        T[x][y] = '>';
        x++;
        break;
      case 'O':
        T[x][y] = '<';
        x--;
        break;
      default:
        return;
    }
  }
}
// ################# FIM TABULEIRO #####################
// ################# INICIO SOLVER #####################
typedef struct {
  int x;
  int y;
} Posicao;

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
    for (int j = 0; j < n; j++) {
      printf("%d ", tabuleiro[j][i]);
    }
    printf("\n");
  }
}

int encontra_pontuacao_perfeita(int n, int **T_int) {
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
  for (int i = 0; i < 3; i++) vizinhos[i] = (Posicao){0, 0};
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

int calcula_score_negativo(int n, int **T_int, Posicao caminho_atual[n * n]) {
  int score = 0;
  for (int i = 0; i < n * n; i++) {
    if (caminho_atual[i].x == -1) {
      break;
    }
    if (T_int[caminho_atual[i].x][caminho_atual[i].y] < 0) {
      score += T_int[caminho_atual[i].x][caminho_atual[i].y];
    }
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
  for (int i = 0; i < n * n; i++) {
    caminho_atual[i] = (Posicao){-1, -1};
  }
}

void zera_visitados(int n, int visitados[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      visitados[i][j] = 0;
    }
  }
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
// ################# INICIO MAIN #######################
int main() {
  // T eh o Tabuleiro, que serah passado para a sua funcao, voce pode
  // modificar T se precisar, T_original eh guardado inalterado

  char **T;
  char **T_original;
  int n;

  T = le_tabuleiro(&n);
  T_original = copia_tabuleiro(T, n);

  imprime_tabuleiro(T_original, n);

  // o caminho tera no maximo n*n de tamanho, por isso eh prealocado aqui
  char *caminho = (char *)malloc(n * n * sizeof(char));

  // sua funcao devolve o tamanho do caminho e preenche o caminho com
  //'N', 'S', 'L', 'O'
  int tamanho_caminho = encontra_caminho_exato(T, n, caminho);

  printf("tamanho_caminho = %d\n", tamanho_caminho);
  for (int i = 0; i < tamanho_caminho; i++) {
    printf("%c", caminho[i]);
  }

  int res = 0;
  int dano = INT_MAX;

  if (tamanho_caminho > 0 &&
      verifica_caminho(T_original, n, caminho, tamanho_caminho, &dano)) {
    printf("Tomou %d de dano! (quanto mais negativo melhor)\n", dano);
    desenha_caminho(T_original, n, caminho, tamanho_caminho);
    // imprime_tabuleiro(T_original, n);
  } else {
    printf("Algo errado no seu caminho!\n");
    desenha_caminho(T_original, n, caminho, tamanho_caminho);
    // imprime_tabuleiro(T_original, n);
    res = -1;
  }

  libera_tabuleiro(T, n);
  libera_tabuleiro(T_original, n);
  free(caminho);
  return res;
}
