#ifndef __TRANSMISSOR_H__
#define __TRANSMISSOR_H__

#include "misc.h"

void AplicacaoTransmissora();

void CamadaDeAplicacaoTransmissora(char *_mensagem);

void CamadaEnlaceDadosTransmissora(datagrama *_quadro);

void CamadaEnlaceDadosTransmissoraControleDeErro(datagrama *_quadro);

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(datagrama *_quadro);

void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(datagrama *_quadro);

void CamadaEnlaceDadosTransmissoraControleDeErroCRC(datagrama *_quadro);

#endif
