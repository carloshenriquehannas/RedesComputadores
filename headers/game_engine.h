#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

// estrutura de dados do jogador
typedef struct {
	int comp;            	// se eh um bot ou nao
	char simb;           	// caracter utilizado pelo jogador
} p_structure;

// estrutura de dados do jogo
typedef struct {
	int b_size;        	// tamanho do tabuleiro
	int s_size;        	// tamanho da sequencia de vitoria
	char **board;           // tabuleiro do jogo
	int p_num;              // numero max de jogadores
	p_structure *p_list;    // lista de jogadores conectados
	int g_ended; 		// estado atual do jogo (0 jogo esta acontecendo, 1 alguem ganhou, -1 empatou)
	int p_connected; 	// numeros de jogadores conectados
	int next_player;        // proximo a jogar
} g_structure;

// variavel que representa a ultima jogada a ser comunicada
typedef struct {
	int row;
	int col;
	char symb;
	int end;
	int next_player;
} lp;

// variavel para compartilhar informacoes basicas com o cliente
typedef struct{
	int player_id;
	char player_symbol;
	int b_size;
	int s_size;
} cbi;

// Funcao que testa se o jogador com o caracter 'x' venceu
int win(g_structure *game, char x);

// Funcao que procura a melhor jogada do computador. Procura jogar onde ha chance de vencer, assim como jogar onde ha chance de perder
int jogabot(g_structure *game, char x);

// Funcao que computa a chance de vitoria em uma jogada
int chances(g_structure *game, char x, int i, int j);

// Funcao para imprimir o tabuleiro na tela
int mostra(g_structure *game);

// Funcao que faz a jogada do usuario
int jogapessoa(g_structure *game, char x, int row, int col);

// Funcao caso nao haja um vencedor, i.e, partida deu velha
int empatou(g_structure *game);




#endif
