/* Copyright 2018 Roberto Sánchez

    This file is part of mediaServer.

    mediaServer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mediaServer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with mediaServer.  If not, see <http://www.gnu.org/licenses/>.*/
#ifndef TYPES_H_
#define TYPES_H_

#include <semaphore.h>
#include <stdbool.h>
#include "const.h"

typedef struct database_conf{
  char username[SHORT_TAM];
  char password[SHORT_TAM];
  char host[SHORT_TAM];
  char database[SHORT_TAM];
}database_conf;

typedef struct connection_s {
  sem_t * semid;
  int sockfd;
  database_conf dbConf;
}Connection;

typedef struct conf_var_s{
  char variable[TAM];
  char value[TAM];
}Config;

typedef struct network_conf{
  short unsigned int port;
  int listen_queue;
  bool https;
}network_conf;


#endif /*TYPES_H_*/
