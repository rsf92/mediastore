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

#ifndef FSHANDLE_H
#define FSHANDLE_H_

#include <dirent.h>
#include <unistd.h>
#include "../const.h"

typedef struct directory_t{
  char filename[TAM];
  struct directory_t * next;
}dir;

int fs_create_dir(char * dirname);
dir * fs_explore_dir(char * dirname);
int fs_remove_dir(char * dirname);

int fs_add_dir(dir ** dir_list,char * filename);
int fs_search_dir(dir ** dir_list,char * filename);
int fs_delete_dir(dir ** dir_list,char * filename);


#endif /*FSHANDLE_H_*/
