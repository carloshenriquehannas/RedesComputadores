#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../headers/game_engine.h"

//Funcao que procura vencedor em linhas, colunas ou diagonais
int win(g_structure *game, char x){ 
	for(int i = 0;i < game->b_size; i++) { //Percorre linha
            	int contL = 0; //Variavel auxiliar para contabilizacao de vitoria em linha
            	int contC = 0; //Variavel auxiliar para contabilizacao de vitoria em coluna
            	for(int j = 0; j < game->b_size; j++){ //Percorre coluna
                	if (game->board[i][j] == x){ //Verifica se na posicao [i][j] ha uma jogada
                    		contL++; //Caso positivo, incrementa a variavel auxiliar de linha
                	}else{
                    		contL=0;
                	}

                	if (game->board[j][i]==x) { //Verifica se na posicao [j][i] ha uma jogada
                    		contC++; //Caso positivo, incrementa a variavel auxiliar de coluna
                	} else{
                    		contC=0;
                	}

                	if(contC ==  game->s_size|| contL == game->s_size){ //Verifica se as variaveis auxiliares sao iguais a sequencia que define um vencedor
                    		return 1; //Caso positivo, houve vitoria por linha e/ou coluna
                	}
            	}
        }
	//Verifica-se as diagonais principais do tabuleiro, i.e, diagonais da esquerda para direita
        for(int i = 0; i < game->b_size-game->s_size+1; i++){
            for(int j = 0; j < game->b_size-game->s_size+1; j++){
                int contD1 = 0; //Variavel D1 auxiliar de diagonais principais
                for(int k = 0; k < game->s_size; k++){
                    if (game->board[i+k][j+k] ==x ){ //Verifica se na posicao [i+k][j+k] ha uma jogada
                        contD1++; //Caso positivo, incrementa-se a variavel D1
                    } else{ 
                        break;
                    }
                }
                if (contD1 == game->s_size){ //Verificacao se D1 possui o mesmo valor da sequencia de vencedor
                    return 1; //Caso positivo, houve vencedor em diagonais principais
                }
            }
        }
	//Verifica-se as diagonais secundarias do tabuleiro, i.e, diagonais da direita para esquerda
        for(int i = 0; i < game->b_size-game->s_size+1; i++){
            for(int j = game->s_size-1; j < game->b_size; j++){
                int contD2 = 0; //Variavel auxilizar D2 de diagonais secundarias
                for(int k = 0; k < game->s_size; k++){
                    if (game->board[i+k][j-k] == x){ //Verifica se na posicao [i+k][j-k] ha uma jogada
                        contD2++; //Caso positivo, incrementa-se a variavel D2
                    } else{
                        break;
                    }
                }
                if (contD2 == game->s_size){ //verificacao se D2 possui o mesmo valor de sequencia de vencedor
                    return 1; //Caso positivo, houve vencedor em diagonais secundarias
                }
            }
        }
        return 0; //Caso em que ainda nao houve vencedor da partida
}

//Funcao que procura a melhor jogada do computador. Procura jogar onde ha chance de vencer, assim como jogar onde ha chance de perder

