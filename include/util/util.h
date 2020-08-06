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
#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include "../types.h"

int parseConfig(char * configVar,Config * conf);
void daemonize(int (*main_function)(void));
struct tm * parseDate(char *date);
bool isDateOk(char * date);
bool isMailOk(char * mail);
bool isNameOk(char * name);
bool dir_exists(char * directory);
int upperize(char * string);
bool isPasswordOk(char * password);
#endif
