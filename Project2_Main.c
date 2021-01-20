#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int isInvalidLen(char argument[],int size); //function to check whether argument next to -l is invalid
int isInvalidArg(char argument[],int size); //function to check whether argument starting with - invalid
int fileArg(char argument[],int size); //checks if an argument doesn't start with - (whether it is a file)
void countChar(FILE *filepointer); //function to count characters and store
void countWord(FILE *filepointer); //function to count words and store
int preprocessChar(char c); //function to preprocess character before counting
char* preprocessWord(char word[], int len); //function to preprocess word before counting
void selectHighest(int length,unsigned long int totalCount); //function to select the highest occurences and store
unsigned long int findMaxW(); //function to find the node with the max count
int maxWordLen(); //finction to find the maximum length from the word lengths stored in highest linked list
unsigned long int totalW(); //function to get total count 
void printGraph1(char ch); //function to print the graph
int checkZero(char argument[],int size); //function to check if an argument is zero

typedef struct ___{ //structure declared to store characters & words and their count 

	char w[70];
	unsigned long int count;
	struct ___ *link;


}words;

void deleteListW(words *head); //to delete all the nodes after using to free memory
void deleteNodeW(words *p); //to delete a specific node in words linked list


typedef struct __{ //structure to store words and characters having highest percentages (only the needed amount to print the graph is stored)
	char c[70];
	float percentage;
	struct __ *link;

}highest;

highest *root2; //to store 1st node address of the linked list -highest
words *root3; //to store 1st node address of the linked list -words

int barLen(highest *p, int len,int rootLen); //function to calculate the bar lengths of the graph


unsigned long int nodeCount; //global variable of count of nodes
int realLength; //if l argument is higher than the number of nodes, number of nodes will be assigned to realLength
int scale; //scale is 1 if --scaled given

int main(int argc,char** argv){
	
	//argument handling

	//error messages
	int i,countArgC=0,countArgW=0,countFile=0,length,checklen=0;
	FILE *filepointer; //file pointer declaration
	char ch;

	
	if(argc==1){ //if no arguments given
		printf("No input files were given\n");
		printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
		return 0;
	}

	else{ //if any argument is given
		for(i=0;i<argc;i++){ //checks all arguments one by one
			if(strcmp(argv[i],"-l")==0){ //if any of the arguments is -l

				if(i==argc-1){ //if -l is the last argument
					printf("Not enough options for [-l]\n");
					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}


				else if((checkZero(argv[i+1],strlen(argv[i+1])))==1){ //if next argument is zero
					return 0;
				} 
				

				else if((isInvalidLen(argv[i+1],strlen(argv[i+1])))==1){ //isInvalidLen function returns 1 if the argument next to -l is invalid
					
					printf("Invalid options for [-l]\n");
					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}

				else if((isInvalidLen(argv[i+1],strlen(argv[i+1])))==2){ //for negative argument after -l
					
					printf("Invalid option(negative) for [-l]\n");
					printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 0;
				}
			}
			else if(isInvalidArg(argv[i],strlen(argv[i]))){ //isInvalidArg function returns 1 if its an invalid argument starting with -
				printf("Invalid option [%s]\n",argv[i]);
				printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 0;
				
			}
		}

		//if none of the above errors were found, checks whther both -c and -w are given
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-c")==0){ //if argument is -c 
				countArgC++;
			}
			if(strcmp(argv[i],"-w")==0){ //if argument is -w 
				countArgW++;
			}
			
		}
		if(countArgC>0&&countArgW>0){ //if both -c and -w were given
			printf("[-c] and [-w] cannot use together\n");
			printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
			return 0;
		}

		//if none of the above errors were found
		//checks whether any filename is given(filenames are considered as arguments that is not a length and doesn't start with -)
		//if a filename is found it checks whether it exists(can be opened)
		for(i=1;i<argc;i++){
			if(isInvalidLen(argv[i],strlen(argv[i]))){ //if argument is not a length
				if(fileArg(argv[i],strlen(argv[i]))){ //if an argument is considered as a filename
					filepointer = fopen(argv[i],"r"); //opens file in read mode
					if(filepointer==NULL){ //if it cannot be opened
						printf("Cannot open one or more given files\n");
						return 0;
					}
					else{
						fclose(filepointer); //file closed
					}
					countFile++;
				}
			}

			else{ //if argument is a length
				if(strcmp(argv[i-1],"-l")!=0){ //if the previous argument is not -l
					filepointer = fopen(argv[i],"r"); //opens file in read mode
					if(filepointer==NULL){ //if it cannot be opened
						printf("Cannot open one or more given files\n");
						return 0;
					}
					else{
						fclose(filepointer); //file closed
					}
					countFile++;
				}

			}
		}
		if(countFile==0){ //if no files found
			printf("No input files were given\n");
			printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv[0]);
			return 0;
		}
	}

	//all the error messages are finished handling (Except for empty files error)
	//if none of the above errors in the arguments were found program proceeds

	countArgC=0; //count of -c assigned to 0 for further use

	for(i=0;i<argc;i++){

		if(strcmp(argv[i],"-c")==0){
			countArgC++; //-c argument count increased
		}

		else if(strcmp(argv[i],"--scaled")==0){ //SCALED OPTION
			scale = 1; 
		}
		
		else if(strcmp(argv[i],"-l")==0){ //if any of the arguments is -l (last -l is taken)
			length = atoi(argv[i+1]); //argument next to -l is assigned as length
			checklen++;
		}		
		
	}
	if(checklen==0){ //since checklen was initially assigned to 0
		length = 10; //default length is 10
	}

	if(length>0){ //if there is a graph to print(if length = 0 that means 0 is given after -l, in that case nothing should be printed)

		for(i=1;i<argc;i++){ //to pass files into the count functions
			//checks if argument is a file
			if(isInvalidLen(argv[i],strlen(argv[i]))){ //if argument is not a length
				if(fileArg(argv[i],strlen(argv[i]))){ //if an argument is considered as a filename

					filepointer = fopen(argv[i],"r"); //opens file in read mode
						
					if(countArgC>0){ //if -c is given
						countChar(filepointer); //function call to count characters
						ch = 'c';
	
					}
					else{ //if -c is not given
						countWord(filepointer); //function call to count words
					
					}
						
					fclose(filepointer); //close file
					
				}
			}
			
		}

		selectHighest(length,totalW()); //creates a linked list to store highest occurences and frees the memory with highest counts

		if(root2==NULL){ //at this point,if root2 =NULL that means the input files were empty and no list was created
			printf("No data to process\n"); //ERROR MESSAGE
			return 0;
		}
		deleteListW(root3); //frees the memory of the word linked list(since this memory is not needed after creating the highest linked list )

		printf("\n");

		if(ch=='c'){
			printGraph1('c'); //print graph (-c mode)
		}
		else{
			printGraph1('w'); //print graph (-w mode)
		}


	}
		
	return 0;
}