int jogabot(g_structure *game, char x){
	
	int mchances = 0;
        int l, c;

	if(game->b_size % 2 == 1 && game->board[game->b_size/2][game->b_size/2] == '-'){ //Verifica se o tabuleiro possui dimensao impar e se a casa central esta vazia
        	game->board[game->b_size/2][game->b_size/2] = x; //Caso positivo a jogada acontece na celula central do tabuleiro
        	return 0;
        }

        for(int i = 0;i < game->b_size; i++){ //Percorre linhas
        	for(int j = 0; j < game->b_size; j++){ //Percorre colunas
                	if(game->board[i][j] == '-'){ //Verifica se a posicao esta vazia para jogar
                    		game->board[i][j] = x; //Caso positivo, faz-se uma jogada temporaria la
                    		if(win(game, x)){ //Verifica se ganhou com a ultima jogada
                        		return 0; //Caso positivo, a jogada acontece la e o jogo possui um vencedor
                    		} else{
                        		game->board[i][j] = '-'; //Caso contrario, a jogada nao eh de fato realizada e a celula esta vazia
                    		}
				//Naquela celula em que ele jogou, verifica se algum jogador pode ganhar jogando ali
                    		for(int k = 0;k < game->p_num; k++){ //Iteracao por todos os jogadores
                        		game->board[i][j] = game->p_list[k].simb;
                        		if(win(game, game->p_list[k].simb)){ //Verificacao se o adversario pode ganhar naquela posicao
                            			game->board[i][j] = x; //Caso positivo, o bot joga ali para bloquear a vitoria do adversario
                            			return 0;
                        		} else{
                            			game->board[i][j]='-'; //Caso contrario, a celula continua marcada como vazia
                        		}
                    		}
				//Agora verifica-se a probabilidade de vitoria caso o bot jogue naquela casa
				int aux = 0;
                    		if((aux =  chances(game, x, i, j)) > mchances){ //Calculo de chances de vitoria
                        		mchances = aux; //Armazena o numero de chances em mchances
                        		l = i; //Armazena a linha
                        		c = j; //Armazena a coluna
                    		}
        		}
        	}
        	if (mchances > 0){ //Verifica se ha alguma chance de ganhar jogando na posicao [l][c]
            		game->board[l][c] = x; //Caso positivo, faz-se a jogada ali
            		return 0;
        	}
		//Caso em que ainda nao houve uma jogada promissora. Iteracao pelo tabuleiro e joga numa posicao adjacente ao seu respectivo simbolo
        	for(int i = 0;i < game->b_size; i++){ //Percorre linhas
            		for(int j = 0;j < game->b_size; j++){ //Percorre colunas
                		if(game->board[i][j] == x){ //Verifica se a posicao [i][j] possui uma jogada do bot
				//Caso positivo, verifica-se onde ha celulas vazias para realizar a jogada em uma posicao estrategica
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
        	for(int i = 0; i < game->b_size; i++){ //Percorre linhas
            		for(int j = 0; j < game->b_size; j++){ //Percorre colunas
                		if(game->board[i][j] == x){ //Verifica se a posicao [i][j] possui uma jogada
				//Caso positivo, faz-se a jogada na celula adjacente
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
		//Caso as alternativas acima nao sejam feitas, o bot joga na primeira casa vazia em que encontrar
        	for(int i = 0;i < game->b_size; i++){ //Percorre linhas
            		for(int j = 0;j < game->b_size; j++){ //Percorre colunas
                		if(game->board[i][j] == '-'){ //Verificacao se ha casa vazia
                    			game->board[i][j] = x; //Caso positivo, faz-se a jogada naquela posicao
                    			return 0;
                		}
            		}
        	}
    	}

	return -1; //Caso em que houve erro para fazer a jogada
}

int chances(g_structure *game, char x, int i, int j){
	
	int nchances = 0; //Variavel auxiliar que verifica a quantidade de chances de vitoria
        game->board[i][j] = x; 
        
	for(int k = 0; k < game->b_size; k++){ //Percorre linhas
        	for(int l = 0;l < game->b_size; l++){ //Percorre colunas
                	if(game->board[k][l] == '-'){ //Verifica se posicao [k][l] esta vazia
                        	game->board[k][l] = x; //Caso positivo, faz-se uma jogada temporaria na celula
                        	if (win(game, x)){ //Verificacao se eh uma jogada com possivel vitoria
                            		nchances++; //Caso positivo, incrementa-se a variavel
                        	}
                        game->board[k][l]='-'; //A celula de jogada temporaria recebe indicacao inicial de casa vazia
                 	}
                }
        }

        game->board[i][j]='-';
        return nchances; //Retorna o numero de chances possivel de vitoria
}

//Funcao para imprimir o tabuleiro na tela
int mostra(g_structure *game){
        printf("  %c",'A'); //Print que marca a coluna A do tabuleiro
        for(int i = 1; i < game->b_size; i++){ 
            	printf(" %c", 65+i); //Print que marca as colunas B, C, D, ... do tabuleiro
        }
        printf("\n");
        for(int i = 0; i < game->b_size; i++){
            	printf("%d ", i+1); //Print das linhas 1, 2, 3, ... do tabuleiro
            	for(int j = 0; j < game->b_size; j++){
                	printf("%c ",game->board[i][j]); //Print da area de jogadas do tabuleiro
            	}
            	printf("\n");
        }
}

//Funcao que faz a jogada do usuario
int jogapessoa(g_structure *game, char x, int row, int col){
        
	if(row > 0 && col > 0 && row <= game->b_size && col <= game->b_size && game->board[row - 1][col - 1] == '-'){ //Verificacao se a casa esta vazia e pertence ao tabuleiro
		game->board[row - 1][col - 1] = x; //Caso positivo, faz-se uma jogada bem sucedida
		return 0;	
	}
       	return -1; //Caso em que a jogada eh invalida 
}

//Funcao caso nao haja um vencedor, i.e, partida deu velha
int empatou(g_structure *game){
	for(int i = 0; i < game->b_size; i++){ //Percorre linhas
            	for(int j = 0; j < game->b_size; j++){ //Percorre colunas
                	if(game->board[i][j] == '-'){ //Verifica se ha alguma casa vazia
                    		return 0; //Caso positivo, o jogo ainda nao terminou
                	}
            	}
        }
	//Nao havendo casas vazias, o jogo eh finalizado como empatado
        return 1;
}

