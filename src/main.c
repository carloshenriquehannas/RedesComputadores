#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../headers/game_engine.h"
#include "../headers/server.h"
#include "../headers/client.h"

int main(int argc, char *argv[]){

	char str[1024];

	while(1){	
    		printf("\nJOGO DA VELHA++\n");

    		printf("\n 1 - Hospedar um jogo\n");
    		printf("\n 2 - Entrar em uma sala\n");
    		printf("\n\n 3 - Sair do jogo\n\n");

		while(1){
			fgets(str, sizeof(str), stdin);
			switch(atoi(str)){
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

    // Explicação do modo de jogar
    printf("\nMODO DE JOGAR\n");
    printf("Apos aparecer o tabuleiro, basta digitar a letra que representa a coluna e, em seguida, o numero que corresponde a linha em que quer jogar. Pressione ENTER no fim\n");
    printf("Exemplo: para jogar na primeira coluna e segunda linha, digite A2");

    // Começar o jogo
    printf("\n\n-TUDO PRONTO, O JOGO COMECOU!-\n\n");

}
