#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){

 ////////////////////////////// MENU INICIAL //////////////////////////////
    int n, p;
    printf("\nJOGO DA VELHA++\n");

    printf("\nCONFIGURACAO INICIAL\n");
    printf("\nDIMENSAO DO TABULEIRO\n");
    printf("Digite a dimensao N do tabuleiro. Exemplo: para N = 3, digite o numero 3.\n");
    printf("Digite a dimensao do tabuleiro: ");
    scanf("%d", &n);
    char M[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            M[i][j]='-';}}

    printf("\n-> Dimensao selecionada: %d x %d\n", n,n);

    printf("\nSEQUENCIA DO VENCEDOR\n");
    printf("Digite a sequencia P que define um vencedor. Exemplo: para P = 3, digite o numero 3.\n");
    printf("Digite a sequencia que define o vencerdor: ");
    //Aqui tem que colocar o tratamento de caso em que S > N: isso nao pode acontecer

    scanf("%d", &p);
    printf("\n-> Tamanho de sequencia escolhido: %d\n", p);

    printf("\nQUANTIDADE DE JOGADORES\n");
    printf("Digite a quantidade de jogadores. Exemplo: para 5 jogadores, digite o numero 5.\n");
    printf("Digite o numero de jogadores que irao jogar: ");

    int jog;
    scanf("%d", &jog);
    printf("\n-> Numero de jogadores selecionados: %d\n", jog);

    struct jogador{
        int comp;
        char simb;
    };
    struct jogador joga[jog];

    for (int i=0;i<jog;i++){
        printf("\n-INFORMACOES DO JOGADOR %d-\n", i+1);
        printf("Digite 1 para que o jogador seja a maquina. Digite 0 para que o jogador nao seja a maquina\n");
        printf("Digite aqui: ");
        scanf("%d",&joga[i].comp);
        printf("\n");
        printf("Digite o simbolo que ira representar esse jogador. Exemplo: para ser o simbolo 'X', basta digitar a letra X\n");
        printf("Digite agora, o simbolo que representara este jogador: ");
        scanf(" %c",&joga[i].simb);
    }

    printf("\nMODO DE JOGAR\n");
    printf("Apos aparecer o tabuleiro, basta digitar a letra que representa a coluna e, em seguida, o numero que corresponde a linha em que quer jogar. Pressione ENTER no fim\n");
    printf("Exemplo: para jogar na primeira coluna e segunda linha, digite A2");

    printf("\n\n-TUDO PRONTO, O JOGO COMECOU!-\n\n");

    ////////////////////////////// CODIGO DO JOGO //////////////////////////////

    int win(char x){
        //procura vencedor em linhas, colunas ou diagonais
        for(int i=0;i<n;i++) {
            int contL=0;
            int contC=0;
            for(int j=0;j<n;j++){
                if (M[i][j]==x){
                    contL++;}
                else{
                    contL=0;}
                if (M[j][i]==x){
                    contC++;}
                else{
                    contC=0;}
                if(contC==p || contL==p){
                    return 1;}}}
        for(int i=0;i<n-p+1;i++){
            for(int j=0;j<n-p+1;j++){
                int contD1=0;
                for(int k=0;k<p;k++){
                    if (M[i+k][j+k]==x){
                        contD1++;}
                    else{
                        break;}}
                if (contD1==p){
                    return 1;}}}
        for(int i=0;i<n-p+1;i++){
            for(int j=p-1;j<n;j++){
                int contD2=0;
                for(int k=0;k<p;k++){
                    if (M[i+k][j-k]==x){
                        contD2++;}
                    else{
                        break;}}
                if (contD2==p){
                    return 1;}}}
        return 0;}
    int jogabot(char x){
        int chances(char x,int i,int j){
            int nchances=0;
            M[i][j]=x;
            for(int k=0;k<n;k++){
                for(int l=0;l<n;l++){
                    if(M[k][l]=='-'){
                        M[k][l]=x;
                        if (win(x)){
                            nchances++;}
                        M[k][l]='-';}}}
            M[i][j]='-';
            return nchances;}
        int mchances=0;
        int l,c;
        if(n%2==1 && M[n/2][n/2]=='-'){
            M[n/2][n/2]=x;
            return 0;}
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(M[i][j]=='-'){
                    M[i][j]=x;
                    if(win(x)){
                        return 0;}
                    else{M[i][j]='-';}
                    for(int k=0;k<jog;k++){
                        M[i][j]=joga[k].simb;
                        if(win(joga[k].simb)){
                            M[i][j]=x;
                            return 0;}
                        else{M[i][j]='-';}}
                    if(chances(x,i,j)>mchances){
                        mchances=chances(x,i,j);
                        l=i;
                        c=j;}}}}
        if (mchances>0){
            M[l][c]=x;
            return 0;}
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(M[i][j]==x){
                    if(M[i-1][j-1]==x && M[i+1][j+1]=='-'){
                        M[i+1][j+1]=x;return 0;}
                    if(M[i+1][j+1]==x && M[i-1][j-1]=='-'){
                        M[i-1][j-1]=x;return 0;}
                    if(M[i+1][j]==x && M[i-1][j]=='-'){
                        M[i-1][j]=x;return 0;}
                    if(M[i-1][j]==x && M[i+1][j]=='-'){
                        M[i+1][j]=x;return 0;}
                    if(M[i-1][j+1]==x && M[i+1][j-1]=='-'){
                        M[i+1][j-1]=x;return 0;}
                    if(M[i+1][j-1]==x && M[i-1][j+1]=='-'){
                        M[i-1][j+1]=x;return 0;}
                    if(M[i][j+1]==x && M[i][j-1]=='-'){
                        M[i][j-1]=x;return 0;}
                    if(M[i][j-1]==x && M[i][j+1]=='-'){
                        M[i][j+1]=x;return 0;}}}}
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(M[i][j]==x){
                    if(M[i][j+1]=='-'){
                        M[i][j+1]=x;return 0;}
                    if(M[i+1][j]=='-'){
                        M[i+1][j]=x;return 0;}
                    if(M[i+1][j+1]=='-'){
                        M[i+1][j+1]=x;return 0;}
                    if(M[i+1][j-1]=='-'){
                        M[i+1][j-1]=x;return 0;}
                    if(M[i-1][j+1]=='-'){
                        M[i-1][j+1]=x;return 0;}
                    if(M[i-1][j-1]=='-'){
                        M[i-1][j-1]=x;return 0;}
                    if(M[i][j-1]=='-'){
                        M[i][j-1]=x;return 0;}
                    if(M[i-1][j]=='-'){
                        M[i-1][j]=x;return 0;}}}}
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(M[i][j]=='-'){
                    M[i][j]=x;
                    return 0;}}}}
    int mostra(){
        printf("  %c",'A');
        for(int i=1;i<n;i++){
            printf(" %c",65+i);}
        printf("\n");
        for(int i=0;i<n;i++){
            printf("%d ",i+1);
            for(int j=0;j<n;j++){
                printf("%c ",M[i][j]);}
            printf("\n");}}
    int jogapessoa(char x){
        int l;
        char c;
        scanf("%c",&c);
        scanf("%d",&l);
        if (M[l-1][c-'A']=='-'){
            M[l-1][c-'A']=x;
            return 0;}
        return 1;}
    int empatou(){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(M[i][j]=='-')
                {return 0;}}}
        return 1;}
    if(joga[0].comp==0){
        mostra();}
    int i;
    for(i=0;i<=jog;i++){
        if(i==jog){
            i=0;}
        if(joga[i].comp==1){
            jogabot(joga[i].simb);}
        else{
            while(jogapessoa(joga[i].simb));}
        mostra();
        if (empatou() || win(joga[i].simb)){
            break;}}
    if (win(joga[i].simb)){
        printf("Jogador %c ganhou!!!\n", joga[i].simb);}
    else{
        printf("Jogo empatado\n");}}

