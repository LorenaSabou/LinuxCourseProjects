#!/bin/bash

if [ $# -ne 1 ];then
   echo "Error. Usage:"
   echo " The program takes as input one argument,representing the name of a folder/directory and retrieves if changes are taking place inside of it (creation or deletion of files) . Give the name of an existent folder,please!"
   exit 1;
fi

printf "\n"

if [ ! -d $1 ];then
   echo "Error.The input parameter is not a directory.Try again!"
   exit 2;
fi

folder=$1
PREV="" #here we keep track of the modifications(using string comparison).These strings contain all the files in the folder and its subfolers by using the command ls -lR 

while true
do
	if [ -z "$PREV" ];then
	   PREV=`ls -lR $folder`
	   #this is done at the begining,when the string is empty
	fi
	
	sleep 1 #refresh

	NOW=`ls -lR $folder`

	if [ ! "$PREV" = "$NOW" ];then
		echo Modifications have been done on folder $folder
	fi

	PREV=$NOW
done

#we shall run the program in background ,using the following command line:
# ./l11.sh folder_name & / we could use ./l11.sh ~ & to run it on our home dir

# for every modification,the program will notify the user and will expect an ENTER

# in order to KILL the program we must use the following command line:
# kill %1 or by doing a 'ps' command,getting its PID and killing it afterwards

