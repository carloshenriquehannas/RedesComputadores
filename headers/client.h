#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "game_engine.h"

// Funcao basica de comunicacao
int client();

// Funcao para receber o ip do teclado
int get_lobby_code(char *ip);

// Funcao de comunicacao para receber as informacoes basicas
int recv_basic_info(int c_socket, cbi *client_basic_info);

#endif
