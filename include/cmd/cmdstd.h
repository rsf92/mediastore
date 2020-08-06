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
#ifndef CMDSTD_H_
#define CMDSTD_H_

#include <stdarg.h>

#include "cmdstr.h"

#define TAMCMD 100

typedef enum {LOGIN, REGISTER,DISCONNECT, OPERATION_UNKNOWN, BAD_MESSAGE, ERROR} CmdType;
typedef enum {OPOK, ERR_UNREGISTERED, ERR_BADLOGIN, ERR_ALREADY_LOGGEDIN, ERR_ALREADY_REGISTERED, ERR_UNKNOWN_ERR} OpReturn;

CmdType cmdDetect (char * cmd);

int cmdCreate(CmdType type,char * cmd, ...);

int cmdParse(CmdType type,char * cmd, ...);

char * stackCommands(int ncommands, ...);

char ** unStackCommands(char * commands, int *ncommands);

void destroyCmdArray(char ** commands, int * ncommands);

void destroyStackedCmd(char * commands);

#endif
