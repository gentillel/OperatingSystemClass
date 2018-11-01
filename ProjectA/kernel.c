main(){
	char* msg = "hello world";

	int i = 0;
	int next = 0;

	while(msg[i] != '\0'){
		
		putInMemory(0xB800,0x140 + next,msg[i]);
		putInMemory(0xB800,0x141 + next,0x7);
		i++;
		next += 2;
	}

	while(1);
	
}
