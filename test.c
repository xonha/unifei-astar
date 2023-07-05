#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ################# INICIO SOLVER #####################
typedef struct {
  int x;
  int y;
} Posicao;
typedef struct no {
  int f;
  int g;
  Posicao posicao;
  struct no *parente;
} No;

// typedef struct no {
//   int pontos;
//   Posicao posicao;
//   struct no *parente;
// } No;

No REINICIAR_NO = {-1, -1, {-1, -1}, NULL};

void imprime_matriz(No **matriz, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matriz[j][i].f != -1) {
        // printar coordenadas
        printf("(%d,%d) ", j, i);
      } else {
        printf(" # ");  // Espaço vazio para valores f = -1
      }
    }
    printf("\n");
  }
  printf("\n");
}
int distancia_manhattan(Posicao a, Posicao b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

int caminhavel(char **Tabuleiro, No **matriz, Posicao posicao, int n) {
  // se x ou y forem negativos ou maior/igual a n, nao sao validos
  if (posicao.x < 0 || posicao.x >= n || posicao.y < 0 || posicao.y >= n)
    return 0;

  // se a posicao estiver na matriz fechada, nao eh caminhavel
  if (matriz[posicao.x][posicao.y].f != -1) return 0;

  if (Tabuleiro[posicao.x][posicao.y] == '0')
    return 1;
  else
    return 0;
}

void encontra_vizinhos(char **Tabuleiro, Posicao *vizinhos, Posicao posicao,
                       No **matriz, int n) {
  int x = posicao.x;
  int y = posicao.y;

  for (int i = 0; i < 4; i++) {
    vizinhos[i] = (Posicao){-1, -1};
  }

  if (caminhavel(Tabuleiro, matriz, (Posicao){x - 1, y}, n))
    vizinhos[0] = (Posicao){x - 1, y};

  if (caminhavel(Tabuleiro, matriz, (Posicao){x, y - 1}, n))
    vizinhos[1] = (Posicao){x, y - 1};

  if (caminhavel(Tabuleiro, matriz, (Posicao){x + 1, y}, n))
    vizinhos[2] = (Posicao){x + 1, y};

  if (caminhavel(Tabuleiro, matriz, (Posicao){x, y + 1}, n))
    vizinhos[3] = (Posicao){x, y + 1};
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

Posicao encontrar_menor_f(No **matriz, int n) {
  Posicao menor = {-1, -1};  // Inicializar com valores inválidos
  int menorF = INT_MAX;      // Inicializar com um valor grande

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matriz[i][j].f >= 0 && matriz[i][j].f < menorF) {
        menorF = matriz[i][j].f;
        menor = (Posicao){i, j};
      }
    }
  }

  return menor;
}

void preencher_parente_dos_vizinhos(Posicao *vizinhos, No **matriz, No atual) {
  for (int i = 0; i < 4; i++) {
    if (vizinhos[i].x == -1) continue;
    matriz[vizinhos[i].x][vizinhos[i].y].parente = &atual;
  }
}

char direcao(Posicao atual, Posicao parente) {
  if (atual.x == parente.x) {
    if (atual.y < parente.y)
      return 'N';
    else
      return 'S';
  } else {
    if (atual.x < parente.x)
      return 'O';
    else
      return 'L';
  }
}

void inverte_caminho(char *caminho) {
  if (caminho == NULL) {
    return;  // Verifica se o ponteiro é nulo para evitar erros
  }

  int tamanho = strlen(caminho);
  int inicio = 0;
  int fim = tamanho - 1;

  while (inicio < fim) {
    // Troca os caracteres
    char temp = caminho[inicio];
    caminho[inicio] = caminho[fim];
    caminho[fim] = temp;

    // Move os índices
    inicio++;
    fim--;
  }
}

int preenche_caminho(char *caminho, No **lista_fechada, No atual, int n) {
  int x = atual.posicao.x;
  int y = atual.posicao.y;
  int tamanho = 0;
  while (x != 0 || y != 0) {
    caminho[tamanho++] = direcao(atual.posicao, atual.parente->posicao);
    atual = *atual.parente;
    x = atual.posicao.x;
    y = atual.posicao.y;
  }
  inverte_caminho(caminho);
  return tamanho;
}

int encontra_caminho_exato(char **Tabuleiro, int n, char *caminho) {
  Posicao vizinhos[4];

  No **lista_aberta = malloc(n * sizeof(No *));
  No **lista_fechada = malloc(n * sizeof(No *));

  for (int i = 0; i < n; i++) {
    lista_aberta[i] = malloc(n * sizeof(No));
    lista_fechada[i] = malloc(n * sizeof(No));
    for (int j = 0; j < n; j++) {
      lista_aberta[i][j] = REINICIAR_NO;
      lista_fechada[i][j] = REINICIAR_NO;
    }
  }

  lista_aberta[0][0] = (No){0, 0, (Posicao){0, 0}, NULL};

  No atual = REINICIAR_NO;
  No proximo = REINICIAR_NO;

  while (1) {
    Posicao menor = encontrar_menor_f(lista_aberta, n);

    atual = lista_aberta[menor.x][menor.y];
    lista_aberta[menor.x][menor.y] = REINICIAR_NO;

    encontra_vizinhos(Tabuleiro, vizinhos, menor, lista_fechada, n);
    preencher_parente_dos_vizinhos(vizinhos, lista_aberta, atual);

    for (int i = 0; i < 4; i++) {
      int x = vizinhos[i].x;
      int y = vizinhos[i].y;
      if (x == -1 || y == -1) continue;
      if (proximo.posicao.x == n - 1 && proximo.posicao.y == n - 1) {
        return preenche_caminho(caminho, lista_fechada, proximo, n);
      }

      int h = distancia_manhattan(vizinhos[i], (Posicao){n - 1, n - 1});
      proximo.g = atual.g + 1;
      proximo.f = proximo.g + h;
      proximo.posicao = vizinhos[i];
      proximo.parente = &lista_fechada[menor.x][menor.y];

      if (lista_fechada[x][y].f != -1 && lista_fechada[x][y].f < proximo.f)
        continue;

      if (lista_aberta[x][y].f != -1 && lista_aberta[x][y].f < proximo.f)
        continue;

      if (lista_aberta[x][y].f != -1 && lista_aberta[x][y].f > proximo.f) {
        lista_aberta[x][y] = proximo;
        continue;
      }
      lista_aberta[x][y] = proximo;
    }

    // push Current on the CLOSED list
    lista_fechada[menor.x][menor.y] = atual;
  }
}

// ################# FIM SOLVER ########################