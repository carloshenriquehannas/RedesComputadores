#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<poll.h>

#include "../headers/game_engine.h"
#include "../headers/server.h"

int start = 0, processed = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct{
	int player_id;
	int socket_id;
	g_structure *game;
} thread_info;

int server(){
	g_structure *game = malloc(sizeof(g_structure) * 1);

	game->g_ended = 0;

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

	int s_socket, c_sockets[game->p_num];

	struct sockaddr_in s_addr, c_addr;
	socklen_t c_len = sizeof(c_addr);	
	struct pollfd poll_fds[game->p_num];

	// Cria o socket do servidor
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(s_socket == -1){
		printf("Erro! Falha na criacao do socket\n");
		exit(EXIT_FAILURE);
	}
	
	// Configura o endereco do servidor
	memset(&s_addr, 0 , sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8080);
	s_addr.sin_addr.s_addr = INADDR_ANY;

	// 'Bind' do servidor ao ip
	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1){
		printf("Erro! Falha ao fazer o bind\n");
		exit(EXIT_FAILURE);
	}

	// Entrar em modo de escuta
	if(listen(s_socket, game->p_num - 1) == -1){
		printf("Erro! Falha ao escutar uma conexao\n");
		exit(EXIT_FAILURE);
	}	

	printf("Sala criada! O codigo da sala eh: TODO\n");

	poll_fds[0].fd = s_socket;
    	poll_fds[0].events = POLLIN;

	pthread_t h_thread;
	pthread_t c_threads[game->p_num - 1];

	thread_info ht_info;
	thread_info ct_info[game->p_num - 1];

	ht_info.player_id = 0;
	pthread_create(&h_thread, NULL, host_handler, &ht_info);

	// Tenta coneccao com os outros jogadores ate o host decidir comecar o jogo
	
	while(!start){	
		// Aceitar uma conexao
		int num_events = poll(poll_fds, game->p_connected + 1, 200);
        	if (num_events == -1) {
            		perror("poll");
            		exit(EXIT_FAILURE);
        	}

        	// Check if the listening socket has an event
        	if ((poll_fds[0].revents & POLLIN) && game->p_connected < game->p_num - 1) {

            		// Accept a new client connection
            		c_sockets[game->p_connected] = accept(s_socket, (struct sockaddr *)&c_addr, &c_len);
            		if (c_sockets[game->p_connected] == -1) {
                		printf("Erro ao aceitar a conexao\n");
            		} else {
                		printf("Jogador conectado! FD: %d\n", c_sockets[game->p_connected]);

                		// Create a new thread to handle the client
				ct_info[game->p_connected].player_id = game->p_connected - 1;
                		if (pthread_create(&c_threads[game->p_connected], NULL, client_handler, &(ct_info[game->p_connected])) != 0) {
                    			perror("Erro ao criar a thread\n");
                		} else {
                    			(game->p_connected)++;
                		}
            		}	
				
		}
	}
	
	while(1){
		pthread_mutex_lock(&mutex);
		if(!processed){

			

			game->next_player = (game->next_player + 1) % game->p_num;
			processed = 1;
			pthread_cond_broadcast(&cond);
		}
		pthread_mutex_unlock(&mutex);	
	}
	
	close(s_socket);
	return 0;
}

void *host_handler(void *arg){
	thread_info *h_info = ((thread_info *) arg);

	printf("Selecione seu caracter! Por favor, use bom senso!\n");
	scanf("%c", &(h_info->game->p_list[0].simb));
	
	

	printf("Para iniciar o jogo, aperte 0\n");
	while(1){
		int aux;
		scanf("%d", &aux);
		if(!aux){
			start = 1;
			break;
		}
	}
	
	while(!h_info->game->g_ended){
		pthread_mutex_lock(&mutex);
		
		if(processed){
			
			// Envia o tabuleiro processado

			// Espera sua vez de jogar
			while(h_info->game->next_player != h_info->player_id){
				pthread_cond_wait(&cond, &mutex);
			}

			// Avisa o usuario que eh a vez dele jogar
			

			// Recebe a jogada do usuario (jah validada)


			// Aplica a jogada

			processed = 0;
		}	

		pthread_mutex_unlock(&mutex);	
	}	
	
}

void *client_handler(void *arg){
	thread_info *c_info = ((thread_info *) arg);
	
	
	while(1){
		pthread_mutex_lock(&mutex);
		if(processed){
			
			// Envia o tabuleiro processado

			// Espera sua vez de jogar
			while(c_info->game->next_player != c_info->player_id){
				pthread_cond_wait(&cond, &mutex);
			}

			// Avisa o usuario que eh a vez dele jogar
			

			// Recebe a jogada do usuario (jah validada)


			// Aplica a jogada

			processed = 0;
		}	
		pthread_mutex_unlock(&mutex);	
	}	
	

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
