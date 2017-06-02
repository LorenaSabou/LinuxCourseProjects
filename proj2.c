#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

/*
For each command line argument launch two subprocesses that will race to establish:
- the length of the longest line, if the argument is a file (using popen and wc).
- the size in bytes if the argument is a directory (using popen and du). 
Each process, before send the result to the parent, will sleep between one and five seconds (using sleep call).
The communication between processes we will use a single pipe channel.
Each process will send to the parent a structure that contains his pid, the argument and the established result.
The parent will print only the n/2 received results (n being the number of arguments), the rest will be ignored.
*/

typedef struct{
	int pid;
	int arg;
	int result;
}Result;
/*The structure that the child will send to the parent contains only the index in the arguments array,not the argument itself
This way it is easier to extract from pipe results which do not intercalate*/

int main(int argc,char* argv[]){
	int p1,p2,fd[2],i;
	FILE* f;

	if (pipe(fd) == -1) {
		perror("Error.Can't create pipe.\n");
		exit(1);
	}

	p1=fork();
	if ( p1 > 0 ){
		//parent process creates the second child
		p2 = fork();
	}

	if ( p1 == 0 || p2 == 0 ){
		Result r;
		struct stat sb;
		int secondsToSleep;
		close(fd[0]);
		r.pid=getpid();

		for ( i=1 ; i < argc ; i++ ){
			char cmd[100],rez[100];
			r.arg=i;
			if (stat(argv[i],&sb)==1){
				perror("Error at stat function.\n");
				exit(EXIT_FAILURE);
			}
			if ((sb.st_mode & S_IFMT ) ==S_IFREG)
			{
				strcpy(cmd,"wc -L  ");
				strcat(cmd,argv[i]);
				strcat(cmd,"| awk \'{print $1;}\' ");
				f=popen(cmd,"r");
			}
			else if ((sb.st_mode& S_IFMT) ==S_IFDIR)
			{
				strcpy(cmd,"du ");
				strcat(cmd,argv[i]);
				strcat(cmd," | awk \'{print $1;}\' ");
				f=popen(cmd,"r");
			}
			//extracting the result from the stream 
			if (fgets(rez,sizeof(rez),f)!=NULL){
				r.result=atoi(rez);
			}
			else
			{
				r.result=0;
			}
			
			pclose(f);
			srand(time(0));
			secondsToSleep= rand() %5 +1;
			sleep(secondsToSleep);
			write(fd[1],&r,sizeof(Result));
		}
		close(fd[1]);
		exit(0);
	}
	//PARENT PROCESS
	else{
	close(fd[1]);
	int printed=0;

	for (i=1 ; i < argc ;i++){
		Result r;
		read(fd[0],&r,sizeof(Result));
		if (printed <= (argc-1)/2){
			if (r.arg==i){
			printf("PID : %d  RESULT: %d  ARGUMENT: %s\n",r.pid,r.result,argv[i]);
			printed++;}
		}
	}


	close(fd[0]);
	printf("Parent: I am waiting for a child...\n");
	wait(0);
	printf("Parent: I am waiting for the other child...\n");
	wait(0);
	printf("Parent: done\n");
	}
	return 0;
}

				
