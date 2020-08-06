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
#include <stdbool.h>
#include <time.h>
#include "../../include/const.h"
#include "../../include/user/user.h"
#include "../../include/util/util.h"

void test_createUser(void);

int main(void){

  test_createUser();

  printf("[OK]\n");
  return 0;
}

void test_createUser(void){
  User * createdUser;
  UserType type=ADMIN;
  char badDate[]="12-13-2017";
  char goodDate[]="12-01-2017";

  createdUser= createUser(NULL, type, NULL, NULL);
  assert(createdUser == NULL);

  createdUser= createUser("roberto", type, NULL, NULL);
  assert(createdUser == NULL);

  createdUser= createUser("roberto", type, "Roberto", NULL);
  assert(createdUser == NULL);

  createdUser= createUser("roberto", type, "Roberto", NULL);
  assert(createdUser == NULL);

  createdUser= createUser("roberto", type, "Roberto", NULL);
  assert(createdUser == NULL);

  createdUser= createUser("roberto", type, "Roberto", parseDate(goodDate));
  assert(createdUser == NULL);

  createdUser= createUser("roberto@hotmail", type, "Roberto", parseDate(goodDate));
  assert(createdUser == NULL);

  createdUser= createUser("roberto@hotmail.", type, "Roberto",  parseDate(goodDate));
  assert(createdUser == NULL);

  createdUser= createUser("roberto@hotmail.es", type, "Rob/rto", parseDate(goodDate));
  assert(createdUser == NULL);

  createdUser= createUser("roberto@hotmail.es", type, "Roberto", parseDate(badDate));
  assert(createdUser == NULL);

  createdUser= createUser("roberto@hotmail.es", type, "Roberto", parseDate(goodDate));
  assert(createdUser != NULL);

  destroyUser(createdUser); //test with valgrind
}
