#ifndef BOOTUP_SERVER_H_
#define BOOTUP_SERVER_H_

#include "network.h"

/*Server bootup specific constants*/
#define ERR_SOCKFD -2
#define ERR_BIND -3
#define ERR_LISTEN -4
#define ERR_ACCEPT -5
#define ERR_NOPIDS -6
#define ERR_MUTEX -7
#define ERR_DATABASE -8
/*Networking specific prototypes*/
void server_bootup_log(void);
int init_server(void);

#endif
