#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../headers/transmissor.h"
#include "../headers/misc.h"

#define TAMANHO_CRC 33

void AplicacaoTransmissora(){
	char _mensagem[1024];

	// Recebe as mensagens do transmissor e repassa elas para o 
	while(fgets(_mensagem, 1023, stdin) != NULL){
		CamadaDeAplicacaoTransmissora(_mensagem);	
	}
}

void CamadaDeAplicacaoTransmissora(char *_mensagem){
	datagrama *_quadro = malloc(sizeof(datagrama));
	strToBin(_mensagem, _quadro);

	CamadaEnlaceDadosTransmissora(_quadro);
}

void CamadaEnlaceDadosTransmissora(datagrama *_quadro){
	// Faz controle de erro por parte do transmissor
	// Nesse caso, adiciona os bits de verificacao de erro
	CamadaEnlaceDadosTransmissoraControleDeErro(_quadro);

	// Chama a camada de transporte (meio de transporte)
	MeioDeComunicacao(_quadro);	

	// Libera memoria
	free(_quadro->_binData);
	free(_quadro);
}

void CamadaEnlaceDadosTransmissoraControleDeErro(datagrama *_quadro){
	switch(_quadro->_tipoDeControleDeErro){
		case 2:
			CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(_quadro);
			break;
		case 1:
			CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(_quadro);
			break;
		case 0:
			CamadaEnlaceDadosTransmissoraControleDeErroCRC(_quadro);
			break;
	}


}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(datagrama *_quadro){
	uint8_t *_aux = malloc(sizeof(uint8_t)*(_quadro->_binDataLen + 1));
	_quadro->_totalLen = _quadro->_binDataLen + 1;
	int _oneBits = 0;

	CopiaDadosBin(_quadro, _aux);

	for(int i = 0; i < _quadro->_binDataLen; i++){
		if(_quadro->_binData[i] == 1) _oneBits++;
	}
	if((_oneBits % 2) == 0){
		_aux[_quadro->_totalLen - 1] = 0;
	} else {
		_aux[_quadro->_totalLen - 1] = 1;
		
	}
	free(_quadro->_binData);
	_quadro->_binData = _aux;
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(datagrama *_quadro){
	uint8_t *_aux = malloc(sizeof(uint8_t)*(_quadro->_binDataLen + 1));
	_quadro->_totalLen = _quadro->_binDataLen + 1;
	int _oneBits = 0;

	CopiaDadosBin(_quadro, _aux);

	for(int i = 0; i < _quadro->_binDataLen; i++){
		if(_quadro->_binData[i] == 1) _oneBits++;
	}
	if((_oneBits % 2) == 1){
		_quadro->_binData[_quadro->_totalLen - 1] = 0;
	} else {
		_quadro->_binData[_quadro->_totalLen - 1] = 1;
		
	}
	free(_quadro->_binData);
	_quadro->_binData = _aux;
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(datagrama *_quadro){
	// Polinomio gerador (CRC-32)
        int _polinomioGerador[TAMANHO_CRC] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1};

}
