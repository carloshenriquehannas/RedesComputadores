#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../headers/game_engine.h"

//Funcao que procura vencedor em linhas, colunas ou diagonais
int win(g_structure *game, char x){
	for(int i = 0;i < game->b_size; i++) {
            	int contL = 0;
            	int contC = 0;
            	for(int j = 0; j < game->b_size; j++){
                	if (game->board[i][j] == x){
                    		contL++;
                	}else{
                    		contL=0;
                	}

                	if (game->board[j][i]==x) {
                    		contC++;
                	} else{
                    		contC=0;
                	}

                	if(contC ==  game->s_size|| contL == game->s_size){
                    		return 1;
                	}
            	}
        }
        for(int i = 0; i < game->b_size-game->s_size+1; i++){
            for(int j = 0; j < game->b_size-game->s_size+1; j++){
                int contD1 = 0;
                for(int k = 0; k < game->s_size; k++){
                    if (game->board[i+k][j+k] ==x ){
                        contD1++;
                    } else{
                        break;
                    }
                }
                if (contD1 == game->s_size){
                    return 1;
                }
            }
        }
        for(int i = 0; i < game->b_size-game->s_size+1; i++){
            for(int j = game->s_size-1; j < game->b_size; j++){
                int contD2 = 0;
                for(int k = 0; k < game->s_size; k++){
                    if (game->board[i+k][j-k] == x){
                        contD2++;
                    } else{
                        break;
                    }
                }
                if (contD2 == game->s_size){
                    return 1;
                }
            }
        }
        return 0;
}

//Funcao que procura a melhor jogada do computador. Procura jogar onde ha chance de vencer, assim como jogar onde ha chance de perder

int jogabot(g_structure *game, char x){
	
	int mchances = 0;
        int l, c;

	if(game->b_size % 2 == 1 && game->board[game->b_size/2][game->b_size/2] == '-'){
        	game->board[game->b_size/2][game->b_size/2] = x;
        	return 0;
        }

        for(int i = 0;i < game->b_size; i++){
        	for(int j = 0; j < game->b_size; j++){
                	if(game->board[i][j] == '-'){
                    		game->board[i][j] = x;
                    		if(win(game, x)){
                        		return 0;
                    		} else{
                        		game->board[i][j] = '-';
                    		}
                    		for(int k = 0;k < game->p_num; k++){
                        		game->board[i][j] = game->p_list[k].simb;
                        		if(win(game, game->p_list[k].simb)){
                            			game->board[i][j] = x;
                            			return 0;
                        		} else{
                            			game->board[i][j]='-';
                        		}
                    		}

				int aux = 0;
                    		if((aux =  chances(game, x, i, j)) > mchances){
                        		mchances = aux;
                        		l = i;
                        		c = j;
                    		}
        		}
        	}
        	if (mchances > 0){
            		game->board[l][c] = x;
            		return 0;
        	}
        	for(int i = 0;i < game->b_size; i++){
            		for(int j = 0;j < game->b_size; j++){
                		if(game->board[i][j] == x){
                    			if(game->board[i-1][j-1] == x && game->board[i+1][j+1] == '-'){
                        			game->board[i+1][j+1] = x;
                        			return 0;
                    			}
                    			if(game->board[i+1][j+1] == x && game->board[i-1][j-1] == '-'){
                        			game->board[i-1][j-1] = x;
                        			return 0;
                    			}
                    			if(game->board[i+1][j] == x && game->board[i-1][j] == '-'){
                        			game->board[i-1][j] = x;
                        			return 0;
                    			}
                    			if(game->board[i-1][j] == x && game->board[i+1][j] == '-'){
                        			game->board[i+1][j]=x;
                        			return 0;
                    			}
                    			if(game->board[i-1][j+1] == x && game->board[i+1][j-1] == '-'){
                      		  		game->board[i+1][j-1] = x;
                        			return 0;
                    			}
                    			if(game->board[i+1][j-1] == x && game->board[i-1][j+1] == '-'){
                        			game->board[i-1][j+1] = x;
                        			return 0;
                    			}
                    			if(game->board[i][j+1] == x && game->board[i][j-1] == '-'){
                        			game->board[i][j-1] = x;
                        			return 0;
                    			}
                    			if(game->board[i][j-1] == x && game->board[i][j+1] == '-'){
                        			game->board[i][j+1] = x;
                        			return 0;
                    			}
                		}
            		}
        	}
        	for(int i = 0; i < game->b_size; i++){
            		for(int j = 0; j < game->b_size; j++){
                		if(game->board[i][j] == x){
                    			if(game->board[i][j+1] == '-'){
                        			game->board[i][j+1] = x;
                        			return 0;
                    			}
                    			if(game->board[i+1][j] == '-'){
                        			game->board[i+1][j] = x;
                        			return 0;
                    			}
                    			if(game->board[i+1][j+1] == '-'){
                        			game->board[i+1][j+1] = x;
                       				return 0;
                    			}
                    			if(game->board[i+1][j-1] == '-'){
                        			game->board[i+1][j-1] = x;
                        			return 0;
                    			}
                    			if(game->board[i-1][j+1] == '-'){
                        			game->board[i-1][j+1] = x;
                        			return 0;
                    			}
                    			if(game->board[i-1][j-1] == '-'){
                        			game->board[i-1][j-1] = x;
                        			return 0;
                  			}
                    			if(game->board[i][j-1] == '-'){
                        			game->board[i][j-1] = x;
                        			return 0;
                    			}
                    			if(game->board[i-1][j] == '-'){
                        			game->board[i-1][j] = x;
                        			return 0;
                    			}
                		}
            		}
        	}
        	for(int i = 0;i < game->b_size; i++){
            		for(int j = 0;j < game->b_size; j++){
                		if(game->board[i][j] == '-'){
                    			game->board[i][j] = x;
                    			return 0;
                		}
            		}
        	}
    	}

	return -1;
}

