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
#include <stdio.h>
#include "../../include/db/dbstd.h"
#include "../../include/fs/fsconfig.h"
#include"../../include/const.h"

/*
* Name: testDBUser
* Params:
* dbConf: Set of values required for connecting to the database
* Description: Tests connection to the data base with the user provided. If successful, closes connection
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
int testDBUser(database_conf dbConf) {

    int ret = ERR;



    return ret;
}

/*
* Name: DBConnect
* Params:
* dbConf: Set of values required for connecting to the database
* Description: Connects to database
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
PGconn * DBConnect(database_conf dbConf) {

    PGconn * conn = NULL;
    char connectStr[TAM];

    sscanf (connectStr, "host=%s dbname=%s user=%s password=%s" ,dbConf.host, dbConf.database,dbConf.username,dbConf.password);

    conn = PQconnectdb(connectStr);

    if (PQstatus(conn) == CONNECTION_BAD) {

        DBDisconnect(conn);
        conn=NULL;
    }

    return conn;
}

/*
* Name: DBDisconnect
* Params:
* Description: Disconnects from database
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
int DBDisconnect(PGconn *conn) {

    PQfinish(conn);
    return OK;
}

/*
* Name: DBExecuteQuery
* Params:
* query: Query to execute
* Description: Executes a SQL query
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
int DBExecuteQuery(char * query) {


    return OK;

}

/*
* Name: DBGetResults
* Params:
* ncols: Number of columns in the query
* values: Value of each row in the query
* Description: Gets a row of the result from a query
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
int DBGetResults(int ncols, char ** values) {



    return OK;
}

/*
* Name: insertRow
* Params:
* table: Table in which the row will be inserted
* values: Variable list with the values to be inserted
* Description: Inserts a row of given values into the passed row
* Return: ERR if any error ocurred, otherwise it shall return OK
*/
int insertRow(char * table, ...){
  int ret=ERR;

  return ret;
}
