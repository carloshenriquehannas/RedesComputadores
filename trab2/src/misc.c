#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

#include "../headers/misc.h"

// funcao que converte string para uma sequencia de bits
void strToBin(char *_str, datagrama *_quadro){
	if(_str == NULL){
		perror("Erro ao processar a mensagem!\n");
		exit(-1);
	}	

	// descobre o tamanho da string com o \n
	size_t _len = strlen(_str);

	// aloca 8 bits para cada caracter, desconsiderando o \n
	_quadro->_binData = malloc(sizeof(uint8_t)*(_len-1)* 8);
	_quadro->_binDataLen = 0;


	for(size_t i = 0; i < (_len - 1); i++){
		uint8_t _aux = _str[i];

		// para cada bit do caracter, isola apenas ele
		for(int j = 7; j >= 0; --j){
			if(_aux & (1 << j)){
				_quadro->_binData[_quadro->_binDataLen++] = 1;
			}else{
				_quadro->_binData[_quadro->_binDataLen++] = 0;
			}
		}	
	}

	// imprime os valores em binario apenas para comparacao
	printf("\nMensagem antes do envio em binario:\n");	
	for(int i = 0; i < _quadro->_binDataLen; i++){
		if((i % 8) == 0 && i != 0) printf(" ");
		printf("%d", _quadro->_binData[i]);
	}
	printf("\n\n");
}

void binToStr(unsigned char *_str, datagrama *_quadro){
	// define o tamanho da string a ser reconstruida
	int _strLen = (_quadro->_binDataLen / 8);

	// vetor intermediario para converter binario -> decimal -> char
	int _auxArr[_strLen];

	// aloca o tamnaho do vetor mais 1 para o '/0'
	_str = malloc(sizeof(char)*(_strLen + 1));

	// zera a string para fazer a conversao
	for(int i = 0; i < _strLen; i++){
		_auxArr[i] = 0;
	}

	int _aux = 0;
	for(int i = 0; i < _quadro->_binDataLen; i++){
		_auxArr[_aux] = (char)(_str[_aux] ^ (_quadro->_binData[i] << (7 - (i % 8))));	
		if((i % 8) == 7){
			_aux++;	
		}
	}

	for(int i = 0; i < _strLen; i++){
		_str[i] = (char) _auxArr[i];
	}
	_str[_strLen] = '\0';

}

void MeioDeComunicacao(datagrama *_quadro){
	int _porcentagemDeErros;	

	printf("Digite a porcentagem de erros (e.g. 20, 30, 70):\n");
	scanf("%d", &_porcentagemDeErros);
	if(_porcentagemDeErros > 100 || _porcentagemDeErros < 0){
		printf("Entrada invalida! Utilizando erro padrao de 5 porcento\n\n");
		_porcentagemDeErros = 5;    	// porcentagem de erros padrao

	}

	for(int i = 0; i < _quadro->_totalLen; i++){
		if((rand() % 101) < _porcentagemDeErros){
			// se deu erro, inverte o bit
			_quadro->_binData[i] = (_quadro->_binData[i] ^ 1);
		}
	}

}

void CopiaDadosBin(datagrama *_quadro, uint8_t *_dest){
	for(int i = 0; i < _quadro->_totalLen; i++){
		_dest[i] = _quadro->_binData[i];
	}	
}

