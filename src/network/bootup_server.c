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
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include"../../include/types.h"
#include "../../include/network/bootup_server.h"
#include "../../include/network/service.h"
#include "../../include/fs/fsconfig.h"

#define SECONDS 5


sem_t * dbmutex;

/*
* Name: init_server
* Params: None
* Description: Creates and binds socket to physical address. Then sets a listening queue and waits for connections.
* Return: Error Code when a failures occurs.
*/
int init_server(void)
{
    pthread_t tid;
    int socketfd;
    int ret;
    struct sockaddr_in addr, conaddr;
    unsigned int size=sizeof(struct sockaddr_in);
    char name[TAM];
    char password[TAM];
    sem_t * mutex;
    mode_t mode=0;
    Connection con;
    network_conf conf;
    database_conf dbconf;

    /*Creates a socket file descriptor*/
    socketfd= socket(AF_INET,SOCK_STREAM, 0);
    if(socketfd == ERR){
        return ERR_SOCKFD;
    }

    syslog(LOG_DAEMON, "Loading database configuration" );
    ret=DBUserRead(&dbconf);
    if(ret == ERR) {
      syslog(LOG_DAEMON, "Error loading database configuration. Aborting" );
      return ERR_DATABASE;
    }

    syslog(LOG_DAEMON, "Database configuration loaded");

    syslog(LOG_DAEMON, "Socket opened" );
    conf=NetworkConfigRead();

    /*Sets address for the socket.*/
    memset(&addr, 0, sizeof(struct sockaddr_in));/*Clear struct*/

    addr.sin_family = AF_INET; //TCP
    addr.sin_port = htons(conf.port);
    addr.sin_addr.s_addr = INADDR_ANY;

    /*Binds socket to physical address*/
    ret =bind(socketfd, (struct sockaddr *) &addr, size);
    if(ret == ERR){
        close(socketfd);
        return ERR_BIND;
    }

    /*Sets listening queue*/
    ret= listen(socketfd, conf.listen_queue);
    if(ret == ERR){
        close(socketfd);
        return ERR_LISTEN;
    }
    syslog(LOG_DAEMON, "Creating Mutexes" );

    /*Creates mutex for connection setup process*/
    mutex =sem_open("connection_mutex", O_CREAT,mode,MUTEXVAL);
    if(mutex == SEM_FAILED){
        close(socketfd);
        return ERR_MUTEX;
    }

    /*Creates mutex for database access*/
    dbmutex =sem_open("database_mutex", O_CREAT,mode,MUTEXVAL);
    if(dbmutex == SEM_FAILED){
        sem_close(mutex);
        close(socketfd);
        return ERR_MUTEX;
    }

    sem_getvalue(dbmutex, &ret);
    if(ret == 0){
      sem_post(dbmutex);
    }
    con.semid = mutex;
    con.dbConf=dbconf;

    while(1){
        /*Accepts connection*/
        syslog(LOG_DAEMON, "Accept queue");
        ret=accept(socketfd, (struct sockaddr *) &conaddr,&size);
        if(ret == ERR){
            close(socketfd);
            sem_unlink("connection_mutex");
            sem_close(mutex);
            return ERR_ACCEPT;
        }

        con.sockfd =ret;

        /*Creates service function thread*/
        pthread_create(&tid, NULL, &init_servicing, (void *)&con);

        /*Remains locked until thread copies params*/
        sem_wait(mutex);
      }
    return OK;
}

/*
* Name: server_bootup_log
* Params: None
* Description: Prints error causes during server boot if there were any
* Return: Nothng
*/
void server_bootup_log(void)
{
    int i = init_server();
    /*Switch to notify error source*/
    switch(i){
        default:
        return;
    }
    i = errno; /*What was its cause*/
    /*Switch to notify errno failure*/
    switch(i){
        default:
        return;
    }

}
