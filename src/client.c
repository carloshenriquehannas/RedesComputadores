#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>

#include "../headers/client.h"
#include "../headers/game_engine.h"
#include "../headers/server.h"

int client(){
	int c_socket;
	struct sockaddr_in s_addr;

	g_structure game;
	
	cbi client_basic_info;
	lp last_play;

	memset(&client_basic_info, 0, sizeof(cbi));
	memset(&last_play, 0, sizeof(lp));
	
	// Cria o socket do cliente
	c_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(c_socket == -1){
		printf("Erro ao criar o socket\n");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		// Recebe o codigo da sala para conexao
		char ip[16] = {0};
		if(get_lobby_code(ip)){

			// Inicializa a estrutura de endereco do servidor
			s_addr.sin_family = AF_INET;
			s_addr.sin_port = htons(8080);
			s_addr.sin_addr.s_addr = inet_addr(ip);

			// Conecta ao servidor
			if(connect(c_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) != -1){
				break;
			} else {
				printf("Nao foi possivel conectar ao servidor. Tente novamente.\n");
			}
		}
	}
	
	printf("Voce entrou na sala.\n");

	int last_player = -1;

	if(!recv_basic_info(c_socket, &client_basic_info)){
		printf("Erro de comunicaco com o servidor!\n");
		exit(-1);	
	}

	printf("Tamanho do tabuleiro: %d\nTamanho da sequencia: %d\nSeu caractere: '%c'\n\n", client_basic_info.b_size, client_basic_info.s_size, client_basic_info.player_symbol);

	game.b_size = client_basic_info.b_size;
	game.s_size = client_basic_info.s_size;

	b_create(&game);	

	while(!last_play.end){
		if(!recv_last_play(c_socket, &last_play)){
			printf("Erro de comunicacao!\n");
			exit(-1);
		}
		if (last_player != last_play.next_player){
			last_player = last_play.next_player;
		}else {
			game.board[last_play.row - 1][last_play.col - 1] = last_play.symb;
			mostra(&game);
			continue;
		}

		game.board[last_play.row - 1][last_play.col - 1] = last_play.symb;
		mostra(&game);
		

		// Se for a vez do jogador
		if(last_play.next_player == client_basic_info.player_id && last_play.end == 0){
			// Avisa o usuario que eh a vez dele jogar
			printf("\nEh sua vez! Digite sua jogada no formato 'N N', linha por coluna. Exemplo: 5 3\n");

			// Recebe a jogada do usuario (jah validada)
	            	int row, col;
                   	while(1){
                        	scanf("%d %d", &row, &col);
                           	if(row <= client_basic_info.b_size && col <= client_basic_info.b_size && col > 0 && row > 0 && game.board[row - 1][col - 1] == '-'){
                                   	break;
                           	}
                           	printf("Jogada invalida!\n");
			}

			last_play.row = row;
			last_play.col = col;
			last_play.symb = client_basic_info.player_symbol;
			last_play.next_player++;

			if(!send_last_play(c_socket, &last_play)){
				printf("Erro de comunicacao!\n");
				exit(-1);		
			}
			printf("%d\n", last_play.next_player);
		}
	}
	switch(last_play.end){
                 case 1:
                         printf("Jogador %c venceu!\n", last_play.symb);
			 break;
                 case -1:
                         printf("Empate!\n");
			 break;
         }

	return 0;
}

int get_lobby_code(char *ip){
	char input[100];

    	printf("Digite o endereco de IP: ");
	fgets(input, sizeof(input) - 1, stdin);

	int i = 0;
	while(input[i] != '\n'){
		ip[i] = input[i];
		i++;
	}
	ip[i] = '\0';

    	if(strlen(ip) > 0 && strlen(ip) <= 16)return 1;

	return 0;
}

int recv_basic_info(int c_socket, cbi *client_basic_info){
	int b_recv;
	char aux[sizeof(cbi)];

	// Recebe id do jogador
	if((b_recv = recv(c_socket, &aux, sizeof(cbi), 0)) == -1) return 0;
	memcpy(client_basic_info, aux, sizeof(cbi));

	return 1;
}

