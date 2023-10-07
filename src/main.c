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
					//server();
					break;
				
				case 2:
					//client();
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

    // Definição do menu de configuração inicial
    printf("\nCONFIGURACAO INICIAL\n");

    // Configuração da dimensão do tabuleiro
    printf("\nDIMENSAO DO TABULEIRO\n");
    printf("Digite a dimensao N do tabuleiro. Exemplo: para N = 3, digite o numero 3.\n");
    printf("Digite a dimensao do tabuleiro: ");
    do{
        scanf("%d", &n);
        if (n <= 0){
            printf("Tamanho invalido. Digite novamente: ");
        }
    } while(n <= 0 ); //Tratamento de erro para dimensao de tabuleiro
    printf("\n-> Dimensao selecionada: %d x %d\n", n, n);

    //Criacao do tabuleiro com a respectiva dimensao e preenchimento dele com - (vazio)
    char M[n][n];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            M[i][j]='-';}
    }

    // Configuração da sequência necessária para vencer
    printf("\nSEQUENCIA DO VENCEDOR\n");
    printf("Digite a sequencia P que define um vencedor. Exemplo: para P = 3, digite o numero 3.\n");
    printf("Digite a sequencia que define o vencerdor: ");
    do{
        scanf("%d", &p);
        if(p > n || p <= 0){
            printf("Tamanho invalido. Digite novamente: ");
        }
    } while(!(p > 0 && p <= n)); //Tratamento de erro para sequencia de um vencedor
    printf("\n-> Tamanho de sequencia escolhido: %d\n", p);

    // Configuração da quantidade de jogadores
    printf("\nQUANTIDADE DE JOGADORES\n");
    printf("Digite a quantidade de jogadores. Exemplo: para 5 jogadores, digite o numero 5.\n");
    printf("Digite o numero de jogadores que irao jogar: ");
    do{
        scanf("%d", &jog);
        if(jog <= 0){
            printf("Quantidade invalida. Digite novamente: ");
        }
    } while(jog <= 0); //Tratamento de erro para quantidade de jogares
    printf("\n-> Numero de jogadores selecionados: %d\n", jog);

    //Preenchimento das informacoes de cada um dos jogadores
    for (int i = 0; i < jog; i++){
        printf("\n-INFORMACOES DO JOGADOR %d-\n", i+1);
        printf("Digite 1 para que o jogador seja a maquina. Digite 0 para que o jogador nao seja a maquina\n");
        printf("Digite aqui: ");
        scanf("%d", &joga[i].comp);
        printf("\n");
        printf("Digite o simbolo que ira representar esse jogador. Exemplo: para ser o simbolo 'X', basta digitar a letra X\n");
        printf("Digite agora, o simbolo que representara este jogador: ");
        scanf(" %c", &joga[i].simb);
    }

    // Explicação do modo de jogar
    printf("\nMODO DE JOGAR\n");
    printf("Apos aparecer o tabuleiro, basta digitar a letra que representa a coluna e, em seguida, o numero que corresponde a linha em que quer jogar. Pressione ENTER no fim\n");
    printf("Exemplo: para jogar na primeira coluna e segunda linha, digite A2");

    // Começar o jogo
    printf("\n\n-TUDO PRONTO, O JOGO COMECOU!-\n\n");

    //Tratamento caso o jogador queira jogar de novo ou sair do jogo
    char again[10];
    printf("Digite a palavra JOGAR e pressione ENTER para jogar novamente. Aperte qualquer tecla para sair e pressione ENTER.\n");
    printf("Digite aqui: ");
    scanf("%s", again);

    for(i = 0; i < strlen(again); i++){
        again[i] = toupper(again[i]);
    }

    if(!(strcmp(again, "JOGAR"))){
        goto inicio;
    }
}
