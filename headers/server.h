#ifndef __SERVER_H__
#define __SERVER_H__

#include "game_engine.h"

int server();

// Funcoes para receber parametros basicos do teclado

void get_b_size(g_structure *game);

void get_s_size(g_structure *game);	

void get_p_num(g_structure *game);

// Funcoes para criar estruturas de dados

void b_create(g_structure *game);

void p_create(g_structure *game);

// Threads do host e do cliente

void *host_handler(void *arg);

void *client_handler(void *arg);

// Funcao para receber o ip do teclado

int read_ip(char *ip);

// Funcoes para enviar dados

int send_basic_info(int c_socket, cbi *client_basic_info);

int recv_last_play(int c_socket, lp *last_play);

int send_last_play(int c_socket, lp *last_play);

#endif
