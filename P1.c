#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 72
int f = 0;
int p = 0;
int line = 1;
FILE *file = NULL;
FILE *outfile = NULL;
int clear(char* buffer){
	int j = 0;
	for(j = 0; j < 72; j++){
		buffer[j] = '\0';
	}
}

/*
	Real Second!
*/
/*
	Identifer:
	Up to 10 character
	Starts character
	Can have numbers after
*/
int identifier(char* buffer){
	int j = 0;
	if(isalpha(buffer[0])){
		//check if each of the others are alpha or digit
		j = 1;
		while(1){
			//got an identifier token
			if(buffer[j] == ' '){
				int k = 0;
				for(k = 0; k < j; k++){
					printf("%c", buffer[k]);
				}
				printf("\n");
				/*
					Got a token. Update the buffer to have something other than the token
				*/
				return 1;
			}
			if(isalpha(buffer[j]) || isdigit(buffer[j])){
				if( j >= 10){
					printf("identifier too long\n");
				}
				j = j + 1;
			}else{
				printf("not correct ID format with buffer = %s\n", buffer);
				return -1;
			}
		}
	}else{
		//not an identifier
		printf("not correct ID format\n");
		return -1;
	}
}
int real(char* buffer){



	return 0;
}
/*
	Long Real first
*/

int relop(char* buffer){




	return 0;
}
/*
	If reach /0 then read next buffer
*/
int readline(char* buffer){
	int i = 0;
	char ch;
	int newline = 0;
	int j = 0;
	while((ch = fgetc(file)) != '\n'){
		if(ch == EOF){
			return -1;
		}
		//While it doesn't equal newline, put it into buffer
		buffer[i] = ch;
		//do something with buffer
		i++;
	}
	buffer[i] = '\n';
	return 0;
}
int whitespace(char* buffer){
	int i = 0;
	while(1){
		//Look for something other than whitespace
		if(buffer[i] == '\n'){
			clear(buffer);
			int status = readline(buffer);
			if(status == -1){
				printf("End of file.\n");
				break;
			}
			i = 0;
		}else if(buffer[i] == '\t' || buffer[i] == ' '){
			//found something other than a space
			//update index f
			/*
				Trim the buffer by shifting it
			*/
			i = i + 1;
		}else{
			f = i;
			printf("char[%i] = %c\n", i, buffer[f]);
			return 1;

		}
	}

	return 0;
}

int main(){
	file = fopen("sourcefile.txt", "r");
	outfile = fopen("outfile.txt", "w");

	char buffer[SIZE];
	int i = 0;
	int j = 0;
	int status = 0;
	status = readline(buffer);
	if(status == -1){
		printf("End of file.\n");
	}else{
		whitespace(buffer);
		identifier(buffer);
	}
//	while(status != -1){
//		//printf("%i.    %s\n", line, buffer);
//		whitespace(buffer);
//		clear(buffer);
//		line = line + 1;
//		i++;
//		status = readline(buffer);

//	}
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
