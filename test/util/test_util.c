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


#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/const.h"
#include "../../include/util/util.h"

void test_isDateOk(void);
void test_parseDate(void);
void test_isMailOk(void);
void test_isNameOk(void);
void test_upperize(void);

int main(void) {

  test_isDateOk();
  test_parseDate();
  test_isMailOk();
  test_isNameOk();
  test_upperize();
  printf("[OK]\n");
  return OK;
}

void test_isDateOk(void){
  bool ret;
  char date[TAM]="01-03-2016";

  ret=isDateOk(NULL);
  assert(ret ==  false);

  ret=isDateOk("01032016");
  assert(ret ==  false);

  ret=isDateOk("0120332016");
  assert(ret ==  false);

  ret=isDateOk(date);
  assert(ret ==  true);

  strcpy(date,"01-13-2016" );
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"01-00-2016");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"00-01-2016");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"31-02-2016");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"29-02-2015");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"AA-BB-CCCC");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"29-02-2016");
  ret=isDateOk(date);
  assert(ret ==  true);

  strcpy(date,"29-02-2100");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"31-04-2100");
  ret=isDateOk(date);
  assert(ret ==  false);

  strcpy(date,"29-02-2400");
  ret=isDateOk(date);
  assert(ret ==  true);

}

void test_parseDate(void){
  struct tm * ret = NULL;
  char date[TAM]="01-03-2016";

  ret=parseDate(NULL);
  assert(ret == NULL);

  ret=parseDate("01032016");
  assert(ret == NULL);

  ret=parseDate("0120332016");
  assert(ret == NULL);

  ret=parseDate(date);
  assert(ret->tm_mday ==  1);
  assert(ret->tm_mon ==  3);
  assert(ret->tm_year ==  2016);

  strcpy(date,"01-13-2016" );
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"01-00-2016");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"00-01-2016");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"31-02-2016");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"29-02-2015");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"AA-BB-CCCC");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"29-02-2016");
  ret=parseDate(date);
  assert(ret->tm_mday ==  29);
  assert(ret->tm_mon ==  2);
  assert(ret->tm_year ==  2016);

  strcpy(date,"29-02-2100");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"31-04-2100");
  ret=parseDate(date);
  assert(ret == NULL);

  strcpy(date,"29-02-2400");
  ret=parseDate(date);
  assert(ret->tm_mday ==  29);
  assert(ret->tm_mon ==  2);
  assert(ret->tm_year ==  2400);
}

void test_isMailOk(void){
  bool b;

  b=isMailOk(NULL);
  assert(b == false);

  b=isMailOk("roberto");
  assert(b == false);

  b=isMailOk("roberto@");
  assert(b == false);

  b=isMailOk("roberto@.");
  assert(b == false);

  b=isMailOk("roberto@algo.");
  assert(b == false);

  b=isMailOk("roberto@algo.es");
  assert(b == true);
}

void test_isNameOk(void){
  bool b;

  b=isNameOk(NULL);
  assert(b == false);

  b=isNameOk("roberto@");
  assert(b == false);

  b=isNameOk("rob/rto@");
  assert(b == false);

  b=isNameOk("roberto.");
  assert(b == false);

  b=isNameOk("robert9");
  assert(b == false);
  b=isNameOk("roberto");
  assert(b == true);
}

void test_upperize(void){
  int b;
  char  str[6]="hola";

  b=upperize(NULL);
  assert(b == ERR);

  b=upperize(str);
  assert(b == OK);
  assert(strcmp(str,"HOLA") == 0);

}