int checkZero(char argument[],int size){

	int i,count=0;

	if(size==1){ //if argument has only one character
		if(argument[0]=='0'){
			return 1;
		}
	}

	else{
		for(i=1;argument[i]!='\0';i++){ //checks from 2nd character
			if(argument[i]=='0'){
				count++;
			}
		}

		if(count==size-1){ //all the characters from 2nd character is zero
			if(argument[0]=='0'||argument[0]=='+'||argument[0]=='-'){ //if 1st character is 0 or + or -
				return 1;
			}
		}
	}

	return 0;

}


int isInvalidLen(char argument[],int size){

	if((size==1)&&(argument[0]=='-')){ //if - is the argument
		return 1;
	}

	int i=1; //checks from 2nd character of the argument
	while(i<size){
	
		if(argument[i]<'0'||argument[i]>'9'){ //checks whether argument has characters that are not numbers(0-9)
			return 1;			
		}
		i++;

	}

	//check first character

	if(argument[0]=='+'||(argument[0]>='0'&&argument[0]<='9')){ //if first character is a digit or a + argument is valid

		return 0;

	}

	else{
		if(argument[0]=='-'){ //if first character is - and others are numbers
			return 2;
		}
		else{
			return 1;
		}
	}
}

int isInvalidArg(char argument[],int size){
	
	if(argument[0]=='-'){ //checks only arguments starting with -

		if((strcmp(argument,"-c")!=0)&&(strcmp(argument,"-w")!=0)&&(strcmp(argument,"-l")!=0)&&(strcmp(argument,"--scaled")!=0)){
			return 1; //if argument is not any of the valid ones

		}

	}

	return 0;

}

int fileArg(char argument[],int size){

	if(argument[0]!='-'){ //if argument does not strart with - function returns 1(considers it as a filename)
		return 1;
	}
	return 0;
}

