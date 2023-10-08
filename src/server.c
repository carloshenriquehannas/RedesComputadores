#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<time.h>

#include "../headers/game_engine.h"
#include "../headers/server.h"

int server(){
	g_structure *game = malloc(sizeof(g_structure) * 1);
	
    	// Definição do menu de configuração inicial
    	printf("\n\nOPCOES DE JOGO\n\n");

    	// Configuração da dimensão do tabuleiro
	get_b_size(game);

    	// Criacao do tabuleiro com a respectiva dimensao e preenchimento dele com - (vazio)
	b_create(game);

    	// Configuração da sequência necessária para vencer
	get_b_size(game);

    	// Configuração da quantidade de jogadores
	get_p_num(game);

	// Cria os jogadores
	p_create(game);

	// Cria o socket do servidor
	


	return 0;
}

void get_b_size(g_structure *game){
    	printf("\nDIMENSAO DO TABULEIRO\n");
    	printf("Digite a dimensao N do tabuleiro. Exemplo: para N = 3, digite o numero 3. Obs: a dimensao deve ser maior que 2\n");
    	printf("Digite a dimensao do tabuleiro: ");
    	do{
        	scanf("%d", &(game->b_size));
        	if (game->b_size < 3){
            		printf("\nTamanho invalido. Digite novamente: ");
        	}
    	} while(game->b_size < 3); //Tratamento de erro para dimensao de tabuleiro
    	printf("\n-> Dimensao selecionada: %d x %d\n\n", game->b_size, game->b_size);

}

void get_s_size(g_structure *game){	
    	printf("\nSEQUENCIA DO VENCEDOR\n");
    	printf("Digite a sequencia  que define um vencedor. Exemplo: para P = 3, digite o numero 3.\n");
    	printf("Digite a sequencia que define o vencerdor: ");
    	do{
        	scanf("%d", &(game->s_size));
        	if( (game->s_size > game->b_size) || (game->s_size <= 1) ){
            		printf("Tamanho invalido. Digite novamente: ");
        	}
    	} while( (game->s_size <= 1) || (game->s_size > game->b_size) ); //Tratamento de erro para sequencia de um vencedor
    	printf("\n-> Tamanho de sequencia escolhido: %d\n\n", game->s_size);
}

void get_p_num(g_structure *game){
    	printf("\nQUANTIDADE DE JOGADORES\n");
    	printf("Digite a quantidade de jogadores. Exemplo: para 5 jogadores, digite o numero 5.\n");
    	printf("Digite o numero de jogadores que irao jogar: ");
    	do{
        	scanf("%d", &(game->p_num));
        	if(game->p_num <= 0){
            		printf("\nQuantidade invalida. Digite novamente: ");
        	}
    	} while(game->p_num <= 0); //Tratamento de erro para quantidade de jogares
    	printf("\n-> Numero de jogadores selecionados: %d\n\n", game->p_num);
}

void b_create(g_structure *game){
	game->board = malloc(sizeof(char *) * game->b_size);
	for(int i = 0; i < game->b_size; i++){
		game->board[i] = malloc(sizeof(char) * game->b_size);
	}
}

void p_create(g_structure *game){
	game->p_list = malloc(sizeof(p_structure) * game->p_num);	
}
