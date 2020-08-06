#ifndef CONFIG_H_
#define CONFIG_H

int checkConfig(void);
int create_config(void);
int add_server(char * server);
int remove_server(char * server);
int add_user(char * username, char * password);
int remove_user(char * username);

#endif
