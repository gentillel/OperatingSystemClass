
#include<stdio.h>
#include<stdlib.h>

int main (int argc, char*argsv[]){

	if (argc < 3){
	   printf("too little argument you will suffer seg fault\n");
	   return -1;
	}else if(argc > 3){
	    printf("too many arguments given\n");
	    return -1;
	}

	int a = atoi(argsv[0]); 
	int b = atoi(argsv[1]);
	// printf("%d\n",a+b);
	return a+b;
}
