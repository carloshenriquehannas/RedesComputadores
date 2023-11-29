#ifndef __MISC_H__
#define __MISC_H__

#include<stdint.h>

typedef struct{
	uint8_t *_binData;
	int _binDataLen;
	uint8_t _tipoDeControleDeErro;
	int _totalLen;
} datagrama;

void strToBin(char *_str, datagrama *_quadro);

void binToStr(char *_str, datagrama *_quadro);

void MeioDeComunicacao(datagrama *_quadro);

void CopiaDadosBin(datagrama *_quadro, uint8_t *_dest);

#endif
