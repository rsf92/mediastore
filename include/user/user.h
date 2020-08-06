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
#ifndef USER_H_
#define USER_H_

#include <time.h>
#include <stdbool.h>

typedef enum {CUSTOMER, EDITOR, ADMIN} UserType;

typedef struct user_t User;

User * createUser(char * mail, UserType type, char * name, struct tm * date);
void destroyUser(User * user);
char * getMail (User * user);
char * getName (User * user);
struct tm * getDate (User * user);
bool isAdminUser (User user);
bool isEditorUser (User user);
bool isCustomerUser (User user);
#endif
