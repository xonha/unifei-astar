//  Voce pode submetera esse codigo
// entao alteracoes aqui nao terao efeitos
// nao adianta modificar ela.

#include <stdio.h>
#include <stdlib.h>

#include "limits.h"
#include "solver.h"
#include "tabuleiro.h"

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
