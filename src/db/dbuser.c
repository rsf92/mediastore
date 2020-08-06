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
#include <syslog.h>
#include"../../include/util/util.h"
#include "../../include/db/dbuser.h"
#include"../../include/const.h"

#define USERDATA 7

int add_user(database_conf dbConf, char * mail, char * password, char * name,  char * date) {

    char query[TAM];

    int ret;

    if (isMailOk(mail) == false || isPasswordOk(password) == false || isNameOk(name) == false ||  isDateOk(date) == false)
        return ERR;

    sprintf(query, "INSERT INTO mms_user values (default,'%s',encrypt_password('%s'),set_type(),'%s',to_date('%s', 'DD-MM-YYYY'), false)", mail, password, name,date);
    DBExecuteQuery(query);

    return OK;
}

int toEditor(database_conf dbConf, char * mail){

  char query[TAM];
  int ret;

  if(mail == NULL)
    return ERR;

  if (isAdmin(dbConf, mail) == false)
    return ERR;

  sprintf(query, "UPDATE  mms_user SET usertype ='%d where usertype = %d", EDITOR, ADMIN);
  ret= DBExecuteQuery(query);


  return ret;
}

bool isAdmin(database_conf dbConf, char * mail){

  char query[TAM];
  char value[1][TAM];

  if(mail == NULL)
    return false;

  sprintf(query, "SELECT usertype FROM  mms_user WHERE mail =%s", mail);
  DBExecuteQuery(query);

  DBGetResults( 1,(char **)value);

  if(atoi(value[0]) == ADMIN)
    return true;

  return false;
}

bool isRegistered(database_conf dbConf, char * mail){
  char query[TAM];

  char value[1][TAM];

  if(mail == NULL)
    return false;

  sprintf(query, "SELECT count(uid) FROM  mms_user WHERE mail =%s", mail);
  DBExecuteQuery(query);

  DBGetResults( 1,(char **)value);



  if(atoi(value[0]) != 0)
    return true;

  return false;
}

int changePassword(database_conf dbConf, char * mail, char * oldpassword, char * newpassword){

  char query[TAM];


  if(mail == NULL || oldpassword == NULL || newpassword == NULL )
    return ERR;

  sprintf(query, "UPDATE mms_user SET passwd = encrypt_password(%s) WHERE mail ='%s' AND password = encrypt_password(%s)", newpassword, mail, oldpassword);
  DBExecuteQuery(query);

  return OK;
}

User * getUser(database_conf dbConf, char * mail){

  UserType type;
  char query[TAM];
  char value[USERDATA][TAM];
  struct tm * date;
  int ret;

  if(mail == NULL)
    return NULL;

  sprintf(query, "SELECT * FROM mms_user WHERE mail ='%s'", mail);
  DBExecuteQuery(query);

  ret = DBGetResults(USERDATA,(char **)value);
  if(ret == ERR)
      return NULL;

  switch(atoi(value[3])){
    case 0:
      type=CUSTOMER;
      break;
    case 1:
      type=EDITOR;
      break;
    case 2:
      type=ADMIN;
      break;
  }

  date= parseDate(value[5]);

  return createUser(mail, type,value[4], date);
}
