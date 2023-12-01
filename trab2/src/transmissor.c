#include<stdio.h>
#include<stdlib.h>

#include "../headers/transmissor.h"
#include "../headers/receptor.h"
#include "../headers/misc.h"

void AplicacaoTransmissora(){
	char _mensagem[1024];

	printf("Digite a mensagem a ser enviada:\n");

	// Recebe as mensagens do transmissor e repassa elas para o 
	fgets(_mensagem, 1023, stdin);

	CamadaDeAplicacaoTransmissora(_mensagem);	
	
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

	// Chama a proxima camada
	CamadaEnlaceDadosReceptora(_quadro);

	// Libera memoria
	free(_quadro->_binData);
	free(_quadro);
}

void CamadaEnlaceDadosTransmissoraControleDeErro(datagrama *_quadro){
	printf("Digite o tipo de erro desejado:\n0 - CRC32\n1 - Bit de paridade impar\n2 - Bit de paridade par\n\n");
	scanf("%d", &(_quadro->_tipoDeControleDeErro));

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
		default:
			printf("Valor invadido!\n");
			exit(-1);
			break;
	}
}

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(datagrama *_quadro){
	uint8_t *_aux = NULL;
	_aux = malloc(sizeof(uint8_t)*(_quadro->_binDataLen + 1));
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
	uint8_t *_aux = NULL;
	_aux = malloc(sizeof(uint8_t)*(_quadro->_binDataLen + 1));

	_quadro->_totalLen = _quadro->_binDataLen + 1;

	int _oneBits = 0;

	CopiaDadosBin(_quadro, _aux);

	for(int i = 0; i < _quadro->_binDataLen; i++){
		_oneBits += _quadro->_binData[i];
	}
	if((_oneBits % 2) == 1){
		_aux[_quadro->_totalLen - 1] = 0;
	} else {
		_aux[_quadro->_totalLen - 1] = 1;
		
	}

	free(_quadro->_binData);
	_quadro->_binData = _aux;
}

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(datagrama *_quadro){
	// Polinomio gerador (CRC-32)
        int _polinomioGerador[TAMANHO_CRC] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1};

	_quadro->_totalLen = _quadro->_binDataLen + TAMANHO_CRC;

	uint8_t *_aux = NULL;
	_aux = malloc(sizeof(uint8_t) * (_quadro->_totalLen));
	
	CopiaDadosBin(_quadro, _aux);

	for(int i = _quadro->_binDataLen; i < (_quadro->_totalLen); i++){
		_aux[i] = 0;
	}

	for(int i = 0; i < _quadro->_binDataLen; i++){
		if(_aux[i] == 1){
			for(int j = 0; j < TAMANHO_CRC; j++){
				_aux[i+j] = (_aux[i+j] ^ _polinomioGerador[j]);
			}
		}
	}

	CopiaDadosBin(_quadro, _aux);

	free(_quadro->_binData);
	_quadro->_binData = _aux;
}
















