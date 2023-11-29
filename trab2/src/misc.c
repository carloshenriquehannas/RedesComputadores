#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>

#include "../headers/misc.h"
#include "../headers/transmissor.h"

void strToBin(char *_str, datagrama *_quadro){
	if(_str == NULL){
		perror("Erro ao processar a mensagem!\n");
		exit(-1);
	}	

	size_t _len = strlen(_str);
	_quadro->_binData = malloc(sizeof(uint8_t)*_len);
	_quadro->_binDataLen = 0;


	for(size_t i = 0; i < _len; i++){
		uint8_t _aux = _str[i];
		for(int j = 7; j >= 0; --j){
			if(_aux & (1 << j)){
				_quadro->_binData[_quadro->_binDataLen++] = 1;
			}else{
				_quadro->_binData[_quadro->_binDataLen++] = 0;
			}
		}	
	
	}
}

void binToStr(char *_str, datagrama *_quadro){



}

void MeioDeComunicacao(datagrama *_quadro){
	int _porcentagemDeErros = 10;    	// porcentagem de erros padrao

	printf("Digite a porcentagem de erros (e.g. 20%, 30% , 70%):\n");
	scanf("%d\n", &_porcentagemDeErros);

	for(int i = 0; i < _quadro->_totalLen; i++){
		if((rand() % 101) <= _porcentagemDeErros){
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

