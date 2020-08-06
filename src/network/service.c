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
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <semaphore.h>
#include "../../include/network/service.h"
#include "../../include/db/dbstd.h"
#include "../../include/db/dbuser.h"
#include"../../include/cmd/cmdstd.h"
#include"../../include/user/user.h"
#include"../../include/const.h"
#include"../../include/types.h"

extern sem_t * dbmutex;

void * init_servicing(void * connection){

  char msg[MAXMESSAGE]="Welcome\n";
  char name[MAXMESSAGE];
  char mail[MAXMESSAGE];
  char password[MAXMESSAGE];
  char date[MAXMESSAGE];
  char *stack;
  char ** commands;
  int numCommands;
  int numCommandsToSend;
  size_t size=strlen(msg);
  int sockfd;
  Connection con;
  int i;
  int ret;
  ssize_t numBytes;
  CmdType cmdRecv;
  CmdType cmdSend;
  User * current= NULL;
  OpReturn opRet;
  PGconn * conn;

  if(connection == NULL){
    return NULL;
  }

  con = *((Connection *)connection);

  sem_post(con.semid); //Connection data is read, next connection can modify variable

  syslog(LOG_DAEMON, "Connected");

  conn= DBConnect(con.dbConf);
  if(conn == NULL){
    syslog(LOG_DAEMON, "Error, impossible to connect to database. Aborting");
    return NULL;
  }

  syslog(LOG_DAEMON, "Connection to database stablished");
  sockfd = con.sockfd;

  /*Send hello command*/
  send(sockfd,(void *)msg, size,0);

  /*Wait for received commands*/
  while ((numBytes = recv(sockfd,(void *)msg, MAXMESSAGE,0)) != -1) {
;
    if(numBytes > MIN_COMMAND){

      commands=unStackCommands(msg, &numCommands); /*Unstack commands received*/

      if(commands == NULL){ /*If failure, send error return to receive loop*/
        /*TODO Send error message*/
        cmdSend=OPERATION_UNKNOWN;
        cmdCreate(cmdSend,msg);
        size=strlen(msg);
        send(sockfd,(void *)msg, strlen(msg),0);

      }
      else{
        for(i=0; i< numCommands;i++){
          cmdRecv=cmdDetect(commands[i]);
          numCommandsToSend =0;
          switch(cmdRecv){

            case DISCONNECT:
              syslog(LOG_DAEMON, "User Disconnects" );
              cmdSend=DISCONNECT;
              cmdCreate(cmdSend,msg);
              stack= stackCommands(++numCommandsToSend,msg);
              send(sockfd,(void *)stack, strlen(stack),0);

              /*Clean resources used by thread*/
              destroyStackedCmd(stack);
              destroyCmdArray(commands, &numCommands);
              close(sockfd);
              return NULL;
            case REGISTER:

              opRet = OPOK;
              syslog(LOG_DAEMON, "Register command" );
              cmdSend=REGISTER;
              cmdParse(cmdSend,commands[i],&mail,&password, &name, &date);
	            //If a user has already logged in this server's thread, there can be no other register operations on this thread
              if(current == NULL){

                sem_wait(dbmutex);
                current = getUser(con.dbConf, mail);
                sem_post(dbmutex);

                if(current == NULL){
                  sem_wait(dbmutex);

                  ret = add_user(con.dbConf, mail, password, name, date);
                  sem_post(dbmutex);

                  if(ret == ERR){
                    opRet = ERR_UNKNOWN_ERR;
                  }else{
                    opRet = OPOK;
                  }
                }
                else{
                  opRet = ERR_ALREADY_REGISTERED;
                }
              }else{
                opRet = ERR_ALREADY_LOGGEDIN;

              }

              cmdCreate(cmdSend,msg, opRet);

              stack= stackCommands(++numCommandsToSend,msg);

              break;
            default:
              syslog(LOG_DAEMON, "Unknown command" );
              cmdSend=OPERATION_UNKNOWN;
              cmdCreate(cmdSend,msg);
              stack= stackCommands(++numCommandsToSend,msg);


              break;
          }
          send(sockfd,(void *)stack, strlen(stack),0);
          destroyStackedCmd(stack);

        }
        destroyCmdArray(commands, &numCommands);
      }
    }
  }
  return NULL;
}
