#ifndef __RECEPTOR_H__
#define __RECEPTOR_H__

#include "misc.h"

void AplicacaoReceptora(unsigned char *_str);

void CamadaDeAplicacaoReceptora(datagrama *_quadro);

void CamadaEnlaceDadosReceptora(datagrama *_quadro);

void CamadaEnlaceDadosReceptoraControleDeErro(datagrama *_quadro);

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(datagrama *_quadro);

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(datagrama *_quadro);

void CamadaEnlaceDadosReceptoraControleDeErroCRC(datagrama *_quadro);

#endif
