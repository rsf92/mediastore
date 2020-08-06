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
#include <string.h>
#include "../../include/const.h"
#include "../../include/user/user.h"
#include "../../include/util/util.h"

struct user_t{
  char mail[TAM];
  UserType type;
  char name[TAM];
  struct tm * date;
};

/*
* Name: createUser
* Params:
* mail: User email address, must be correctly formatted
*
* type: Can be EDITOR, ADMIN and CLIENT
*
* name: User name
*
* date: User's date of birth
*
* Description: Allocates memory for a variable of type struct user_t and initializes it with given values.
* Return: This function returns the allocated struct, NULL in case any error happened during the execution of this function.
*/

User * createUser(char * mail, UserType type, char * name, struct tm * date){

  User * new_user;

  if(mail == NULL || name == NULL  || date == NULL)
    return NULL;

  if(isMailOk(mail) == false){
      return NULL;
  }

  if(isNameOk(name) == false){
      return NULL;
  }

  new_user=(struct user_t *)calloc(1, sizeof(struct user_t));
  if(new_user == NULL){
    return NULL;
  }

  strcpy(new_user->mail, mail);
  strcpy(new_user->name, name);

  new_user->type = type;
  new_user->date = date;
  return new_user;
}

/*
* Name: destroyUser
* Params:
* user: Pointer to the struct to be freed. If such pointer is NULL, no action will be performed
*
* Description: This function de-allocates memory for a struct user_t pointer variable.
* Return: This function does not return anything.
*/
void destroyUser(User * user){

  if(user != NULL)
    free(user);

}

/*
* Name: getMail
* Params:
* user: Pointer to the struct to be used.
*
* Description: Getter for the mail field in the struct.
* Return: This function returns a pointer to the mail field on the struct.
*/
char * getMail (User * user){

  if(user == NULL)
    return NULL;

  return user->mail;
}

/*
* Name: isAdminUser
* Params:
* user: Struct to be used.
*
* Description: Checks whether the given user in an admin.
* Return: This function returns a true if the user is an admin, false otherwise.
*/
bool isAdminUser (User user){
  return user.type == ADMIN;
}
/*
* Name: isEditorUser
* Params:
* user: Struct to be used.
*
* Description: Checks whether the given user in an editor.
* Return: This function returns a true if the user is an editor, false otherwise.
*/
bool isEditorUser (User user){
  return user.type == EDITOR;
}
/*
* Name: isCustomerUser
* Params:
* user: Struct to be used.
*
* Description: Checks whether the given user in a customer.
* Return: This function returns a true if the user is a customer, false otherwise.
*/
bool isCustomerUser (User user){
  return user.type == CUSTOMER;
}
/*
* Name: getName
* Params:
* user: Pointer to the struct to be used.
*
* Description: Getter for the name field in the struct.
* Return: This function returns a pointer to the name field on the struct.
*/
char * getName (User * user){
  return user->name;
}
/*
* Name: getDate
* Params:
* user: Pointer to the struct to be used.
*
* Description: Getter for the date field in the struct.
* Return: This function returns a pointer to the date field on the struct.
*/
struct tm * getDate (User * user){
  return user->date;
}
