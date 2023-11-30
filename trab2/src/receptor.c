#include<stdio.h>
#include<stdlib.h>

#include "../headers/receptor.h"

void AplicacaoReceptora(char *_str){
	printf("Mensagem recebida:\n");

	printf("%s\n", _str);

}

void ChamadaDeAplicacaoReceptora(datagrama *_quadro){
	char *_str;

	binToStr(_str, _quadro);

	AplicacaoReceptora(_str);
}

void CamadaEnlaceDadosReceptora(datagrama *_quadro){
	printf("Mensagem binaria recebida:\n");
	// imprime os valores em binario apenas para comparacao
	for(int i = 0; i < _quadro->_binDataLen; i++){
		if((i % 8) == 0 && i != 0) printf(" ");
		printf("%d", _quadro->_binData[i]);
	}
	printf("\n\n");

	CamadaEnlaceDadosReceptoraControleDeErro(_quadro);

	ChamadaDeAplicacaoReceptora(_quadro);
}

void CamadaEnlaceDadosReceptoraControleDeErro(datagrama *_quadro){
	switch(_quadro->_tipoDeControleDeErro){
		case 0:
			CamadaEnlaceDadosReceptoraControleDeErroCRC(_quadro);
			break;
		case 1:
			CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(_quadro);
			break;
		case 2:
			CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(_quadro);
			break;
		default:
			printf("Erro na verificacao de erro!\n");
			exit(-1);
			break;
	}
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(datagrama *_quadro){
	int _oneBits = 0;

	// soma-se todos os bits, incluindo o bit de paridade
	for(int i = 0; i < _quadro->_totalLen; i++){
		_oneBits += _quadro->_binData[i];	
	}

	// se nao for impar, ha erro
	if((_oneBits % 2) != 0){
		printf("Erro detectado na transmissao!!\n");
	}

}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(datagrama *_quadro){
	int _oneBits = 0;

	// soma-se todos os bits, incluindo o bit de paridade
	for(int i = 0; i < _quadro->_totalLen; i++){
		_oneBits += _quadro->_binData[i];	
	}

	// se nao for impar, ha erro
	if((_oneBits % 2) != 1){
		printf("Erro detectado na transmissao!!\n");
	}

}

void CamadaEnlaceDadosReceptoraControleDeErroCRC(datagrama *_quadro){

}
