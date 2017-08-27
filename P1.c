#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 72
int f = 0;
int p = 0;
int line = 0;

int relop(char* buffer){

	return 0;
}
int whitespace(char* buffer){
	int i = 0;
	while(i < SIZE){
		//Look for something other than whitespace
		if(buffer[i] != '\r' && buffer[i] != '\n' && buffer[i] != '\t' && buffer[i] != ' '){
			//found something other than a space
			//update index f
			f = i;
			printf("%c", buffer[f]);
			return 1;

		}
		i = i + 1;
	}

	return 0;
}

int readline(char* buffer){
	FILE *file = fopen("sourcefile.txt", "r");
	FILE *outfile = fopen("outfile.txt", "w");
	int i = 0;
	char ch;
	int newline = 0;
	while((ch = fgetc(file)) != EOF){
		//While it doesn't equal newline, put it into buffer
		buffer[i] = ch;
		if(ch == '\n'){
			line = line + 1;
			//do something with buffer
			printf("%d.    %s", line, buffer);
			whitespace(buffer);
			//clear buffer
			int j = 0;
			for(j = 0; j < SIZE; j++){
				buffer[j] = '\0';
			}
			i = -1;
		}
		i++;
	}
	return 0;
}

int main(){
	char buffer[SIZE];
	readline(buffer);
/*
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
		line_counter++;
		memset(buffer, '\0', SIZE);
	}
	fclose(file);
	fclose(outfile);
*/
}
