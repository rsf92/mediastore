/*  Copyright 2018 Roberto Sánchez

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
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include"../include/const.h"
#include "../include/network/bootup_server.h"
#include "../include/config/config.h"
#include "../include/util/util.h"


/*
* Name: main
* Params: None
* Description: Starts the server
* Return:OK
*/
int main(void) {

    int ret;

    /*Checks if config files are all created*/
    ret = checkConfig();
    if (ret == ERR) {

      syslog(LOG_DAEMON, "Impossible to launch service! Configuration file missing!" );
      return ERR;

    }

    /*Creates a daemon process*/
    daemonize(init_server);

    return OK;
}
