#ifndef DBUSER_H_
#define DBUSER_H_

#include <stdbool.h>
#include "../user/user.h"
#include "dbconst.h"
#include "dbstd.h"

int add_user(database_conf dbConf, char * mail, char * password, char * name, char * date);
int toEditor(database_conf dbConf, char * mail);
bool isAdmin(database_conf dbConf, char * mail);
bool isRegistered(database_conf dbConf, char * mail);
int changePassword(database_conf dbConf, char * mail, char * oldpassword, char * newpassword);
User * getUser(database_conf dbConf, char * mail);
#endif
