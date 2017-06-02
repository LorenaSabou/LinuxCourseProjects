#!/bin/bash

#TASK: read user names from keyboard and for each of them output:
#     -its full name
#     -the number of processes
#     -the current running command

while true
do
 printf "\n"
 echo "Please enter a user name or EXIT to exit the program:"
 read user

           #check for exit
 if [ "$user" = "EXIT" ] || [ "$user" = "exit" ];then
     exit 1
 fi


           #check if user name is valid
 users="$(compgen -u)"
 count=0
 for name in $users;do
    if [ "$name" = "$user" ];then
	count=`expr $count + 1 `
    fi
 done

 if [ $count -eq 0 ];then 
    echo "The user $user does not exist.Try again!"
    continue
 fi 
 
 #I have used the grep filter to get rid of the header
 FULL_NAME=`finger -s | grep "$user" | sed "s/$user *\([a-zA-Z].*\) tty.*$/\1/"`
 echo "Full name: $FULL_NAME"

 NO_OF_PROC=`ps -u $user | wc -l`
 echo "Number of processes: $NO_OF_PROC"

 CURRENT=`w -h $user | awk '{print $8}'`
 echo "Current process: $CURRENT"

done
