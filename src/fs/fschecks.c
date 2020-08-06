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

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/util/util.h"
#include "../../include/fs/fschecks.h"


int check_root(void){

  bool b;
  char dir[TAM]=ROOT_DIR;

  b=dir_exists(dir);
  if(b == false)
    return ERR;

  return OK;
}

int check_config(void){

  bool b;
  char dir[TAM]=CONFIG_DIR;

  b=dir_exists(dir);
  if(b == false)
    return ERR;

  return OK;
}

int check_network(void){

  bool b;
  char dir[TAM]=CONFIG_DIR;

  b=dir_exists(dir);
  if(b == false)
    return ERR;

  return OK;
}

int check_DB(void){

  bool b;
  char dir[TAM]=DB_DIR;

  b=dir_exists(dir);
  if(b == false)
    return ERR;

  return OK;
}

int all_checks_fs(void){

  int ret;

  chdir(getenv("HOME"));

  ret =check_root();
  if(ret == ERR)
    return ERR;

  ret =check_config();
  if(ret == ERR)
    return ERR;

  ret =check_DB();
  if(ret == ERR)
    return ERR;

  ret =check_network();
  if(ret == ERR)
    return ERR;

  return OK;
}
