#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>

#include "../headers/client.h"
#include "../headers/game_engine.h"

int client(){
	int c_socket;
	struct sockaddr_in s_addr;

	g_structure *game;	
	
	// Cria o socket do cliente
	c_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(c_socket == -1){
		printf("Erro ao criar o socket\n");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		// Recebe o codigo da sala para conexao
		char ip[32];
		get_lobby_code(ip);

		// Inicializa a estrutura de endereco do servidor
		memset(&s_addr, 0, sizeof(s_addr));
		s_addr.sin_family = AF_INET;
		s_addr.sin_port = htons(8080);
		inet_pton(AF_INET, ip, &s_addr.sin_addr);

		// Conecta ao servidor
		if(connect(c_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) != -1){
			free(ip);
			break;
		} else {
			printf("\nNao foi possivel conectar a sala. Verifique o codigo e tente novamente.\n");
		}
	}



	return 0;
}

void get_lobby_code(char *ip){
	char aux[1024];
	printf("Digite o codigo da sala: ");
	fgets(aux, sizeof(aux),stdin);
	int i = 0, j = 0;
	while(i < strlen(aux) - 5){ 
		ip[j] = aux[i];
		i++;
		j++;
		if((i+1) % 3 == 0){
			j++;
			ip[j] = '.';
		}
		i++;
		j++;
	}
	ip[j] = '\0'; 
}
