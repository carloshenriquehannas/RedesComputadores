#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<poll.h>

#include "../headers/game_engine.h"
#include "../headers/server.h"

int start = 0, processed = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct{
	int player_id;
	int socket_id;
	g_structure *game;
} thread_info;

lp last_play;

cbi client_basic_info;

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
	get_s_size(game);

    	// Configuração da quantidade de jogadores
	get_p_num(game);

	// Cria os jogadores
	p_create(game);

	int s_socket, c_sockets[game->p_num]; 					// define os sockets

	memset(&last_play, 0, sizeof(lp));
	memset(&client_basic_info, 0, sizeof(cbi));

	client_basic_info.b_size = game->b_size;
	client_basic_info.s_size = game->s_size;

	struct sockaddr_in s_addr, c_addr;  					// cria os enderecos de socket
	socklen_t c_len = sizeof(c_addr);	

	// Cria o socket do servidor
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(s_socket == -1){
		printf("Erro! Falha na criacao do socket\n");
		exit(EXIT_FAILURE);
	}
	
	// Configura o endereco do servidor
	char ip[16];
	while(!read_ip(ip));
	memset(&s_addr, 0 , sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8080);
	s_addr.sin_addr.s_addr = inet_addr(ip);
	
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

	printf("Sala criada!\n");
	
	// declara as threads do host e dos clientes
	pthread_t h_thread;
	pthread_t c_threads[game->p_num - 1];

	// inicializa as informacoes passadas para as threads
	thread_info ht_info;
	ht_info.game = game;
	ht_info.socket_id = -1;
	thread_info ct_info[game->p_num - 1];
	for(int i = 0; i < game->p_num - 1; i++){
		ct_info[i].game = game;
	}

	ht_info.player_id = 0;
	pthread_create(&h_thread, NULL, host_handler, &ht_info);

	// Tenta coneccao com os outros jogadores ate o host decidir comecar o jogo
	
	while(!start && game->p_connected < (game->p_num - 1)){	
		
		// Aceita uma nova conexao com um cliente
		printf("Tentou aceitar\n");
            	c_sockets[game->p_connected] = accept(s_socket, (struct sockaddr *)&c_addr, &c_len);
            	if (c_sockets[game->p_connected] == -1) {
                	printf("Erro ao aceitar a conexao\n");
            	} else {
                	printf("Jogador conectado! FD: %d\n", c_sockets[game->p_connected]);

                	// Cria uma nova thread para lidar com o cliente
			ct_info[game->p_connected].player_id = game->p_connected - 1;
                	if (pthread_create(&c_threads[game->p_connected], NULL, client_handler, &(ct_info[game->p_connected])) != 0) {
                  		perror("Erro ao criar a thread\n");
				exit(-1);
                	} else {
                    		(game->p_connected)++;
                	}
            	}	
	}
	
	while(!game->g_ended){
		pthread_mutex_lock(&mutex);
		if(!processed){

			// Faz as verificacoes relativas a jogada atual	
			if(win(game, last_play.symb)){        // se ganhou
				last_play.end = 1;
				game->g_ended = 1;
			} else if (empatou(game)){            // se o jogo chegou a um empate
				last_play.end = -1;
				game->g_ended = -1;
			} else {			      // se continua normalmente
				game->next_player = (game->next_player + 1) % (game->p_connected + 1);  	// atualiza o proximo jogador
				last_play.next_player = game->next_player;
				processed = 1;				// define a informacao como processada
			}
			pthread_cond_broadcast(&cond); 					// avisa os clientes que as informacoes foram processadas
		}
		pthread_mutex_unlock(&mutex);	
	}
	
	// espera todas as threads terminarem o processamento
	pthread_join(h_thread, NULL);
	for(int i = 0; i <  game->p_connected; i++){
		pthread_join(c_threads[i], NULL);
	}
	
	// fecha os sockets
	close(s_socket);
	for(int i = 0; i < game->p_connected; i++){
		close(c_sockets[i]);
	}
	close(s_socket);

	// libera memorias alocadas
	for(int i = 0; i < game->b_size; i++){
		free(game->board[i]);
	}
	free(game->board);
	free(game->p_list);
	free(game);
	pthread_cond_destroy(&cond);
	return 0;
}

