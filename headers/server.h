#ifndef __SERVER_H__
#define __SERVER_H__

#include "game_engine.h"

int server();

void get_b_size(g_structure *game);

void get_s_size(g_structure *game);	

void get_p_num(g_structure *game);

void b_create(g_structure *game);

void p_create(g_structure *game);

void *host_handler(void *arg);

void *client_handler(void *arg);

int read_ip(char *ip);
#endif
