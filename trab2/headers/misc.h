#ifndef __MISC_H__
#define __MISC_H__

#include<stdint.h>

#define TAMANHO_CRC 33

typedef struct{
	uint8_t *_binData;
	int _binDataLen;
	int _tipoDeControleDeErro;
	int _totalLen;
} datagrama;

void strToBin(char *_str, datagrama *_quadro);

unsigned char *binToStr(datagrama *_quadro);

void MeioDeComunicacao(datagrama *_quadro);

void CopiaDadosBin(datagrama *_quadro, uint8_t *_dest);

#endif