void countChar(FILE *filepointer){

	words *temp1,*temp2;
	char ch;
	char word[2];
	int i;

	while((ch=fgetc(filepointer))!=EOF){ //character assigned to ch variable, loop runs till end of file
		
		i=0;

		if(preprocessChar(ch)){ //only processes alpha numeric characters 
			if((isalpha(ch))&&(isupper(ch))){
				ch = tolower(ch); //capital letters converts to simple
				
			}
			//need to check if ch is already in the linked list
		
			word[0] = ch; //character is stored as a string
			word[1] = '\0';
			temp1 = root3;
			while(temp1!=NULL){ //runs through entire linked list
			
				if(strcmp(word,temp1->w)==0){ //if ch(word) is already in the linked list
					(temp1->count)++; //count increases by 1
					i++;
					break;
				}
				temp1 = temp1->link; //goes to next node
			}
			if(i==0){ //if ch was not found in the linked list
	
				temp2 = (words*)(malloc(sizeof(words))); //memory allocated dynamically 
				strcpy(temp2->w,word); //word is copied to temp2->w
				temp2->count=1; //count is 1
				temp2->link = NULL; 
			
		
				//to make the last node of the linked list connect with the new node
				if(root3==NULL){ //if temp2 is the first node of the linked list
					root3 = temp2;
				}
				else{
					words *p;
					p=root3;
					while(p->link != NULL){
						p = p->link;
					}
					//now p is the last node 
					p->link = temp2; //last node linked with new node
				}
			}
		}
		

	}
}


unsigned long int findMaxW(){

	words *p = root3;
	unsigned long int max = p->count; //1st node's count value assigned as max

	while(p!=NULL){ //runs through entire linked list
		if((p->count)>max){ //if count is higher than max
			max = p->count; //count assigned as max
		}
		p = p->link; //goes to next node
	}
	return max; //highest count returned

}


void selectHighest(int length,unsigned long int totalCount){

	if(length<=nodeCount){ //if length is smaller than the number of nodes
		realLength = length;
	}

	else{ //if length is higher than the number of nodes
		realLength = nodeCount;
	}

	int i;
	unsigned long int max;
	words *p1;
	highest *p;
	
	for(i=0;i<realLength;i++){ //loop runs required times (according to -l argument)

		p = (highest*)(malloc(sizeof(highest))); //memory allocated
		max = findMaxW(); //max count
		p1 = root3;
		while(p1 != NULL){
			if(p1->count == max){ //if p1 node has the max count

				//p1->w field should be assigned to p->c field
				strcpy(p->c,p1->w);
				
				//max percentage should be assigned to p->percentage field
				p->percentage = (max/(double)totalCount)*100;

				//Now the memory of p1 node should be freed and deleted from the linked list
				p->link = NULL;
				deleteNodeW(p1);
				break;
			}
			p1 = p1->link;
		}

		if(root2 == NULL){ //if p is the first node 
			root2 = p;
		}
		else{
			highest *temp;
			temp = root2;
			while(temp->link != NULL){
				temp = temp->link;
			}
			//temp is the last node
			temp->link = p;
		}
	}	
}


int preprocessChar(char c){

	if((isalpha(c))||(c>='0'&&c<='9')){ //if character is a number or an alphabet returns 1
		return 1;
	}
	
	return 0;
}


unsigned long int totalW(){

	unsigned long int totalCount=0;
	words *p;
	p = root3;

	while(p!=NULL){
		totalCount = totalCount + p->count;
		nodeCount++; //nodeCount global variable increased
		p= p->link;
	}

	return totalCount; //total count returned

}


void deleteNodeW(words *p){ //to delete a node in words linked list

	if(p==root3){ //if its the first node
		root3 = p->link;
		p->link = NULL;
		free(p);	
	}
	
	else{

		words *temp = root3; 
		while(temp->link != p){
			temp = temp->link;	
		}
		//now temp is the node before the node p
		temp->link = p->link;
		p->link = NULL;
		free(p);
	}
	
}

void deleteListW(words* head){ //to delete the entire list

	words *temp;
	while(head != NULL){ //checks if head is not NULL
		temp = head;
		head = head->link;
		temp->link = NULL;
		free(temp); //frees memory
	}
}



