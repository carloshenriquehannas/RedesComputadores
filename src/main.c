#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../headers/game_engine.h"
#include "../headers/server.h"
#include "../headers/client.h"

int main(int argc, char *argv[]){

	int aux;

	while(1){	
    		printf("\nJOGO DA VELHA++\n");

    		printf("\n 1 - Hospedar um jogo\n");
    		printf("\n 2 - Entrar em uma sala\n");
    		printf("\n\n 3 - Sair do jogo\n\n");

		while(1){
			scanf("%d", &aux);
			switch(aux){
				case 1:
					server();
					break;
				
				case 2:
					client();
					break;

				case 3:
					exit(0);
					return 0;

				default:
					printf("\nEntrada invalida! Por favor, digite novamente.\n\n");
					break;
			}
		}
	}
}
