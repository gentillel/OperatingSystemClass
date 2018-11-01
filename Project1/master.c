#include<stdio.h>
#include <stdlib.h> //malloc()
#include <unistd.h>//fork() execvp() getppid()
#include <sys/types.h> //getppid()
#include <sys/wait.h>


int main (int argc, char*argsv[]){

	int i;//for the for lop 
	//used by sprintf to save the sums back as strings
	char* backToString = malloc(sizeof(char) * 4);
	//used to store the return var of waitpid
	int status;
	//the arguments that are going to be passed to slave
	char* arg[2];
	//index used to keep track of where 
	//the most updated sums are in argsv
	int index = 3;


	while (index > 2){
		index = 1;
		
		for (i = 1; i < argc ; i = i + 2){

			arg[0]  = argsv[i];
			arg[1]  = argsv[i+1];
			//DEBUG STATEMENT
			// printf("arg[0] + arg[1] == %s + %s\n",arg[0],arg[1]);
			pid_t pid = fork();
			
			if (pid == 0) { 
				//then call execute and turn it into a new process
				execvp("./slave",arg);
			}else{
				int w = waitpid(pid,&status,0);

			     if (w == -1) {
			    	printf("DEBUG:waitpid faillure%d\n",i);
			    	break;
		        }

			    int exit = WEXITSTATUS(status);
			    //DEBUG STATEMENT
			    //printf("child return statement:%d\n",exit);
			    sprintf(backToString,"%d",exit);
			    //DEBUG STATEMENT
			    //printf("backToString:%s\n",backToString);

			    (argsv[index]) = backToString;
			    backToString = malloc(sizeof(char) * 5);

			    //DEBUG STATEMENT
			    //printf("saved:%s\n",argsv[index]);
			    index++;

			    // determining when it will exit the for loop
			    if (3 + i >= argc){

			    	//determining if the list of numbers is odd
					if (argc % 2 == 0){
						//printf("uneven %s\n",argsv[argc-1]);
						argsv[index] = argsv[argc-1];
						argsv[index + 1] = NULL;
						argc = index + 1;
						index = 3;
					}else{

						//printf("will exit index:%d\n",index);
			    		argc = index;
			    		argsv[index] = NULL;
					}
			    }
			}
		}
		//DEBUG STATEMENT
		for(i = 0; i < argc; i++){
			printf("%s ", argsv[i]);
		}
		printf("\n");
	}
	printf("Final answer:%s\n",argsv[1]);
	return 0;
}
