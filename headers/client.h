#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "game_engine.h"

int client();

int get_lobby_code(char *ip);

int recv_basic_info(int c_socket, cbi *client_basic_info);

#endif
