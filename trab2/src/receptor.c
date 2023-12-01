#include<stdio.h>
#include<stdlib.h>

#include "../headers/receptor.h"

void AplicacaoReceptora(unsigned char *_str){
	printf("Mensagem recebida:\n");

	printf("%s\n", _str);

}

void ChamadaDeAplicacaoReceptora(datagrama *_quadro){
	unsigned char *_str = NULL;

	// converte o binario de volta para string
	binToStr(_str, _quadro);

	AplicacaoReceptora(_str);
}

void CamadaEnlaceDadosReceptora(datagrama *_quadro){
	printf("\nMensagem binaria recebida:\n");

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
		printf("Erro detectado na transmissao!!\n\n");
	}else{
		printf("Mensagem recebida sem erros!!\n\n");
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
		printf("Erro detectado na transmissao!!\n\n");
	}else{
		printf("Mensagem recebida sem erros!!\n\n");
	}
}

void CamadaEnlaceDadosReceptoraControleDeErroCRC(datagrama *_quadro){
	// Polinomio gerador (CRC-32)
        int _polinomioGerador[TAMANHO_CRC] = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1};

	// vetor auxiliar para fazer as divisoes polinomiais
	uint8_t *_aux = NULL;
	_aux = malloc(sizeof(uint8_t) * (_quadro->_totalLen));
	
	// copia os dados do quadro para o vetor auxiliar
	CopiaDadosBin(_quadro, _aux);

	// copia os bits de verificacao do quadro para o vetor auxiliar
	for(int i = _quadro->_binDataLen; i < (_quadro->_totalLen); i++){
		_aux[i] = _quadro->_binData[i];
	}

	// faz a divisao polinomial
	for(int i = 0; i < _quadro->_binDataLen; i++){
		if(_aux[i] == 1){
			for(int j = 0; j < TAMANHO_CRC; j++){
				_aux[i+j] = (_aux[i+j] ^ _polinomioGerador[j]);
			}
		}
	}

	// checa se o resto eh 0
	// se for diferente de zero -> erro na transmissao
	for(int i = _quadro->_binDataLen; i < _quadro->_totalLen; i++){
		if(_aux[i] != 0){
			printf("Erro detectado na transmissao!!\n\n");
			break;
		}
	}

	free(_aux);

}