int chances(g_structure *game, char x, int i, int j){
	
	int nchances = 0;
        game->board[i][j] = x;
        
	for(int k = 0; k < game->b_size; k++){
        	for(int l = 0;l < game->b_size; l++){
                	if(game->board[k][l] == '-'){
                        	game->board[k][l] = x;
                        	if (win(game, x)){
                            		nchances++;
                        	}
                        game->board[k][l]='-';
                 	}
                }
        }

        game->board[i][j]='-';
        return nchances;
}

//Funcao para imprimir o tabuleiro na tela
int mostra(g_structure *game){
        printf("  %c",'A');
        for(int i = 1; i < game->b_size; i++){
            	printf(" %c", 65+i);
        }
        printf("\n");
        for(int i = 0; i < game->b_size; i++){
            	printf("%d ", i+1);
            	for(int j = 0; j < game->b_size; j++){
                	printf("%c ",game->board[i][j]);
            	}
            	printf("\n");
        }
}

//Funcao que faz a jogada do usuario
int jogapessoa(g_structure *game, char x){
        
	int l = 5;
        char c;

        printf("Digite a letra da coluna, em seguida o numero da linha.\n");
        printf("Digite aqui: ");
        do {
		scanf("\n%c%d", &c,&l); //Coluna em que vai jogar
            	c = toupper(c); //Letra que representa a coluna em maiusculo, independente da entrada
            	if(c < 'A' || c > 'A' + (game->b_size - 1) || l > game->b_size){
                	printf("Posicao %c,%d invalida. Jogue novamente: ", c,l);
	    	} else if (game->board[l - 1][c - 'A'] == '-') {
                	game->board[l - 1][c - 'A'] = x;
                	return 0;
            	}
        } while(l < 'A' || l > 'A' + (x - 1) || c > x);
        
	return 1;
}

//Funcao caso nao haja um vencedor, i.e, partida deu velha
int empatou(g_structure *game){
	for(int i = 0; i < game->b_size; i++){
            	for(int j = 0; j < game->b_size; j++){
                	if(game->board[i][j] == '-'){
                    		return 0;
                	}
            	}
        }
        return 1;
}

