void printString(char*); //print to char[] to std io
void readString(char*); //read from key
void readSector(char* buffer,int sector); // read from a sector
void handleInterrupt21(int ax,char* bx,int cx,int dx);
int mod(int a,int b);
int div(int a,int b);
//read from a file into char* buffer max size == 13312
void readFile(char* filename, char* buffer);
//compare two "strings" of max size 6 for the purpose of this project
// The int, sometimes we need to tell the function to start at somewhere
//other than the begining for the comparison for the first string
int strcomp(char*,char*, int);
//will be used to load an execute a program
void executeProgram(char* name, int segment);
void terminate();

int DEBBUG = 0;

main(){

	char* msg = "hello world ";
	char line[80];
	char buffer[512];
      

 	makeInterrupt21();
    // interrupt(0x21,0,"enter line: ",0,0);
    // interrupt(0x21,1,line,0,0);
 	// interrupt(0x21,2,buffer,30,0);
 	// interrupt(0x21,3,"message",buffer,0);
 	interrupt(0x21,4,"tstprg",4000,0);
 	// printString(buffer);
 	
 	// executeProgram("tstprg",4000);
	terminate();	
}

void executeProgram(char* name, int segment){ 
	char programBuffer[13312]; //files cannot exceed 13312
	int i = 0;
	readFile("tstprg",programBuffer); //read target prg into buffer

	//loop through buffer and put in memory
	//* 13312 takes too long for now so for debugging purposes 
	//we go up to 3 segment or 3*512 = 1536
	for(i = 0; i < 1536; i++){
		putInMemory (segment, i,programBuffer[i]);

	}
	if(DEBBUG){printString("Launching Program ");}
	launchProgram(segment);
	
}

int strcomp(char* string1,char* string2,int startString1){
	int i = 0;
	
	for (i = 0; i < 6; i++){
		if(string1[i + startString1] != string2[i]){
			// printString("no same");
			return 0;
		}
		if(string1[i + startString1] == 0 && string2[i] ==0){
			// printString("done");
			return 1;
		}
	}
	// printString("No differences found");
	return 1;

}

void readFile(char* filename, char* file){
	//TODO entry can be more than 50 for loop, add strcomp
	//Load the directory sector into a 512 byte character
 	//array using readSector 
 	char dir[512];
 	//char file[512];
 	int i, entry,matchFound = -1;
 	readSector(dir,2);
 	// printString(dir);

 
 	//currently goes through each entry and tries to find if there exist a match
 	for(entry = 0; entry < 512; entry += 32){
 		if(strcomp(dir,filename,entry)){
 			matchFound = entry + 6;
 			break;
 		}
 	}

	//if the file is found read it and print it 	
 	if(matchFound > -1){

 		while(dir[matchFound] != 0){
 			readSector(file,dir[matchFound]);
 			break;
 		}
 	}

 	// printString(file);
 	
}

void handleInterrupt21(int ax,char* bx,int cx,int dx){
	//if ax == 0; we print the string in bx
	// ax == 1; read from te keyboard save char in bx
	//if ax == 2;read from a sector bx == where characters will go, cx == sector;
	switch(ax){
		case 0:
			printString(bx);
			break;

		case 1:
			readString(bx);
			break;

		case 2:
			readSector(bx,cx);
			break;
		case 3:
			readFile(bx,cx);
			break;
		case 4:
			//@params: bx == name of the program, cx == segment
			executeProgram(bx,cx);
			break;

		case 5:
			terminate();
			break;

		default:
			printString("Error: not suported ax ");
	}
	
}

void readSector(char* buffer,int sector){
	/* (ah = 2, al = number of sector) == ax
	// bx where the data should be stored
	//CH = track number CL = relative sector number
	//track = sector/36; relative sector = */
	int ch = div(sector,36); 
	int cl = mod(sector,18)+1;
	//DH = head number head = ( sector / 18 ) MOD 2
    //DL = device number (for the floppy disk, use 0)
    int dh = mod(div(sector,18),2);

    if(DEBBUG){printString("reading sector: ");}

	interrupt(0x13, 2*256+1, buffer,ch*256+cl,dh*256+0);
}

int div(int a,int b){
	
	int q = 0; 
    while ((q * b)  <=  a){
    	q = q + 1 ;    	
    } 
    return q - 1;
                 
}
           
int mod(int a,int b){
	while (a >= b){
		a = a - b ;
	}
	return a;    
}

void readString(char* line){
	int i = 0;
	char enter = 0xd;
	char backspace = 0x8;
	char a[2];

	while(1){
		a[0] = interrupt(0x16, 0, 0, 0, 0);
		a[1] = '\0';

		if(a[0] == enter){
			line[i] = '\n';
			line[i+1] = '\0';
			printString("\n");
			break;

		}else if(a[0] == backspace){
			//do not erase things you don't have access to 
			if(i< 1){continue;}
			//print the char 'backspace' to move back
			printString(a); 
			//print an empty string to erase what was there before,
			//which moves cursor forward
			printString(" ");
			//print another backspace to move the cursor back
			printString(a); //
			i--;
			continue;
		}

		line[i] = a[0];
		i++;
		printString(a);
	}
}

void printString(char* string){
	// char al = 'H'; 
 	// char ah = 0xe; 
	// int ax = ah * 256 + al;
	int i = 0;
	while(string[i] != '\0'){
		interrupt(0x10, 0xe*256+string[i], 0, 0, 0);
		i++;
	}
}

void terminate(){
	while(1);
}
