#! /bin/bash

STATS_FILE=stats.data

if [ $# -gt "1" ]
 then
	STATS_FILE=$1
fi

cloc --out $STATS_FILE ../ 
