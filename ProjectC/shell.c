void commandHandler(char* command);
int strcomp(char* char*);

main(){
	
	while(1){
	//? Do I load it into any sector
	char command[10];
	//print out shell
 	interrupt(0x21,0,"Shell: ",0,0);
 	//read in command
 	interrupt(0x21,1,command,0,0);
 	commandHandler(command);

	}
	return 0;
}

void commandHandler(char* command){
	if (strcomp("type",command,4)){

		interrupt(0x21,0,"type command receive ",0,0);

	}else if(strcomp("execute",command,7)){
		interrupt(0x21,0,"execute command receive ",0,0);
	}
}

int strcomp(char* str, char* rts, int expectedLen){
	int i = 0;
	for(i = 0; i < expectedLen; i++){
		if(str[i] != rts[i]) return 0;
	}
	if(rts[expectedLen] != '\0'){
		interrupt(0x21,0,"too long ",0,0);
		return 0;
	}
	return 1;
}
