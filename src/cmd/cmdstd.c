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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/cmd/cmdstd.h"
#include "../../include/util/util.h"
#include "../../include/strings.h"
/**
 * @file mediaServer/cmdstd
 *
 * @page cmdstd \b cmdstd
 *
 * @brief \b cmdstd - Functions for basic operation on commands, either received or to be sent
 *
 * @section description DESCRIPTION
 *
 * This module provides procedures to cover every operation needed in processing
 * or preprocessing of commands, either upon reception or just before transmission.
 *
 * @section authors AUTHOR
 * Roberto Sánchez Fernández(roberto.sanchez@openmailbox.org)
*/

void destroyStackedCmd(char * commands){

  if(commands != NULL){
      free(commands);
  }
}

void destroyCmdArray(char ** commands, int * ncommands){

  int i;

  if(commands != NULL && ncommands != NULL){

      for(i=0; i<*ncommands;i++){
        free(commands[i]);
      }

      free(commands);

      *ncommands =0;
  }


}

char * stackCommands(int ncommands, ...){

  char * stacked_command= NULL;
  int i;
  size_t tam=1; // For the /0 token
  va_list args;

  if(ncommands == 0)
    return NULL;

  va_start(args, ncommands);

  for(i = 0; i < ncommands; i++){
    tam += strlen(va_arg(args, char *));
    tam++; /* For the ; character*/
  }

  va_end(args);

  stacked_command = (char *) calloc(tam, sizeof(char));
  if(stacked_command == NULL){
    return NULL;
  }

  va_start(args, ncommands);

  for(i = 0; i < ncommands; i++){
    strcat(stacked_command,va_arg(args, char *));
    strcat(stacked_command, ";");

  }

  va_end(args);
  return stacked_command;
}

char ** unStackCommands(char * commands, int * ncommands){

  size_t i=0;
  char ** commandArray,** commandArray2;
  char buffer[MAXMESSAGE], aux[MAXMESSAGE];
  char * pointer=aux;

  if(commands == NULL || ncommands == NULL){
    return NULL;
  }

  strcpy(aux,commands);

  commandArray= (char **) malloc(sizeof(char *));
  if(commandArray == NULL){
      return NULL;
  }

  commandArray[0]= (char *) calloc(TAMCMD, sizeof(char));
  if(commandArray[0] == NULL){
      free(commandArray);
      return NULL;
  }
  strcpy(commandArray[0], strtok_r(aux, ";\r\n\0", (char **)&buffer));
  i++;

  while((pointer = strstr(pointer, ";")) != NULL){

    commandArray2= (char **) realloc(commandArray,(i+1) * sizeof(char *));
    if(commandArray2 == NULL){
        *ncommands=(int)i;
        return commandArray;
    }
    commandArray=commandArray2;

    commandArray[i]= (char *) calloc(TAMCMD, sizeof(char));
    strcpy(commandArray[i], strtok_r(NULL, ";\r\n\0", (char **)&buffer));
    i++;

  };

  /*In case there was more than one command stacked*/
  if(strstr(commands, ";") != NULL){

    commandArray2= (char **) realloc(commandArray,(i+1) * sizeof(char *));
    if(commandArray2 == NULL){
        *ncommands=(int)i;
        return commandArray;
    }
    commandArray=commandArray2;

    commandArray[i]= (char *) calloc(TAMCMD, sizeof(char));
    strcpy(commandArray[i], strtok_r(NULL, ";\r\n\0!", (char **)&buffer));
    i++;
  }
  *ncommands=(int)i;

  return commandArray;
}

CmdType cmdDetect (char * cmd){

  CmdType type = OPERATION_UNKNOWN;
  char cmd2[TAM], cmd_aux[TAM];
  int ret;

  if(cmd != NULL){

    strcpy(cmd_aux, cmd);
    strcpy(cmd2, strtok(cmd_aux, " "));

    ret =upperize(cmd2);
    if(ret == OK){
      if(strstr(cmd2, LOGINCMD) != NULL)
        type = LOGIN;
      else if(strstr(cmd2, REGISTERCMD) != NULL)
        type = REGISTER;
      else if(strstr(cmd2, DISCONNECTCMD) != NULL)
        type = DISCONNECT;
      else{
        type = OPERATION_UNKNOWN;
      }
    }
  }

  return type;
}

int cmdCreate(CmdType type,char * cmd, ...){

  va_list param_list;
  OpReturn ret;

  if(cmd == NULL){
    return ERR;
  }

  va_start(param_list, cmd);

  switch(type){
    case OPERATION_UNKNOWN:
      strcpy(cmd, UNRECOG_CMD);
      break;
    case DISCONNECT:
      strcpy(cmd, DISCONNECT_OK);
      break;
    case LOGIN:
      break;
    case REGISTER:
        ret=va_arg(param_list, OpReturn); /*Get return pointer*/

        switch (ret) {
          case ERR_UNKNOWN_ERR:
            strcpy(cmd, ERR_USERINFO);
            break;
          case ERR_ALREADY_REGISTERED:
            strcpy(cmd, ERR_REG);
              break;
          case ERR_ALREADY_LOGGEDIN:
            strcpy(cmd, ERR_LOGGEDIN);
              break;
          default:
            strcpy(cmd, REG_OK);
        }
        break;
    default:
      break;
  }

  va_end(param_list);

  return OK;
}

int cmdParse(CmdType type,char * cmd, ...){

  va_list param_list;
  int retValue=OK;
  char * StringParam=NULL;
  char cmdCopy[MAXMESSAGE];
  char buffer[MAXMESSAGE];

  if(cmd == NULL){
    return ERR;
  }

  strcpy(cmdCopy,cmd);
  strtok_r(cmdCopy, " \n",(char **)&buffer);

  va_start(param_list, cmd);

  switch (type){
    case LOGIN:

      StringParam=va_arg(param_list, char *);/*Get mail pointer*/
      if(StringParam == NULL){
        retValue=ERR;
        break;
      }

      strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy mail*/

      StringParam=va_arg(param_list, char *); /*Get password pointer*/
      if(StringParam == NULL){
        retValue=ERR;
        break;
      }

      strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy password*/

      break;
    case REGISTER:

        StringParam=va_arg(param_list, char *);/*Get mail pointer*/
        if(StringParam == NULL){
          retValue=ERR;
          break;
        }

        strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy mail*/

        StringParam=va_arg(param_list, char *); /*Get password pointer*/
        if(StringParam == NULL){
          retValue=ERR;
          break;
        }

        strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy password*/

        StringParam=va_arg(param_list, char *); /*Get name pointer*/
        if(StringParam == NULL){
          retValue=ERR;
          break;
        }

        strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy name*/

        StringParam=va_arg(param_list, char *); /*Get date pointer*/
        if(StringParam == NULL){
          retValue=ERR;
          break;
        }

        strcpy(StringParam,strtok_r(NULL, " \n",(char **)&buffer)); /*Copy date*/

        break;
    default:
      break;
  }

  va_end(param_list);

  return retValue;
}
