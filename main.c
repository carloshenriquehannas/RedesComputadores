#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){

/* ---------------------------------------- DEFINICOES DAS VARIAVEIS  ----------------------------------------------- */

    int n, p, jog;

    struct jogador{
        int comp;
        char simb;
    };
    struct jogador joga[jog];

/* ---------------------------------------- CONFIGURACOES INICIAIS DE MENU ------------------------------------------ */

    printf("\nJOGO DA VELHA++\n");

    printf("\nCONFIGURACAO INICIAL\n");

    printf("\nDIMENSAO DO TABULEIRO\n");
    printf("Digite a dimensao N do tabuleiro. Exemplo: para N = 3, digite o numero 3.\n");
    printf("Digite a dimensao do tabuleiro: ");
    do{
        scanf("%d", &n);
        if (n <= 0){
            printf("Tamanho invalido. Digite novamente: ");
        }
    } while(n <= 0); //Tratamento de erro para dimensao de tabuleiro
    printf("\n-> Dimensao selecionada: %d x %d\n", n, n);

    //Criacao do tabuleiro com a respectiva dimensao e preenchimento dele com - (vazio)
    char M[n][n];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            M[i][j]='-';}
    }

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

    printf("\nMODO DE JOGAR\n");
    printf("Apos aparecer o tabuleiro, basta digitar a letra que representa a coluna e, em seguida, o numero que corresponde a linha em que quer jogar. Pressione ENTER no fim\n");
    printf("Exemplo: para jogar na primeira coluna e segunda linha, digite A2");

    printf("\n\n-TUDO PRONTO, O JOGO COMECOU!-\n\n");

