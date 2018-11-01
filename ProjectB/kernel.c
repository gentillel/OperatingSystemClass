
void printString(char*);
void readString(char*);
void readSector(char* buffer,int sector);
void handleInterrupt21(int ax,char* bx,int cx,int dx);
int mod(int a,int b);
int div(int a,int b);


main(){

	char* msg = "hello world ";
	char line[80];
	char buffer[512];
      

 	makeInterrupt21();
    interrupt(0x21,0,"enter line: ",0,0);
    interrupt(0x21,1,line,0,0);
 	interrupt(0x21,2,buffer,30,0);
 	printString(buffer);

	while(1);	
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

    printString("reading sector: ");

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
