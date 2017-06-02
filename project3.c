/* Implement a program that gets as arguments 
 * a file name followed by words.
 * For each word -> create a separate thread that counts its 
 * appearances in the given file.
 * Print out the sum of the appearances of all words.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct{
	char* filename;
	char* word;
}pair; 
//THIS STRUCTURE IS SENT TO EACH THREAD ACTING AS A PAIR OF FILE->WORD

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
int sum=0; 

void* count_occurences(void* args)
{
	int num=0;
	pair* received = args;
	pthread_mutex_lock(&mtx);
	FILE* fp=fopen(received->filename,"r");
	if (fp==NULL){
	
		printf("Inexistent file\n");
		return NULL;
	}

	char* wordFromFile=(char*)malloc(100*sizeof(char));
	char* word=received->word;
	while ( !feof(fp)){
		fscanf(fp,"%s",wordFromFile);
		if (strcmp(wordFromFile,word)==0) num++;
	}

	fclose(fp);
	sum+=num;
	pthread_mutex_unlock(&mtx);
	return NULL;
}

int main(int argc,char*argv[]){
	if ( argc < 3 )
       	{
	  perror ("Usage: ./output filename word1 word2 etc..");
	  exit(1);
	}
	
	int i;
	pthread_t th[argc-2];
	char* filename=argv[1];

	for( i=2 ; i< argc ; i++ )
	{	
		pair* arguments=malloc(sizeof(pair));
		arguments->filename=filename;
		arguments->word=argv[i];

		if(pthread_create(&th[i-2],NULL,count_occurences,arguments) < 0 ) {
			perror("Error when creating threads.\n");
			exit(1);
		}
	
	}

	for ( i=0 ;i<argc - 2;i++)
	{
		pthread_join(th[i],NULL);
	}

	printf(" The sum of occurences of all words is: %d \n",sum);

	return 0;
}	

/*    EXAMPLE OF RUNNING SCENARIO:
 *    ./output coldplay yellow
 *    ->The sum of occurences of all words is: 4        ( This can be checked by simply using     $ grep "yellow" coldplay    in the command line )
 *
 *    ./ouput coldplay you
 *    ->The sum of occurences of all words is :9
 *
 *    ./output coldplay yellow you
 *    ->The sum of occurences of all words is :13
 *
 *    ./output codplay abracadabra 
 *    ->The sum of occurences of all words is: 0
 *
 *    ./output coldplay abracadabra yellow you
 *    ->The sum of occurences of all words is:13
 */
