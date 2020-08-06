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
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../../include/const.h"
#include "../../include/cmd/cmdstd.h"

void teststackCommands(void);
void testUnstackCommands(void);

int main (void){

  teststackCommands();
  testUnstackCommands();

  printf("[OK]\n");
  return OK;
}

void teststackCommands(void){

  char * pointer;
  char commands[5][MAXMESSAGE];

  strcpy(commands[0],"LOGIN Roberto AERRDFAE");
  strcpy(commands[1],"DISCONNECT");

  pointer = stackCommands(0);
  assert(pointer == NULL);

  pointer = stackCommands(1, commands[0]);
  assert(pointer != NULL);
  assert(strcmp(pointer,"LOGIN Roberto AERRDFAE;") == 0);

  pointer = stackCommands(2, commands[0], commands[1]);
  assert(pointer != NULL);
  assert(strcmp(pointer,"LOGIN Roberto AERRDFAE;DISCONNECT;") == 0);

  destroyStackedCmd(pointer);
}

void testUnstackCommands(void){

  char ** pointer;
  int ncommands;
  char commands[MAXMESSAGE]="LOGIN Roberto AERRDFAE;DISCONNECT";

  pointer=unStackCommands(NULL,NULL);
  assert(pointer == NULL);

  pointer=unStackCommands(commands,NULL);
  assert(pointer == NULL);

  pointer=unStackCommands(commands,&ncommands);
  assert(pointer != NULL);
  assert(ncommands == 2);

  destroyCmdArray(NULL,&ncommands);
  assert(ncommands == 2);

  destroyCmdArray(pointer,&ncommands);
  assert(ncommands == 0);

  fgets(commands, MAXMESSAGE, stdin);

  pointer=unStackCommands(commands,&ncommands);
  assert(pointer != NULL);
  assert(ncommands == 2);

  destroyCmdArray(pointer,&ncommands);
  assert(ncommands == 0);
}
