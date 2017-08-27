#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 72
//global f and b pointers
int f;
int b;
void whitespace_machine(char buffer[]){
	int i = 0;
	for(i; i < SIZE; i++){
		if(isspace(buffer[i])){
			//keep iterating
			f++;
		}else if (buffer[i] == '\0'){
			return;
		}else{
			printf("%s", buffer);
			// f should equal the index of the character that is not white space
			return;
		}
	}
	//if the machine reaches the point where it has gone the entire buffer and still doesn't find a char, just put f = -1
	f = -1;
	return;
}
int main(){
	FILE *file;
	FILE *outfile = fopen("outfile.txt", "w");
	int line_counter = 1;
	if(outfile == NULL){
		fprintf(stderr, "Can't open outfile\n");
		return -1;
	}
	file = fopen("sourcefile.txt", "r");
	if(file == NULL){
		fprintf(stderr, "Can't open source file\n");
		return -1;
	}
	char buffer[SIZE];

	while(fgets(buffer, sizeof(buffer), file)){
		fprintf(outfile,"%d. %s", line_counter, buffer);
		//Send the buffer into the whitespace_machine
		whitespace_machine(buffer);
		if(f != -1){
			//if f != -1, it found something other than whitespace
			//printf("\nchar that is not space: %d", f);
		}else{
			//Only whitespace found
		}
		line_counter++;
		memset(buffer, ' ', SIZE);
	}
	fclose(file);
	fclose(outfile);
}
