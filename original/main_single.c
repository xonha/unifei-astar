#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
// -----------------------------------
// Voce deve implementar essa funcao
// -----------------------------------
int encontra_caminho_exato(char **T, int n, char *caminho) {
  int tamanho;
  return tamanho;
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

  // imprime_tabuleiro(T_original, n);

  // o caminho tera no maximo n*n de tamanho, por isso eh prealocado aqui
  char *caminho = (char *)malloc(n * n * sizeof(char));

  // sua funcao devolve o tamanho do caminho e preenche o caminho com
  //'N', 'S', 'L', 'O'
  int tamanho_caminho = encontra_caminho_exato(T, n, caminho);

  // printf("tamanho_caminho = %d\n", tamanho_caminho);
  // for(int i = 0; i < tamanho_caminho; i++){
  //   printf("%c", caminho[i]);
  // }

  int res = 0;
  int dano = INT_MAX;

  if (tamanho_caminho > 0 &&
      verifica_caminho(T_original, n, caminho, tamanho_caminho, &dano)) {
    printf("Tomou %d de dano! (quanto mais negativo melhor)\n", dano);
    // desenha_caminho(T_original, n, caminho, tamanho_caminho);
    // imprime_tabuleiro(T_original, n);
  } else {
    printf("Algo errado no seu caminho!\n");
    // desenha_caminho(T_original, n, caminho, tamanho_caminho);
    // imprime_tabuleiro(T_original, n);
    res = -1;
  }

  libera_tabuleiro(T, n);
  libera_tabuleiro(T_original, n);
  free(caminho);
  return res;
}
