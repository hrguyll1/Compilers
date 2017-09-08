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
#define ERROR		99

int f = 0;
int p = 0;
int line = 1;
FILE *file = NULL;
FILE *outfile = NULL;

/*
	This clears the buffer.
*/
int get_reserved_words(){
	/*
		Get reserved words and put them into an array.
	*/

}
int clear(char* buffer){
	int j = 0;
	for(j = 0; j < 72; j++){
		buffer[j] = '\0';
	}
}
/*
	This should identify all legal numbers.
*/
int long_real(char* buffer){
	if(isdigit(buffer[0])){
		/*
			If it starts with a 0 it is an error condition.
		*/
		if(buffer[0] - '0' == 0){
			printf("Error in integer format. Cannot start with 0.\n");
		}else{
			int j = 1;
			while(isdigit(buffer[j])){
				j++;
			}
			if(buffer[j] == '.'){
				//must include digit
				j++;
				if(!isdigit(buffer[j])){
					printf("Error in format of float.\n");
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
							//TO DO: Retract
							//Save value of int
							int k = 0;
							printf("Found Long Real: ");
							for(k = 0; k < j; k++){
								printf("%c", buffer[k]);
							}
							printf("\n");
						}else if(isdigit(buffer[j])){
							//Assume it is a plus:
							j++;
							while(isdigit(buffer[j])){
								j++;
							}
							//TO DO: Retract
							//Save value of int
							int k = 0;
							printf("Found Long Real: ");
							for(k = 0; k < j; k++){
								printf("%c", buffer[k]);
							}
							printf("\n");

						}else{
							printf("Error in formatting of long real.\n");
						}
					}else{
						//found a floating point number:
						printf("Real number = ");
						int k = 0;
						for(k = 0; k < j; k++){
							printf("%c",buffer[k]);
						}
						printf("\n");

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
					for(k = 0; k < j; k++){
						printf("%c", buffer[k]);
					}
					printf("\n");
				}else if(isdigit(buffer[j])){
					//Assume it is a plus:
					j++;
					while(isdigit(buffer[j])){
						j++;
					}
					//TO DO: Retract
					//Save value of int
					int k = 0;
					printf("Found Long Real: ");
					for(k = 0; k < j; k++){
						printf("%c", buffer[k]);
					}
					printf("\n");
				}
			//This should go in first if statement because it is never reached right now.
			}else{
				printf("Found Real\n");
				int k = 0;
				for(k = 0; k < j; k++){
					printf("%c", buffer[k]);
				}
				printf("\n");
			}
				//TO DO: Retract one character and remove the number from the buffer
		}
	}
			//Need other state
			//Need state for if it's digitE+/-
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
			//found an identifier
			int k = 0;
			char id[j];
			char new_buffer[SIZE];

			for(k = 0; k < j; k++){
				//save identifier into a temporary buffer called id
				id[k] = buffer[k];
			}
			printf("\n%s\n", id);

			//Shift the buffer to the left and remove the token that was just recognized
			//COULD PRESENT AN OFF-BY-ONE ERROR. TEST THIS.
			int z = 0;
			for(k = j; k < 72; k++){
				new_buffer[z] = buffer[k];
				z++;
			}
			buffer = new_buffer;
			printf("\n%s\n", buffer);

			//What if we are at the end of a line?

		}else{
			//identifier too long
			printf("Identifer too long");

			//Still remove the identifier that is too long
			char new_buffer[SIZE];
			int z = 0;
			int k = 0;
			for(k = j; k < 72; k++){
				new_buffer[z] = buffer[k];
				z++;
			}
			buffer = new_buffer;
			printf("\n%s\n", buffer);

		}
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
			printf("\n%s\n", buffer);

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
		//identifier(buffer);
		//long_real(buffer);
		//relop(buffer);
	}
}
