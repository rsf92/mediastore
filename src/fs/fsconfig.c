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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/fs/fsconfig.h"
#include "../../include/fs/fsconst.h"
#include "../../include/network/network.h"
#include "../../include/util/util.h"

int  DBUserRead(database_conf * config){

  FILE * f;
  char str[TAM];
  int ret;
  Config conf;

  if(config == NULL) {

    return ERR;
  }

  f = fopen(DB_CONF_FILE,"r");
  if(f == NULL) {

    return ERR;
  }

  while(fgets(str,TAM,f) != NULL) {

    ret= parseConfig(str, &conf);
    if(ret == OK) {

      if(strcmp(conf.variable,"NAME") == 0) {
          strcpy(config->username, conf.value);
      }
      else if(strcmp(conf.variable,"PASSWD") == 0) {
          strcpy(config->password, conf.value);
      }
      else if(strcmp(conf.variable,"HOST") == 0) {
          strcpy(config->host, conf.value);
      }
      else if(strcmp(conf.variable,"DATABASE") == 0) {
          strcpy(config->database, conf.value);
      }
    }
  }

  return OK;
}

network_conf NetworkConfigRead(void) {

  FILE * f;
  char str[TAM];
  network_conf conf;
  Config config;
  int ret;

  conf.port=PORT;
  conf.listen_queue=LISTEN_BACKLOG;
  conf.https=true;

  f = fopen(DB_CONF_FILE,"r");
  if(f == NULL)
    return conf;

  while(fgets(str,TAM,f) != NULL) {

    ret= parseConfig(str, &config);
    if(ret == OK) {

      if(strcmp(config.variable,"PORT") == 0) {
        conf.port=(short unsigned int)atoi(config.value);
      }
      else if(strcmp(config.variable,"LISTEN_QUEUE") == 0) {
        conf.listen_queue=(int)atoi(config.value);
      }
      else if((strcmp(config.variable,"HTTPS") == 0)) {
        if((strcmp(config.value,"yes") != 0)) {
          conf.https=false;
        }
      }

    }
  }

  return conf;
}
