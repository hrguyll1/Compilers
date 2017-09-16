#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers.h"
#define SIZE 72

/* Token Types */

#define IDENTIFIER 	1
#define INT	 	2
#define REAL		3
#define LONG_REAL	4
#define RELOP		5
#define ADDOP		6
#define MULOP		7
#define	DOT		8
#define OPEN_PAREN	9
#define CLOSE_PAREN	10
#define	COLON		11
#define OPEN_BRACKET	12
#define CLOSE_BRACKET	13
#define ASSIGNOP	14
#define DOUBLE_PERIOD	15
#define LEXERROR	99
#define RW_SIZE		22
#define OPEN_S_BRACKET	24
#define CLOSE_S_BRACKET 25
#define COMMENT_OPEN	26
#define COMMENT_CLOSE	27
#define COMMA		28
#define	SEMICOLON	29
#define STAR		30
#define SLASH		31
#define DIV		32
#define PLUS		36
#define MINUS		37
#define DOUBLE_DOT	38

int f = 0;
int p = 0;
int line = 1;

FILE *file = NULL;
FILE *outfile = NULL;

typedef struct token{

        char* lexeme;
        char* token;
        int attribute;

} token_t;

struct token reserved_words[20];

int get_reserved_words(){
        char temp_buffer[72];
        char ch;
        /*
                Get reserved words and put them into an array.
        */

        FILE *rw_file;

        rw_file = fopen("reservedwords.txt", "r");
        if(rw_file == NULL){
                printf("Could not read reserved words file.\n");
                return -1;
        }
        int i = 0;
        while(i < 20){

                int j = 0;
                int k = 0;
                //grab everything until the line is over
                while((ch = fgetc(rw_file)) != '\n'){
                        temp_buffer[j] = ch;
                        j++;
                }
                temp_buffer[j] = '\0';
                //parse everything in the line

                //get exact size of lexeme
                j = 0;
                while(temp_buffer[j] != ' '){
                        j++;
                }
                reserved_words[i].lexeme = (char*)malloc(j * sizeof(char));
                j = 0;
                while(temp_buffer[j] != ' '){
                        reserved_words[i].lexeme[j] = temp_buffer[j];
                        j++;
                }
                reserved_words[i].lexeme[j] = '\0';

                //get exact size of token
                j = j + 1;
                k = 0;
                int temp = j;
                while(temp_buffer[j] != ' '){
                        k++;
                        j++;
                }
                reserved_words[i].token = (char*)malloc(k * sizeof(char));
                k = 0;
                j = temp;
                while(temp_buffer[j] != ' '){
                        reserved_words[i].token[k] = temp_buffer[j];
                        j++;
                        k++;
                }
                reserved_words[i].token[j] = '\0';

                //get exact size of attribute
                k = 0;
                j = j + 1;
                temp = j;
                while(temp_buffer[j] != '\0'){
                        k++;
                        j++;
                }
                char* attribute = (char*)malloc(k * sizeof(char));
                k = 0;
                j = temp;
                while(temp_buffer[j] != '\0'){
                        attribute[k] = temp_buffer[j];
                        j++;
                        k++;
                }
                attribute[j] = '\0';
                reserved_words[i].attribute = atoi(attribute);


            i++;
        }

        fclose(rw_file);
}

