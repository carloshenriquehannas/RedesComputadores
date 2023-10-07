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
	char **board;
	int p_num;
	p_structure *p_list; 
	int g_status; 		// estado atual do jogo (0 jogo esta acontecendo, 1 alguem ganhou, -1 empatou)
} g_structure;

// Funcao que testa se o jogador com o caracter 'x' venceu
int win(g_structure *game, char x);

// Funcao que procura a melhor jogada do computador. Procura jogar onde ha chance de vencer, assim como jogar onde ha chance de perder
int jogabot(g_structure *game, char x);

// Funcao que computa a chance de vitoria em uma jogada
int chances(g_structure *game, char x, int i, int j);

// Funcao para imprimir o tabuleiro na tela
int mostra(g_structure *game);

// Funcao que faz a jogada do usuario
int jogapessoa(g_structure *game, char x);

// Funcao caso nao haja um vencedor, i.e, partida deu velha
int empatou(g_structure *game);




#endif