/* -------------------------------------------------- CODIGO DO JOGO ------------------------------------------------ */

    //Funcao que procura vencedor em linhas, colunas ou diagonais
    int win(char x){
        for(int i = 0;i < n; i++) {
            int contL = 0;
            int contC = 0;
            for(int j = 0; j < n; j++){
                if (M[i][j] == x){
                    contL++;
                }else{
                    contL=0;
                }
                if (M[j][i]==x) {
                    contC++;
                } else{
                    contC=0;
                }
                if(contC == p || contL == p){
                    return 1;
                }
            }
        }

        for(int i = 0; i < n-p+1; i++){
            for(int j = 0; j < n-p+1; j++){
                int contD1 = 0;
                for(int k = 0; k < p; k++){
                    if (M[i+k][j+k] ==x ){
                        contD1++;
                    } else{
                        break;
                    }
                }
                if (contD1 == p){
                    return 1;
                }
            }
        }
        for(int i = 0; i < n-p+1; i++){
            for(int j = p-1; j < n; j++){
                int contD2 = 0;
                for(int k = 0; k < p; k++){
                    if (M[i+k][j-k] == x){
                        contD2++;
                    } else{
                        break;
                    }
                }
                if (contD2 == p){
                    return 1;
                }
            }
        }
        return 0;
    }

    //Funcao que procura a melhor jogada do computador. Procura jogar onde ha chance de vencer, assim como jogar onde ha chance de perder
    int jogabot(char x){
        int chances(char x, int i, int j){
            int nchances = 0;
            M[i][j] = x;
            for(int k = 0; k < n; k++){
                for(int l = 0;l < n; l++){
                    if(M[k][l] == '-'){
                        M[k][l] = x;
                        if (win(x)){
                            nchances++;
                        }
                        M[k][l]='-';
                    }
                }
            }
            M[i][j]='-';
            return nchances;
        }

        int mchances = 0;
        int l, c;
        if(n%2 == 1 && M[n/2][n/2] == '-'){
            M[n/2][n/2] = x;
            return 0;
        }
        for(int i = 0;i < n; i++){
            for(int j = 0; j < n; j++){
                if(M[i][j] == '-'){
                    M[i][j] = x;
                    if(win(x)){
                        return 0;
                    } else{
                        M[i][j] = '-';
                    }
                    for(int k = 0;k < jog; k++){
                        M[i][j] = joga[k].simb;
                        if(win(joga[k].simb)){
                            M[i][j] = x;
                            return 0;
                        } else{
                            M[i][j]='-';
                        }
                    }
                    if(chances(x, i, j) > mchances){
                        mchances=chances(x, i, j);
                        l = i;
                        c = j;
                    }
                }
            }
        }
        if (mchances > 0){
            M[l][c] = x;
            return 0;
        }
        for(int i = 0;i < n; i++){
            for(int j = 0;j < n; j++){
                if(M[i][j] == x){
                    if(M[i-1][j-1] == x && M[i+1][j+1] == '-'){
                        M[i+1][j+1] = x;
                        return 0;
                    }
                    if(M[i+1][j+1] == x && M[i-1][j-1] == '-'){
                        M[i-1][j-1] = x;
                        return 0;
                    }
                    if(M[i+1][j] == x && M[i-1][j] == '-'){
                        M[i-1][j] = x;
                        return 0;
                    }
                    if(M[i-1][j] == x && M[i+1][j] == '-'){
                        M[i+1][j]=x;
                        return 0;
                    }
                    if(M[i-1][j+1] == x && M[i+1][j-1] == '-'){
                        M[i+1][j-1] = x;
                        return 0;
                    }
                    if(M[i+1][j-1] == x && M[i-1][j+1] == '-'){
                        M[i-1][j+1] = x;
                        return 0;
                    }
                    if(M[i][j+1] == x && M[i][j-1] == '-'){
                        M[i][j-1] = x;
                        return 0;
                    }
                    if(M[i][j-1] == x && M[i][j+1] == '-'){
                        M[i][j+1] = x;
                        return 0;
                    }
                }
            }
        }
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(M[i][j] == x){
                    if(M[i][j+1] == '-'){
                        M[i][j+1] = x;
                        return 0;
                    }
                    if(M[i+1][j] == '-'){
                        M[i+1][j] = x;
                        return 0;
                    }
                    if(M[i+1][j+1] == '-'){
                        M[i+1][j+1] = x;
                        return 0;
                    }
                    if(M[i+1][j-1] == '-'){
                        M[i+1][j-1] = x;
                        return 0;
                    }
                    if(M[i-1][j+1] == '-'){
                        M[i-1][j+1] = x;
                        return 0;
                    }
                    if(M[i-1][j-1] == '-'){
                        M[i-1][j-1] = x;
                        return 0;
                    }
                    if(M[i][j-1] == '-'){
                        M[i][j-1] = x;
                        return 0;
                    }
                    if(M[i-1][j] == '-'){
                        M[i-1][j] = x;
                        return 0;
                    }
                }
            }
        }
        for(int i = 0;i < n; i++){
            for(int j = 0;j < n; j++){
                if(M[i][j] == '-'){
                    M[i][j] = x;
                    return 0;
                }
            }
        }
    }

    //Funcao para imprimir o tabuleiro na tela
    int mostra(){
        printf("  %c",'A');
        for(int i = 1; i < n; i++){
            printf(" %c", 65+i)
            ;
        }
        printf("\n");
        for(int i = 0; i < n; i++){
            printf("%d ", i+1);
            for(int j = 0; j < n; j++){
                printf("%c ",M[i][j]);
            }
            printf("\n");
        }
    }

    //Funcao que faz a jogada do usuario
    int jogapessoa(char x){
        int l;
        char c;
        scanf("%c",&c); //Coluna em que vai jogar
        scanf("%d",&l); //Linha em que vai jogar
        if (M[l-1][c-'A'] == '-'){
            M[l-1][c-'A'] = x;
            return 0;
        }
        return 1;
    }

    //Funcao caso nao haja um vencedor, i.e, partida deu velha
    int empatou(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(M[i][j] == '-'){
                    return 0;
                }
            }
        }
        return 1;
    }

    if(joga[0].comp == 0){
        mostra();
    }

    int i;
    for(i = 0;i <= jog; i++){
        if(i == jog){
            i = 0;}
        if(joga[i].comp == 1){
            jogabot(joga[i].simb);
        } else{
            while(jogapessoa(joga[i].simb));
        }
        mostra();
        if (empatou() || win(joga[i].simb)){
            break;
        }
    }
    if (win(joga[i].simb)){
        printf("Jogador %c ganhou!!!\n", joga[i].simb);
    } else{
        printf("Jogo empatado\n");
    }

    //AQUI TEM QUE ARRUMAR PARA VOLTAR AO INICIO DO JOGO
}