int get_next_token(){

	return 0;
}
int catch_all(char* buffer){
	int j = 0;
	if(buffer[j] == ';'){
		shift(buffer, j + 1, "SEMICOLON", SEMICOLON);
		whitespace(buffer);
	}else if(buffer[j] == '+'){
		shift(buffer, j + 1, "ADDOP", PLUS);
		whitespace(buffer);
	}else if(buffer[j] == '-'){
		shift(buffer, j + 1, "ADDOP", MINUS);
		whitespace(buffer);
	}else if(buffer[j] == ':' && buffer[j + 1] == '='){
		shift(buffer, j + 2, "ASSIGNOP", ASSIGNOP);
		whitespace(buffer);
	}else if(buffer[j] == ','){
		shift(buffer, j + 1, "COMMA", COMMA);
		whitespace(buffer);
	}else if(buffer[j] == '.'){
		shift(buffer, j + 1, "DOT", DOT);
		whitespace(buffer);
	}else if(buffer[j] == ':'){
		shift(buffer, j + 1, "COLON", COLON);
		whitespace(buffer);
	}else if(buffer[j] == '['){
		shift(buffer, j + 1, "OPEN_BRACKET", OPEN_BRACKET);
		whitespace(buffer);
	}else if(buffer[j] == ']'){
		shift(buffer, j + 1, "CLOSE_BRACKET", CLOSE_BRACKET);
		whitespace(buffer);
	}else if(buffer[j] == '{'){
		shift(buffer, j + 1, "OPEN_S_BRACKET", OPEN_S_BRACKET);
		whitespace(buffer);
	}else if(buffer[j] == '}'){
		shift(buffer, j + 1, "CLOSE_S_BRACKET", CLOSE_S_BRACKET);
		whitespace(buffer);
	}else if(buffer[j] == '(' && buffer[j + 1] == '*'){
		shift(buffer, j + 1, "COMMENT_OPEN", COMMENT_OPEN);
		whitespace(buffer);
	}else if(buffer[j] == '*' && buffer[j + 1] == ')'){
		shift(buffer, j + 2, "COMMENT_CLOSE", COMMENT_CLOSE);
		whitespace(buffer);
	}else if(buffer[j] == '*'){
		shift(buffer, j + 1, "MULOP", STAR);
		whitespace(buffer);
	}else if(buffer[j] == '/'){

		shift(buffer, j + 1, "MULOP", SLASH );
		whitespace(buffer);
	}else if(buffer[j] == '('){

		shift(buffer, j + 1, "OPEN_PAREN", OPEN_PAREN);
		whitespace(buffer);
	}else if(buffer[j] == ')'){

		shift(buffer, j + 1, "CLOSE_PAREN", CLOSE_PAREN);
		whitespace(buffer);
	}else{
		//Symbol unrecognized LEXERROR
		shift(buffer, j + 1, "LEXERROR UNRECOGNIZED SYMBOL.", LEXERROR);
		whitespace(buffer);
	}
}
int clear(char* buffer){
	int j = 0;
	for(j = 0; j < 72; j++){
		buffer[j] = '\0';
	}
}
//struct token shfit(char* buffer, int j, char* token_type, int attribute){
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
	if(attribute == IDENTIFIER){
		//check to see if it is a reserved word
		k = 0;
		while(k < RW_SIZE){
			if(strstr(reserved_words[k].lexeme, id)){
				printf("(%i, %s, %s, %i)\n", line, reserved_words[k].lexeme, reserved_words[k].token, reserved_words[k].attribute);
				return 1;
			}
			k++;
		}
	}

	struct token new_token;
	new_token.lexeme = (char*)malloc((j+1) * sizeof(char));
	new_token.lexeme = id;
	new_token.token = (char*)malloc(sizeof(token_type) * sizeof(char));
	new_token.token = token_type;
	//new_token.attribute = (int)malloc(sizeof(attribute) * sizeof());
	new_token.attribute = attribute;
	printf("(%d, %s, %s, %d)\n", line, new_token.lexeme, new_token.token, new_token.attribute);
//	printf("(%d, |%s|, %s, %d)\n", line, id, token_type, attribute);

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
		if(buffer[0] - '0' == 0 && !isdigit(buffer[1])){
			shift(buffer, 1, "INT", INT);
			whitespace(buffer);
		}else if(buffer[0] - '0' == 0){
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
					shift(buffer, j + 1, "LEXERROR", LEXERROR);
					//printf("New Buffer = %s\n", buffer);
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
						if(buffer[j - 1] == '0'){
							printf("LEXERROR, ENDING ZEROS\n");
							shift(buffer, j, "LEXERROR", LEXERROR);
						}else{
							//found a floating point real
							shift(buffer, j, "REAL", REAL);
							whitespace(buffer);
						}
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
				shift(buffer, j, "INT", INT);
				whitespace(buffer);
			}
		}
	}else{
		//The thing is not a number so call the next machine:
		relop(buffer);
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
		//Now remove it
		shift(buffer, j + 2, "RELOP", RELOP);
		whitespace(buffer);
	}else if(buffer[j] == '>'){
		//printf("Found (relop, GT)\n");
		shift(buffer, j + 1, "RELOP", RELOP);
		whitespace(buffer);
	}else if(buffer[j] == '='){
		//printf("Found (relop, EQ)\n");
		shift(buffer, j + 1, "RELOP", RELOP);
		whitespace(buffer);
	}else if(buffer[j] == '<' && buffer[j + 1] == '='){
		//printf("Found (relop, LE)\n");
		shift(buffer, j + 2, "RELOP", RELOP);
		whitespace(buffer);

	}else if(buffer[j] == '<' && buffer[j + 1] == '>'){
		//printf("Found (relop, NE)\n");
		shift(buffer, j + 2, "RELOP", RELOP);
		whitespace(buffer);

	}else if(buffer[j] == '<'){
		//printf("Found (relop, LT)\n");
		shift(buffer, j + 1, "RELOP", RELOP);
		whitespace(buffer);

	}else{
		//printf("Not a relop\n");
		//Go to next machine
		catch_all(buffer);
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
		//line++;
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
