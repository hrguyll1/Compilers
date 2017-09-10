#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers.h"
#define SIZE 72

/* Token Types */

#define IDENTIFIER 	1
#define INTEGER 	2
#define REAL		3
#define LONG_REAL	4
#define RELOP		5
#define ADDOP		6
#define MULOP		7
#define	PERIOD		8
#define PAREN_OPEN	9
#define PAREN_CLOSE	10
#define	COLON		11
#define BRACKET_OPEN	12
#define BRACKET_CLOSE	13
#define ASSIGNOP	14
#define DOUBLE_PERIOD	15
#define LEXERROR	99
#define RW_SIZE		23

int f = 0;
int p = 0;
int line = 1;
FILE *file = NULL;
FILE *outfile = NULL;
char reserved_words[RW_SIZE][SIZE];

/*
	This clears the buffer.
*/
int get_reserved_words(){
	/*
		Get reserved words and put them into an array.
	*/

	FILE *rw_file;
	char l[SIZE];

	rw_file = fopen("reservedwords.txt", "r");
	if(rw_file == NULL){
		printf("Could not read reserved words file.\n");
	}
	int i = 0;
	while(1){
		if(fgets(reserved_words[i], SIZE, rw_file) == NULL){
			break;
		}else if(i >= RW_SIZE){
			break;
		}
		i++;

	}
	fclose(rw_file);
}
int clear(char* buffer){
	int j = 0;
	for(j = 0; j < 72; j++){
		buffer[j] = '\0';
	}
}
int shift(char* buffer, int j, char* token_type, int attribute){
	int k = 0;
	char id[j + 1];
	char new_buffer[SIZE];
	for(k = 0; k < j; k++){
		//save identifier into a temporary buffer called id
		id[k] = buffer[k];
	}
	//null terminate the new buffer
	id[k] = '\0';
	//Shift the buffer to the left and remove the token that was just recognized
	//COULD PRESENT AN OFF-BY-ONE ERROR. TEST THIS.
	int z = 0;
	for(k = j; k < 72; k++){
		new_buffer[z] = buffer[k];
		z++;
	}
	for(z = 0; z < 72; z++){
		buffer[z] = new_buffer[z];
	}

	printf("(line %d, id: %s, %s, %d)\n", line, id, token_type, attribute);
	return 1;
}
/*
	This should identify all acceptable numbers.
*/
int long_real(char* buffer){
	if(isdigit(buffer[0])){
		/*
			If it starts with a 0 it is an error condition.
		*/
		if(buffer[0] - '0' == 0){
			int j = 1;
			//printf("Error in integer format. Cannot start with 0.\n");
			//need to get the entire integer here.
			while(isdigit(buffer[j])){
				j++;
			}
			shift(buffer, j, "LEXERROR", LEXERROR);
			whitespace(buffer);
		}else{
			int j = 1;
			while(isdigit(buffer[j])){
				j++;
			}
			if(buffer[j] == '.'){
				//must include digit
				j++;
				if(!isdigit(buffer[j])){
					/*
						Need to remove the broken number

					*/
					shift(buffer, j, "LEXERROR", LEXERROR);
					whitespace(buffer);
				}else{
					while(isdigit(buffer[j])){
						//keep incrementing
						j++;
					}
					//found long
					if(buffer[j] == 'E'){
						j++;
						if(buffer[j] == '+' || buffer[j] == '-'){
							j++;
							while(isdigit(buffer[j])){
								j++;
							}
							int k = 0;
							//printf("Found Long Real: ");
							//Shift buffer and print out the long real
							shift(buffer, j, "LONG_REAL", LONG_REAL);
							whitespace(buffer);

						}else if(isdigit(buffer[j])){
							//Assume it is a plus:
							j++;
							while(isdigit(buffer[j])){
								j++;
							}
							int k = 0;
							//printf("Found Long Real: ");
							shift(buffer, j, "LONG_REAL", LONG_REAL);
							whitespace(buffer);

						}else{
							//printf("Error in formatting of long real. ");
							shift(buffer, j + 1, "LEXERROR", LEXERROR);
							whitespace(buffer);
						}
					}else{
						//found a floating point real
						shift(buffer, j, "REAL", REAL);
						whitespace(buffer);
					}
				}
			}else if(buffer[j] == 'E'){
				j++;
				if(buffer[j] == '+' || buffer[j] == '-'){
					j++;
					while(isdigit(buffer[j])){
						j++;
					}
					int k = 0;
					printf("Found Long Real: ");
					shift(buffer, j, "LONG_REAL", LONG_REAL);
					whitespace(buffer);

				}else if(isdigit(buffer[j])){
					//Assume it is a plus:
					j++;
					while(isdigit(buffer[j])){
						j++;
					}
					printf("Found Long Real: ");
					shift(buffer, j, "LONG_REAL", LONG_REAL);
					whitespace(buffer);

				}
			}else{
				shift(buffer, j, "REAL", REAL);
				whitespace(buffer);
			}
		}
	}else{
		//The thing is not a number so call the next machine:
		//Addops
	}
	return 1;
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
	if( isalpha(buffer[0]) ){
		j = 1;
		while( (isalpha(buffer[j]) || isdigit(buffer[j]))){
			j++;
		}
		if(j < 11){
			shift(buffer, j, "ID", IDENTIFIER);
			whitespace(buffer);

		}else{
			//identifier too long
			//Still remove the identifier that is too long
			shift(buffer, j, "LEXERROR", LEXERROR);
			whitespace(buffer);
		}

	}else{
		//else the thing was not an identifier, so send it to the number checker
		long_real(buffer);

	}
}
int relop(char* buffer){
	int j = 0;
	if(buffer[j] == '>' && buffer[j + 1] == '='){
		//Found (relop, GE)
		printf("Found (relop GE)\n");
		//Now remove it
	}else if(buffer[j] == '>'){

		printf("Found (relop, GT)\n");
	}else if(buffer[j] == '='){

		printf("Found (relop, EQ)\n");
	}else if(buffer[j] == '<' && buffer[j + 1] == '='){

		printf("Found (relop, LE)\n");
	}else if(buffer[j] == '<' && buffer[j + 1] == '>'){

		printf("Found (relop, NE)\n");
	}else if(buffer[j] == '<'){

		printf("Found (relop, LT)\n");
	}else{
		printf("Not a relop\n");
	}


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
	if(i == 0 && ch == '\n'){
		//the whole line is a newline so line should increment
		line++;
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
			line++;
			int status = readline(buffer);
			if(status == -1){
				printf("End of file.\n");
				break;
			}
			i = 0;
		}else if(buffer[i] == '\t' || buffer[i] == ' '){
			//found something other than a space
			//update index f
			i = i + 1;
		}else{
			f = i;

			/*
				Finally found a character at location i.
				Remove white space.
			*/

			char new_buffer[SIZE];
			int z = 0;
			int k = 0;
			for(k = i; k < 72; k++){
				new_buffer[z] = buffer[k];
				z++;
			}
			buffer = new_buffer;
			identifier(buffer);
			return 1;

		}
	}

	return 0;
}

int main(){
	file = fopen("sourcefile.txt", "r");
	outfile = fopen("outfile.txt", "w");
	get_reserved_words();
	char buffer[SIZE];
	int i = 0;
	int j = 0;
	int status = 0;
	while(readline(buffer) != -1){
		whitespace(buffer);
		clear(buffer);

	}
}