void printGraph1(char ch){ //to print graph

	highest *temp = root2;
	int len,i,j,barLength,rootLength;

	//to get the number of units to reduce from 80
	//units reduced according to the maximum percentage
	if(root2->percentage == 100){ 
		rootLength = 7; //7 should be reduced 
	}
	else if(root2->percentage >= 10){ 
		rootLength = 6; //6 should be reduced 
	}
	else{
		rootLength = 5; //5 should be reduced 
	}

	
	//the maximum length of the word (if character it's 1) should be found from the elements in the linked list "highest"
	
	if(ch=='c'){
		len = 1; //if characters are considered len is 1
	}
	else{
		len = maxWordLen(); //if words are considered, max word length should be found calling the function
	}


	
	//now the graph should be printed

	for(i=0;i<realLength;i++){

		//to print relavant number of spaces
		for(j=1;j<=len+2;j++){
			printf(" ");
		}
		
		printf("\u2502"); // unicode | printed

		barLength = barLen(temp,len,rootLength);
		for(j=1;j<=barLength;j++){ 
			printf("\u2591"); // unicode
		}
		//moves to next line (line2)

		printf("\n");
		printf(" ");

		printf("%s",temp->c);
		//printf(" ");

		for(j=1;j<=(len+2)-1-strlen(temp->c);j++){
			printf(" ");
		}

		printf("\u2502"); //unicode

		for(j=1;j<=barLength;j++){ 
			printf("\u2591"); // unicode
		}

		printf("%.2f%%",temp->percentage);
		
		//moves to next line (line3)

		printf("\n");

		for(j=1;j<=len+2;j++){
			printf(" ");
		}
		
		printf("\u2502"); // unicode | printed

		for(j=1;j<=barLength;j++){ 
			printf("\u2591"); // unicode
		}

		//next line 4
		printf("\n");
		
		for(j=1;j<=len+2;j++){
			printf(" ");
		}
		
		printf("\u2502"); // unicode | printed
		printf("\n"); //next line (new node)

		
		temp = temp->link;
	}

	//to print relavant number of spaces
	for(j=1;j<=len+2;j++){
		printf(" ");
	}
		
	printf("\u2514"); // unicode printed
	
	for(j=1;j<=(80-(len+3));j++){ //final line of the graph
		printf("\u2500"); //unicode printed
	}

	printf("\n");
	
}

int barLen(highest *p, int len,int rootLen){

	//to find the width that is left for printing the bar
	
	int leftWidth,barLength;
	
	//leftWidth represents the width left for printing the bar
	leftWidth = 80 - (len+3) - rootLen ; //80 is the max print width and required spaces are reduced

	if(scale==1){
		barLength = (p->percentage / root2->percentage) * leftWidth;
		
	}
	else{
		barLength = (p->percentage / 100) * leftWidth;

	}
	return barLength;
}


int maxWordLen(){ //to find the length of the longest word in the list

	highest *p;
	p = root2;
	int maxLen = strlen(p->c); //maxLen assigned as the word length in the root
	while(p!=NULL){
		if(strlen(p->c)>maxLen){ //if length of p->c is higher than max
			maxLen = strlen(p->c); //assigned to max
		}
		p = p->link; //goes to next node
	}
	 return maxLen; //max length returned

}

void countWord(FILE *filepointer){

	words *temp1,*temp2;
	char word[1000]; //word with a length of 999 can be stored maximally
	int i;

	while(fscanf(filepointer,"%s",word)==1){ //goes through entire file until EOF
			
		i=0;
		
		strcpy(word,preprocessWord(word,strlen(word))); //processed word copied to word
		
		if(strlen(word)>0){ //if word is not a blank after preprocessing

		
			//need to check if ch is already in the linked list
			temp1 = root3;
			while(temp1!=NULL){ //runs through entire linked list
				if(strcmp(word,temp1->w)==0){ //if w is found in the linked list
					(temp1->count)++; //count increased
					i++;
					break;
				}
				temp1 = temp1->link;
			}
			if(i==0){ //if w was not found in the linked list
				temp2 = (words*)(malloc(sizeof(words))); //memory allocated dynamically
				strcpy(temp2->w, word); //word copied to temp2->w
				temp2->count=1;
				temp2->link = NULL;
			
		
				//to make the last node of the linked list connect with the new node
				if(root3==NULL){ //if temp2 is the first node
					root3 = temp2;
				}
				else{
					words *p;
					p=root3;
					while(p->link != NULL){
						p = p->link;
					}
					//now p is the last node 
					p->link = temp2; //last node linked with new node
				}
			}
		}

	}
	
}


char* preprocessWord(char word[], int len){ //processes the word

	static char newWord[71]; //maximum processed word length is 70
	int i,j=0;
	for(i=0;i<len;i++){
		if(preprocessChar(word[i])){ //if charcater is a number or a alphabet
			if((isalpha(word[i]))&&(isupper(word[i]))){ //if character is an capital letter
				word[i] = tolower(word[i]); //capital letters converts to simple
				
			}
			newWord[j] = word[i]; //if the character is alpha numeric and simple, it stores into newWord
			j++;
		}
	}
	newWord[j] = '\0';

	return newWord;

}

