#ifndef DBSTD_H_
#define DBSTD_H_

#include <stdarg.h>
#include <libpq-fe.h>
#include "../types.h"


int testDBUser(database_conf dbConf);
PGconn * DBConnect(database_conf dbConf);
int DBDisconnect(PGconn * conn);
int DBExecuteQuery(char * query);
int DBGetResults(int ncols, char ** values);
int insertRow(char * table, ...);
#endif
