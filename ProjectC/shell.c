void commandHandler(char* command);
void cmdParamSplit(char* toSplit, char* cmd, char* param);
int strcomp(char* char*);

main(){
	
	while(1){
		//? Do I load it into any sector
		char command[20];
		//print out shell
 		interrupt(0x21,0,"Shell:> ",0,0);
 		//read in command
 		interrupt(0x21,1,command,0,0);
 		commandHandler(command);

	}
	return 0;
}

void commandHandler(char* command){
	char actualCommand[10];
	char parameter[10];
	cmdParamSplit(command,actualCommand,parameter);
	if (strcomp("type",command,4)){

		interrupt(0x21,0,"type command receive ",0,0);

	}else if(strcomp("execute",command,7)){
		interrupt(0x21,0,"execute command receive ",0,0);
	}
}

void cmdParamSplit(char* toSplit, char* cmd, char* param){
	int i = 0;
	int spaceFound = 0;
	interrupt(0x21,0," in cmdParamSplit",0,0);
	while(toSplit[i] != '\0' || i < 20){

		// if(toSplit[i] = ' '){
		// 	spaceFound++;
		// }else if(spaceFound){
		// 	param[i] = toSplit[i];
		// }else{
		// 	cmd[i] = toSplit[i];
		// }
	
		i++;
	}
	interrupt(0x21,0,"found a zero ",0,0);
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
