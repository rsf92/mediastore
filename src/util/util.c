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
/**
 * @file mediaServer/util
 *
 * @page util \b util
 *
 * @brief \b util - Utilities functions.
 *
 * @section description DESCRIPTION
 *
 * This module provides procedures to cover multiple simple utilities required
 * along the code, in a centalised manner to make code maintenance simpler
 *
 * @section authors AUTHOR
 * Roberto Sánchez Fernández(roberto.sanchez@openmailbox.org)
*/
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include "../../include/util/util.h"
#include "../../include/const.h"

#define MAXMAIL 30
#define MAXPASS 30
#define DATELEN 10

/*
* Name: daemonize
* Params:
* main_function: Function to be called after daemonizing
* Description: Creates a daemon process and starts the server
* Return: Nothing
*/
void daemonize(int (*main_function)(void)) {
    pid_t pid;
    int sid;

    pid = fork(); /*We create a new process*/

    if (pid < 0) {
        exit(-1);
    } else if (pid != 0)
        exit(0); /*Then we kill it, so that the whole server runs in background*/

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(1);
    }

    chdir("/");

    close(STDIN_FILENO); /*we close every open file so that the server doesn't interfere with any running process in that terminal*/
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    openlog(NAME, LOG_PID, LOG_USER); /*We create a log entry to notify errors and important events within the server*/
    sleep(1);

    main_function(); /*Start proper daemon execution*/

}

/*
* Name: isDateOk
* Params:
* date: Date string to be checked
* Description: Checks that a given string date is correctly formatted  as dd-mm-yyyy
* Return: True if the date is correctly formatted, otherwise it returns false
*/

bool isDateOk(char * date){

  char day[4];
  char month[4];
  char year[6];
  int dayval;
  int monthval;
  int yearval;
  int maxVal;
  int i;
  char tempDate[DATELEN+1];
  char buffer[MAXMESSAGE];

  if(date == NULL){
    return false;
  }

  if(strlen(date) != DATELEN || date[2] != '-' || date[5] != '-'){
    return false;
  }

  strcpy(tempDate, date);

  strcpy(day, strtok_r(tempDate,"-",(char **)&buffer));
  strcpy(month, strtok_r(NULL,"-",(char **)&buffer));
  strcpy(year, strtok_r(NULL,"\n",(char **)&buffer));

  for(i=0; i< 2; i++){
    if((isdigit(day[i]) && isdigit(month[i])) == 0)
        return false;
  }

  for(i=0; i< 4; i++){
    if(isdigit(year[i]) == 0)
        return false;
  }

  dayval=atoi(day);
  monthval=atoi(month);
  yearval=atoi(year);

  if( dayval> 31 || dayval < 1){
    return false;
  }else if(monthval > 12 || monthval < 1){
    return false;
  }else if(yearval < 1900){
    return false;
  }

  switch(monthval){
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if(dayval == 31){
        return false;
      }
      break;
    default:

      maxVal = 28;
      if(yearval %4 == 0){
          if(yearval %100 == yearval %400){
              maxVal = 29;
          }
      }

      if( dayval> maxVal){
        return false;
      }
  }

  return true;
}

/*
* Name: parseDate
* Params:
* date: String containing the date to be translated.
* Description: Turns a date string into a struct tm.
* Return: This function returns the converted struct, NULL if any error occurred.
*/

struct tm * parseDate(char *date){

  time_t rawtime={0};
  struct tm * time;
  char newDate[DATELEN+1];
  char buffer[MAXMESSAGE];
  /*If date is NULL, all values set to ERR, otherwise perform operation*/
  if(date == NULL){
    return NULL;
  }

  strcpy(newDate,date);

  time = localtime( &rawtime );

  if (isDateOk(date) == true){
    time->tm_mday=atoi(strtok_r(newDate,"-",(char **)&buffer));
    time->tm_mon=atoi(strtok_r(NULL,"-",(char **)&buffer));
    time->tm_year=atoi(strtok_r(NULL,"\0",(char **)&buffer));
  }else{
      return NULL;
  }

  return time;
}

/*
* Name: isMailOk
* Params:
* mail: String containing the mail to validate
* Description: This function validates the correct format of an email address.
* Return: This function returns true upon correct execution, false otherwise.
*/
bool isMailOk(char * mail){

  char * at;

  /*If NULL pointer, return false*/
  if(mail == NULL){
    return false;
  }
  if(strlen(mail) > MAXMAIL){
    return false;
  }
  /*Look for @, if not found, return false*/
  at=strstr(mail, "@");
  if(at == NULL){
    return false;
  }

  /*Look for . after the @, if not found, return false*/
  if(strstr(at, ".") == NULL){
    return false;
  }

  /*Look for . after the @, if it's right after @, return false*/
  if(strstr(at, ".") == (at+1)){
    return false;
  }

  if(*(strstr(at, ".")+1) == '\0'){
    return false;
  }

  return true;
}

/*
* Name: isPasswordOk
* Params:
* password: String containing the password to validate.
* Description: Validates the correct format of a password string.
* Return: This function returns true upon correct execution, false otherwise.
*/
bool isPasswordOk(char * password){

  
  /*If NULL pointer, return false*/
  if(password == NULL){
    return false;
  }
  if(strlen(password) > MAXPASS){
    return false;
  }

  return true;
}

/*
* Name: isNameOk
* Params:
* name: String containing the name to validate
* Description: This function validates the correct format of a name.
* Return: This function returns true upon correct execution, false otherwise.
*/
bool isNameOk(char * name){



  if(name == NULL){
    return false;
  }

  for(size_t i=0; i<strlen(name); i++){
    if(isalpha(name[i]) == 0){
      return false;
    }
  }

  return true;
}

/*
* Name: dir_exists
* Params:
* directory: String containing the directory to validate.
* Description: This function checks wether a directory exists or not in the file system.
* Return: This function returns true upon correct execution, false otherwise.
*/

bool dir_exists(char * directory){
    DIR * root;

    if(directory == NULL)
      return false;

    root=opendir(directory);

    if(root == NULL && errno == ENOENT)
        return false;

    closedir(root);

    return true;
}

/*
* Name: upperize
* Params:
* string: String to be transformed into uppercase.
* Description: This function transforms a string into uppercase.
* Return: This function returns OK upon correct execution, ERR otherwise.
*/
int upperize(char * string){

  int ret = ERR;
  size_t len;

  if(string != NULL){
    len = strlen(string);

    for(size_t i = 0; i< len; i++){
      string[i] = (char)tolower(string[i]);
      string[i] = (char)toupper(string[i]);
    }
    ret = OK;
  }

  return ret;
}

/*
* Name: parseConfig
* Params:
* string: String to parsed.
* Description: This function parses a config file string into a Config variable.
* Return: This function returns the parsed Config.
*/
int parseConfig(char * configVar,Config * conf){

  if(configVar == NULL || configVar[0] == '#')
    return ERR;

  strcpy(conf->variable,strtok(configVar, "="));
  strcpy(conf->value, strtok(NULL, "\r\n"));

  return OK;
}