// Thread do host
void *host_handler(void *arg){
	thread_info *h_info = ((thread_info *) arg);

	h_info->game->p_list[0].simb = (33 + h_info->player_id);
	printf("Seu caracter eh: '%c'\n\n", h_info->game->p_list[0].simb);
	

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
		
		// O tabuleiro eh exibido enquanto nao for a vez do jogador jogar
		while((h_info->game->next_player != h_info->player_id || !processed) && !h_info->game->g_ended){
			mostra(h_info->game);

			pthread_cond_wait(&cond, &mutex);
		}
		if(h_info->game->g_ended) break;

		// Avisa o usuario que eh a vez dele jogar
		printf("Eh sua vez! Digite sua jogada no formato 'N N', linha por coluna. Exemplo: 5 3\n");

		// Recebe a jogada do usuario (jah validada)
		int row, col;
		while(1){
			scanf("%d %d", &row, &col);
			if(row <= h_info->game->b_size && col <= h_info->game->b_size && col > 0 && row > 0 && h_info->game->board[row - 1][col - 1] == '-'){
				break;
			}
			printf("Jogada invalida!\n");
		}
		// Aplica a jogada
		jogapessoa(h_info->game, h_info->game->p_list[h_info->player_id].simb, row, col);	

		last_play.col = col;
		last_play.row = row;
		last_play.symb = h_info->game->p_list[h_info->player_id].simb;

		processed = 0;
		pthread_mutex_unlock(&mutex);	
	}	

	printf("\nFim de jogo!\n");

	mostra(h_info->game);

	switch(last_play.end){
		case 1:
			printf("\nJogador %c venceu!\n", last_play.symb);
			break;
		case -1:
			printf("\nEmpate!\n");
			break;
	}

	return NULL;	
}

// Thread do cliente
void *client_handler(void *arg){
	thread_info *c_info = ((thread_info *) arg);
	
	int b_recv, b_sent;

	c_info->game->p_list[c_info->player_id].simb = (char) (33 + c_info->player_id);

	client_basic_info.player_id = c_info->player_id;
	client_basic_info.player_symbol = (33 + c_info->player_id);

	if((b_sent = send(c_info->socket_id, &client_basic_info, sizeof(cbi), 0)) == -1){
		printf("Erro de comunicacao!\n");
		exit(-1);
	}		
	
	
	while(!c_info->game->g_ended){
		pthread_mutex_lock(&mutex);
		// O tabuleiro eh exibido enquanto nao for a vez do jogador jogar
		while((c_info->game->next_player != c_info->player_id || !processed) && !c_info->game->g_ended){
			if((b_sent = send(c_info->socket_id, &last_play, sizeof(lp), 0)) == -1){
				printf("Erro de comunicacao!\n");
				exit(-1);
			}		
			pthread_cond_wait(&cond, &mutex);
		}
		if(c_info->game->g_ended) break;

		// Avisa o usuario que eh a vez dele jogar
		if((b_sent = send(c_info->socket_id, &last_play, sizeof(lp), 0)) == -1){
			printf("Erro de comunicacao!\n");
			exit(-1);
		}		
		
		// Recebe a jogada do usuario (jah validada)
		if((b_recv = recv(c_info->socket_id, &last_play, sizeof(lp) - 1, 0)) == -1){
			printf("Erro de comunicacao!\n");
			exit(-1);	
		}	

		// Aplica a jogada
		jogapessoa(c_info->game, c_info->game->p_list[c_info->player_id].simb, last_play.row, last_play.col);

		processed = 0;
		pthread_mutex_unlock(&mutex);	
	}	

	if((b_sent = send(c_info->socket_id, &last_play, sizeof(lp), 0)) == -1){
		printf("Erro de comunicacao!\n");
		exit(-1);
	}		
	return NULL;
}	
	
// Funcao para receber do teclado o tamanho do tabuleiro
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

// Funcao para receber do teclado o tamanho da sequencia de vitoria
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

// Funcao para receber do teclado o numero de jogadores maximo na sala
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

// Cria um tabuleiro e inicializa ele vazio
void b_create(g_structure *game){
	game->board = malloc(sizeof(char *) * game->b_size);
	for(int i = 0; i < game->b_size; i++){
		game->board[i] = malloc(sizeof(char) * game->b_size);
	}

	for(int i = 0; i < game->b_size; i++){
		for(int j = 0; j < game->b_size; j++){
			game->board[i][j] = '-';
		}
	}
	game->p_connected = 0;
	game->next_player = 0;
	game->g_ended = 0;
}

// Cria a lista de jogadores
void p_create(g_structure *game){
	game->p_list = malloc(sizeof(p_structure) * game->p_num);	
}

// Le o IP do teclado
int read_ip(char *ip){
	char input[100];

    	printf("Digite o endereco de IP: ");
	scanf("%s", input);

    	// Remove o '\n'
    	size_t input_length = strlen(input);
    	if (input_length > 0 && input[input_length - 1] == '\n') {
        	input[input_length - 1] = '\0';
    	}
    	return 1;
}
