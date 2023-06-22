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

typedef struct no {
  Posicao posicao;
  int g;
  int h;
  int f;
  struct no *pai;
} No;

No no_padrao = {{-1, -1}, -1, -1, -1, NULL};

int distancia_manhattan(Posicao a, Posicao b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

int caminhavel(char **Tabuleiro, int x, int y, int n, No *lista_fechada) {
  // se x ou y forem negativos ou maior/igual a n, nao sao validos
  if (x < 0 || x >= n || y < 0 || y >= n) return 0;

  // se a posicao estiver na lista fechada, nao eh caminhavel
  for (int i = 0; i < n * n; i++) {
    if (lista_fechada[i].posicao.x == x && lista_fechada[i].posicao.y == y)
      return 0;
  }

  if (Tabuleiro[x][y] == '0')
    return 1;
  else
    return 0;
}

int compara_f(const void *a, const void *b) {
  const No *no_a = (const No *)a;
  const No *no_b = (const No *)b;

  // se f(a) e f(b) = -1 retorna 0
  if (no_a->f == -1 && no_b->f == -1) return 0;
  // se f(a) = -1 e f(b) != -1 entao f(a) > f(b), retorna 1
  else if (no_a->f == -1)
    return 1;
  // se f(a) != -1 e f(b) = -1 entao f(a) < f(b), retorna -1
  else if (no_b->f == -1)
    return -1;
  // se f(a) e f(b) != -1 entao compara normalmente
  else if (no_a->f < no_b->f)
    return -1;
  else if (no_a->f > no_b->f)
    return 1;
  else
    return 0;
}

int mesma_posicao(Posicao a, Posicao b) { return a.x == b.x && a.y == b.y; }

int encontra_menor_f_indice(No *lista_aberta, int n) {
  int menor = 0;
  for (int i = 0; i < n; i++) {
    if (lista_aberta[i].f == -1) return menor;
    if (lista_aberta[i].f < lista_aberta[menor].f) menor = i;
  }
  return menor;
}

No encontra_menor_f_no(No *lista_aberta, int n) {
  return lista_aberta[encontra_menor_f_indice(lista_aberta, n)];
}

void encontra_vizinhos(char **Tabuleiro, No *vizinhos, No no_atual, int n,
                       No *lista_fechada) {
  int x = no_atual.posicao.x;
  int y = no_atual.posicao.y;

  for (int i = 0; i < 4; i++) vizinhos[i] = no_padrao;

  if (caminhavel(Tabuleiro, x + 1, y, n, lista_fechada))
    vizinhos[0] = (No){{x + 1, y}, 0, 0, 0, &no_atual};

  if (caminhavel(Tabuleiro, x, y - 1, n, lista_fechada))
    vizinhos[1] = (No){{x, y - 1}, 0, 0, 0, &no_atual};

  if (caminhavel(Tabuleiro, x - 1, y, n, lista_fechada))
    vizinhos[2] = (No){{x - 1, y}, 0, 0, 0, &no_atual};

  if (caminhavel(Tabuleiro, x, y + 1, n, lista_fechada))
    vizinhos[3] = (No){{x, y + 1}, 0, 0, 0, &no_atual};
}

void printa_lista(No *lista, int n) {
  for (int i = 0; i < n; i++) {
    printf("No %d: (%d, %d) g: %d h: %d f: %d\n", i, lista[i].posicao.x,
           lista[i].posicao.y, lista[i].g, lista[i].h, lista[i].f);
  }
}

int encontra_caminho_exato(char **Tabuleiro, int n, char *caminho) {
  int tamanho = 0;
  int n_lista = n * n;

  Posicao posicao_inicial = {0, 0};
  Posicao posicao_final = {n - 1, n - 1};
  No no_inicial = {posicao_inicial, 0, 0, 0, NULL};
  No no_final = {posicao_final, 0, 0, 0, NULL};

  No lista_aberta[n_lista];
  No lista_fechada[n_lista];
  No vizinhos[4];
  No no_atual = no_padrao;
  No no_sucessor = no_padrao;

  // reseta a lista aberta e fechada
  for (int i = 0; i < n_lista; i++) {
    lista_aberta[i] = no_padrao;
    lista_fechada[i] = no_padrao;
  }

  lista_aberta[0] = no_inicial;

  for (int k = 0; k < n_lista; k++) {
    qsort(lista_aberta, n_lista, sizeof(No), compara_f);
    qsort(lista_fechada, n_lista, sizeof(No), compara_f);

    printf("\n\nLista ABERTA:\n");
    printa_lista(lista_aberta, n_lista);
    printf("\n\nLista FECHADA:\n");
    printa_lista(lista_fechada, n_lista);

    no_atual = lista_aberta[0];
    lista_aberta[0] = no_padrao;
    encontra_vizinhos(Tabuleiro, vizinhos, no_atual, n, lista_fechada);

    for (int i = 0; i < 4; i++) {
      no_sucessor = vizinhos[i];
      if (no_sucessor.f == -1) continue;

      if (mesma_posicao(no_sucessor.posicao, posicao_final)) {
        printf("oi");
      }

      no_sucessor.g = no_atual.g + 1;
      no_sucessor.h = distancia_manhattan(no_sucessor.posicao, posicao_final);
      no_sucessor.f = no_sucessor.g + no_sucessor.h;
      // no_sucessor.pai = &no_atual;

      for (int j = 0; j < n; j++) {
        if (lista_aberta[j].f == -1) {
          lista_aberta[n_lista - 1] = no_sucessor;
          break;
        }

        else if (mesma_posicao(lista_fechada[j].posicao, no_sucessor.posicao) &&
                 lista_fechada[j].f < no_sucessor.f)
          break;

        else if (mesma_posicao(lista_aberta[j].posicao, no_sucessor.posicao)) {
          if (lista_aberta[j].f > no_sucessor.f) {
            lista_aberta[n_lista - 1] = no_sucessor;
          }
          break;
        }
      }
    }
    lista_fechada[n_lista - 1] = no_atual;
    no_atual.pai = &lista_fechada[n_lista - 1];
  }

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
