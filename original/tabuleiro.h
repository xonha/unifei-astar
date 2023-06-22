char ** aloca_tabuleiro(int n);

void libera_tabuleiro(char ** T, int n);

//Recebe um tabuleiro T, e cria uma copia
//devolvendo seu apontador
char ** copia_tabuleiro(char ** T, int n);

//Le o tabuleiro da entrada padrao
//devolve o tabuleiro e escreve em n_ o tamanho do 
//tabuleiro
char ** le_tabuleiro(int * n_);

void imprime_tabuleiro(char ** T, int n);

//calcula o dano recebido no ponto x,y
int calcula_dano(char ** T, int n, int x, int y);

int verifica_caminho(char ** T, int n, char * caminho, int tamanho_caminho, int * dano);

void desenha_caminho(char ** T, int n, char * caminho, int tamanho_caminho);

