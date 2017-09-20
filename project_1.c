#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers.h"
#include <time.h>
#define SIZE 72
#define getName(var) #var

int f = 0;
int p = 0;
int line = 1;
int list_size = 0;

FILE *file = NULL;
FILE *outfile = NULL;

typedef struct identifier{
        char* lexeme;
	int value;
	char* ptr;
	struct identifier *next;

} identifier_t;
typedef struct token{

        char* lexeme;
	int token_type;
        char* token;

	int float_flag;
	int int_flag;
	int char_flag;
	int ptr_flag;

        char* c_attribute;
	int i_attribute;
	float f_attribute;
	char* p_attribute;

	char* attribute_c;
//	char* c_attribute;

} token_t;

struct token reserved_words[20];
struct identifier ll_start;
void print_list(){
	struct identifier *current = &ll_start;
	int i = 0;
	printf("\n");
	while(current != NULL){
		printf("Lexeme = |%s|, ptr = %i\n", current->lexeme, current->value);
		current = current->next;
		i++;
	}


}
int find_identifier(char* id, int* out){

	struct identifier *current = &ll_start;
	while(current != NULL){
		if(strcmp(id, current->lexeme) == 0){
			return current->value;
		}
		current = current->next;
	}

	return 0;
}

int add_item(struct identifier *head, char* id){
	struct identifier *ptr = (struct identifier*) malloc(sizeof(struct identifier));
	struct identifier *current = &ll_start;

	while(current->next != NULL){
		current = current->next;
	}
	ptr->lexeme = (char*)malloc(sizeof(char) * sizeof(id));
	strcpy(ptr->lexeme, id);

	//give value a random address
	time_t t;
	srand(time(&t));
	ptr->value = rand() / line;

	ptr->next = NULL;
	current->next = ptr;

	return ptr->value;
}

void print_token(struct token t){
	int width = 15;
	//if(t.token_type == 
        if(t.float_flag == 1){
                printf("%*i %*s        (%i) %*s           %*f   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.f_attribute, t.attribute_c);
        }else if(t.int_flag == 1){
                printf("%*i %*s        (%i) %*s           %*i   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.i_attribute, t.attribute_c);
	}else if(t.ptr_flag == 1){
                printf("%*i %*s        (%i) %*s           %*i   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.i_attribute, t.attribute_c);
        }else{
                printf("%*i %*s        (%i) %*s           %*s   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.c_attribute, t.attribute_c);
	}
}
int get_reserved_words(){
        char temp_buffer[72];
        char ch;
	char* attribute;
	char* token_t;
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
		// Format: lexeme token attribute
                //grab everything until the line is over
                while((ch = fgetc(rw_file)) != '\n'){
                        temp_buffer[j] = ch;
                        j++;
                }
                temp_buffer[j] = '\0';
		j = 0;
		k = 0;
		//get length of the lexeme
		while(temp_buffer[j] != ' '){
			j++;
			k++;
		}
		//allocate space for the lexeme
                reserved_words[i].lexeme = (char*)malloc(j * sizeof(char));

		//allocate space for the token string

                reserved_words[i].token = (char*)malloc(j * sizeof(char));
		//copy over the lexeme
		k = 0;
		j = 0;
		while(temp_buffer[j] != ' '){
			//printf("%c ", temp_buffer[j]);
			reserved_words[i].lexeme[k] = temp_buffer[j];
			reserved_words[i].token[k] = temp_buffer[j];
			j++;
			k++;
		}
		reserved_words[i].lexeme[k] = '\0';
		reserved_words[i].token[k]= '\0';
//		printf("lexeme = %s\n", reserved_words[i].lexeme);

		k = 0;
		j = j + 1;
		int temp = j;
		//now get the token size
		while(temp_buffer[j] != ' '){
			j++;
			k++;
		}

                token_t = (char*)malloc(k * sizeof(char));
		j = temp;
		k = 0;
		while(temp_buffer[j] != ' '){
			token_t[k] = temp_buffer[j];
			j++;
			k++;
		}
		token_t[k] = '\0';
		reserved_words[i].token_type = atoi(token_t);
//		printf("token = %i\n", reserved_words[i].token_type);

		temp = j;
		k = 0;
		//get size of attribute
		while(temp_buffer[j] != '\0'){
			j++;
			k++;
		}
		attribute = (char*)malloc(k * sizeof(char));
		j = temp;
		k = 0;
		while(temp_buffer[j] != '\0'){
			attribute[k] = temp_buffer[j];
			j++;
			k++;
		}
		attribute[k] = '\0';
		reserved_words[i].i_attribute = atoi(attribute);
		j = temp; 
               while(temp_buffer[j] != ' '){
                        k++;
                        j++;
                }
                char* token_t = (char*)malloc(k * sizeof(char));
                k = 0;
                j = temp;
                while(temp_buffer[j] != '\0'){
                        token_t[k] = temp_buffer[j];
                        j++;
                        k++;
                }
                token_t[j] = '\0';
                reserved_words[i].token_type = atoi(token_t);
		free(token_t);


                //get exact size of attribute
                k = 0;
//                j = j + 1;
                temp = j;
                while(temp_buffer[j] != '\0'){
                        k++;
                        j++;
                }
                char* attribute= (char*)malloc(k * sizeof(char));
                k = 0;
                j = temp;
                while(temp_buffer[j] != '\0'){
                        attribute[k] = temp_buffer[j];
                        j++;
                        k++;
                }
                attribute[j] = '\0';
//		printf("attribute = %s\n", attribute);


		reserved_words[i].int_flag = 1;
                //reserved_words[i].i_attribute = atoi(attribute);
		//free(attribute);
		//free(token_t);
		if(reserved_words[i].token_type == 7){
			//MULOP
			reserved_words[i].token = (char*)malloc((sizeof("MULOP") + 1) * sizeof(char));
			reserved_words[i].token = "MULOP";
		}else if(reserved_words[i].token_type == 6){
			//ADDOP
			reserved_words[i].token = (char*)malloc((sizeof("ADDOP")+ 1) * sizeof(char));
			reserved_words[i].token = "ADDOP";

		}
		if(reserved_words[i].token_type == 7 && reserved_words[i].i_attribute == 39){

			reserved_words[i].attribute_c = (char*)malloc(sizeof("and") * sizeof(char));
			reserved_words[i].attribute_c = "and";

		}
		else if(reserved_words[i].token_type == 7 && reserved_words[i].i_attribute == 42){

			reserved_words[i].attribute_c = (char*)malloc(sizeof("div") * sizeof(char));
			reserved_words[i].attribute_c = "div";

		}
		else if(reserved_words[i].token_type == 7 && reserved_words[i].i_attribute == 47){

			reserved_words[i].attribute_c = (char*)malloc(sizeof("mod") * sizeof(char));
			reserved_words[i].attribute_c = "mod";

		}
		else if(reserved_words[i].token_type == 6 && reserved_words[i].i_attribute == 51){

			reserved_words[i].attribute_c = (char*)malloc(sizeof("or") * sizeof(char));
			reserved_words[i].attribute_c = "or";

		}
		else{
			reserved_words[i].attribute_c = NULL;
		}
		//Give it the proper number

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
		shift(buffer, j + 1, "SEMICOLON", SEMICOLON, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '+'){
		shift(buffer, j + 1, "ADDOP", ADDOP, PLUS, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '-'){
		shift(buffer, j + 1, "ADDOP", ADDOP, MINUS, NULL);
		whitespace(buffer);
	}else if(buffer[j] == ':' && buffer[j + 1] == '='){
		shift(buffer, j + 2, "ASSIGNOP", ASSIGNOP, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == ','){
		shift(buffer, j + 1, "COMMA", COMMA, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '.' && buffer[j + 1] == '.'){
		shift(buffer, j + 2, "DOTDOT", DOTDOT, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '.'){
		shift(buffer, j + 1, "DOT", DOT, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == ':'){
		shift(buffer, j + 1, "COLON", COLON, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '['){
		shift(buffer, j + 1, "OPEN_BRACKET", OPEN_BRACKET, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == ']'){
		shift(buffer, j + 1, "CLOSE_BRACKET", CLOSE_BRACKET, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '{'){
		shift(buffer, j + 1, "OPEN_S_BRACKET", OPEN_S_BRACKET, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '}'){
		shift(buffer, j + 1, "CLOSE_S_BRACKET", CLOSE_S_BRACKET, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '(' && buffer[j + 1] == '*'){
		shift(buffer, j + 1, "COMMENT_OPEN", COMMENT_OPEN, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '*' && buffer[j + 1] == ')'){
		shift(buffer, j + 2, "COMMENT_CLOSE", COMMENT_CLOSE, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '*'){
		shift(buffer, j + 1, "MULOP", MULOP, STAR, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '/'){

		shift(buffer, j + 1, "MULOP", MULOP, SLASH, NULL);
		whitespace(buffer);
	}else if(buffer[j] == '('){

		shift(buffer, j + 1, "OPEN_PAREN", OPEN_PAREN, 0, NULL);
		whitespace(buffer);
	}else if(buffer[j] == ')'){

		shift(buffer, j + 1, "CLOSE_PAREN", CLOSE_PAREN, CLOSE_PAREN, NULL);
		whitespace(buffer);
	}else{
		//Symbol unrecognized LEXERROR
		shift(buffer, j + 1, "LEXERROR",  LEXERROR, UNRECOGNIZED_SYMBOL, "Unrecognized Symbol.");
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
int shift(char* buffer, int j, char* token_type, int token_i, int attribute, char* attribute_ch){
	int flag = 0;
	int out;
	int k = 0;
	char id[j + 1];
	char new_buffer[SIZE];
	int float_flag = 0;
	float attribute_f;
	int char_flag = 0;

	for(k = 0; k < j; k++){
		//save identifier into a temporary buffer called id
		id[k] = buffer[k];
	}
	//null terminate the new buffer
	id[k] = '\0';
	char att;

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
			int space = 40;
			if(strcmp(id, reserved_words[k].lexeme) == 0){
				//printf("(%i, %s, %s, %i)\n", line, reserved_words[k].lexeme, reserved_words[k].token, reserved_words[k].attribute);
				print_token(reserved_words[k]);
				return 1;
			}
			k++;
		}
		//it is not a reserved word. Check the symbol table to see if it already has an entry.
		//it definitely won't be in the list if this is the first element in the list... so check if it is:

		if(list_size == 0){

			struct identifier first_node; //(struct identifier*)malloc(sizeof(struct identifier));
			first_node.lexeme = (char*)malloc(sizeof(id) * sizeof(char));
			strcpy(first_node.lexeme, id);
			first_node.ptr = first_node.lexeme;
			first_node.next = NULL;
			time_t t;
			srand(time(&t));
			first_node.value = rand();
			out = first_node.value;
			ll_start = first_node;
			list_size++;

			flag = 1;
			char_flag = 1;
		}else{
			//check if this identifier has been used before. If so, just update the current pointer to the node char ptr


			int found = find_identifier(id, &out);
			if(found == 0){
				out = add_item(&ll_start, id);
			}else{
				out = found;
			}
			flag = 1;

		}

	}

		//if not a reserved word it needs to go into the symbol table
		struct token new_token;

		new_token.lexeme = (char*)malloc((j+1) * sizeof(char));
		new_token.lexeme = id;
		new_token.token = (char*)malloc(sizeof(token_type) * sizeof(char));
		new_token.token = token_type;
		new_token.attribute_c = (char*)malloc(sizeof(attribute_ch)*sizeof(char));
		new_token.attribute_c = attribute_ch;
		if(token_i == ADDOP && attribute == PLUS){
			new_token.attribute_c = (char*) malloc(sizeof(char) * sizeof("plus"));
			new_token.attribute_c = "plus";
		}
		if(token_i == MULOP && attribute == STAR){
			new_token.attribute_c = (char*) malloc(sizeof(char) * sizeof("star"));
			new_token.attribute_c = "star";
		}
		if(token_i == MULOP && attribute == SLASH){
			new_token.attribute_c = (char*) malloc(sizeof(char) * sizeof("slash"));
			new_token.attribute_c = "slash";
		}
		if(token_i == ADDOP && attribute == MINUS){
			new_token.attribute_c = (char*) malloc(sizeof(char) * sizeof("minus"));
			new_token.attribute_c = "minus";
		}

		if(token_i == INT){
			//int so we need to just give that a value of id
			attribute = atoi(id);
			new_token.int_flag = 1;
			new_token.i_attribute = attribute;

		}else if(token_i == REAL){
			//real
			new_token.float_flag = 1;
			new_token.f_attribute = atof(id);
		}else if(flag == 1){
			new_token.ptr_flag = 1;
			new_token.i_attribute = out;
			new_token.attribute_c = (char*)malloc(sizeof("ptr") * sizeof(char));
			new_token.attribute_c = "ptr";
		//	printf("p = %p", out);

		}else if(char_flag == 1 || token_i == LONG_REAL){
			new_token.char_flag = 1;
			new_token.c_attribute = (char*) malloc(sizeof(char) * sizeof(id));
			new_token.c_attribute = id;
		}else{
			new_token.int_flag = 1;
			new_token.i_attribute = attribute;
		}



		new_token.token_type = token_i;
		print_token(new_token);
		return 1;
}
/*
	This should identify all acceptable numbers.
*/
/*
int long_real(char* buffer){
	int j = 0;
	if(isdigit(buffer[0])){
		//if the first digit is 0
		if( buffer[0] = '0' == 0){
		//check for certain cases. For example
		//Certain cases:
			//0.1
			//0
			//00
			//01111
			//0.01
			j++;
			if(buffer[j] == '.'){
				//only acceptabe state
				//jump to state 3
				//reached state 3
				j++;
				//check for things like 1.0000E1
				while(buffer[j] - '0' == 0){
					j++;
				}
				if(!isdigit(buffer[j])){
					//not a long real
					//kickback to 
					real(buffer);
					whitespace(buffer);
					return 0;

				}else{
					j++;
					//STATE 4
					while(isdigit(buffer[j])){
						//Stay in STATE 4
						j++;
					}
					if(buffer[j] == 'E'){
						//STATE 5
						j++;
						if(buffer[j] == '+' || buffer[j] == '-'){
							//STATE 6
							j++;
							if(isdigit(buffer[j])){
								if(buffer[j] -'0' == 0){
									//kick back
									//not long_real
									//zero exponent
									j++;
									while(isdigit(buffer[j])){
										j++;
									}
									shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zeros exponent.");
									whitespace(buffer);

									return 1;
								//ADD ERROR
								}else{
									j++;
									//STATE 7
									while(isdigit(buffer[j])){
										j++;
									}
									//check length
									shift(buffer);
									whitespace(buffer);
									return 1;
									//whitespace
								}
							}else{
								//kick back, not real
							}
						}else if(isdigit(buffer[j])){
							//assume plus
							if(buffer[j] -'0' == 0){
								//kick back
								//not long_real
								//zero exponent
							}else{
								j++;
								//STATE 7
								while(isdigit(buffer[j])){
									j++;
								}
								//check length
								//shift
								//whitespace
							}
						}else{
							//kick back, not real
						}
					}else{
						//kick back, not a real
					}
				}

			}else if(isdigit(buffer[j])){//unacceptable
				//something like 00E+1 or 00E1
				//clear it
				j++;
				while(isdigit(buffer[j])){
					j++;
				}
				if(buffer[j] == 'E'){
					j++;
					if(buffer[j] == '+' || buffer[j] == '-'){
						j++;
					}else if(isdigit(buffer[j])){

					}else{
						//kick back
						return 0;
					}
					if(isdigit(buffer[j])){
						while(isdigit(buffer[j])){
							j++;
						}
						shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros");
						whitespace(buffer);

					}else{
						//kick back
						return 0;
					}
				}else{

					//kick back bc we are at something like 0111x
					return 0;
				}

			}else if(buffer[j] == 'E'){
				j++;
				if(buffer[j] == '+' || buffer[j] == '-'){
					j++;
				}else if(isdigit(buffer[j])){

				}else{
				//kick back
					return 0;
				}
				if(isdigit(buffer[j])){
					while(isdigit(buffer[j])){
						j++;
					}
					shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros");
					whitespace(buffer);
					}else{
					//kick back
					return 0;
				}
				}else{

					//kick back bc we are at something like 0111x
					return 0;
				}


			}else{
				//kick back
				return 0;
			}

		}else{
			j++;
			//state 2
			while(isdigit(buffer[j])){
				//keep accepting digits
				j++;
			}
			//check for state 3
			if(buffer[j] == '.'){
				//reached state 3
				j++;
				//check for things like 1.0000E1
				while(buffer[j] - '0' == 0){
					j++;
				}
				if(!isdigit(buffer[j])){
					//not a long real
					//kickback to 

				}else{
					j++;
					//STATE 4
					while(isdigit(buffer[j])){
						//Stay in STATE 4
						j++;
					}
					if(buffer[j] == 'E'){
						//STATE 5
						j++;
						if(buffer[j] == '+' || buffer[j] == '-'){
							//STATE 6
							j++;
							if(isdigit(buffer[j])){
								if(buffer[j] -'0' == 0){
									//kick back
									//not long_real
									//zero exponent

								}else{
									j++;
									//STATE 7
									while(isdigit(buffer[j])){
										j++;
									}
									//check length
									//shift
									//whitespace
								}
							}else{
								//kick back, not real
							}
						}else if(isdigit(buffer[j])){
							//assume plus
							if(buffer[j] -'0' == 0){
								//kick back
								//not long_real
								//zero exponent
							}else{
								j++;
								//STATE 7
								while(isdigit(buffer[j])){
									j++;
								}
								//check length
								//shift
								//whitespace
							}
						}else{
							//kick back, not real
						}
					}else{
						//kick back, not a real
					}
				}
			}else if(buffer[j] == 'E'){
				//reached state 5
						//STATE 5
						j++;
						if(buffer[j] == '+' || buffer[j] == '-'){
							//STATE 6
							j++;
							if(isdigit(buffer[j])){
								if(buffer[j] -'0' == 0){
									//kick back
									//not long_real
									//zero exponent

								}else{
									j++;
									//STATE 7
									while(isdigit(buffer[j])){
										j++;
									}
									//check length
									//shift
									//whitespace
								}
							}else{
								//kick back, not real
							}
						}else if(isdigit(buffer[j])){
							//assume plus
							if(buffer[j] -'0' == 0){
								//kick back
								//not long_real
								//zero exponent
							}else{
								j++;
								//STATE 7
								while(isdigit(buffer[j])){
									j++;
								}
								//check length
								//shift
								//whitespace
							}
						}else{
							//kick back, not real
						}
			}else{
				//not a long real!
				//kick back to next machine
			}



		}
	}else{
		//not a long real
		//kick to next machine
	}






}
*/

int long_real(char* buffer){

	if(isdigit(buffer[0])){

		//	If it starts with a 0 it is an error condition.
		if(buffer[0] - '0' == 0 && !isdigit(buffer[1]) && buffer[1] != '.'){
			shift(buffer, 1, "INT", INT, -1, "value");
			whitespace(buffer);
		}else if(buffer[0] - '0' == 0 && buffer[1] != '.'){
			int j = 1;
			//printf("Error in integer format. Cannot start with 0.\n");
			//need to get the entire integer here.
			j++;
			while(isdigit(buffer[j])){
				j++;
			}
			if(buffer[j] == '.'){
				//Still keep Shifting
				j++;
				while(isdigit(buffer[j])){
					j++;
				}
				if(buffer[j] == 'E' && buffer[j + 1] - '0' != 0){
					j++;
					if(buffer[j] == '+' || buffer[j] == '-'){
						j++;
					}
					while(isdigit(buffer[j])){
						j++;
					}
				}
			}else if(buffer[j] == 'E'){
				//Still keep shifting
					j++;

				if((isdigit(buffer[j]) && buffer[j] - '0' != 0)|| buffer[j] == '+' || buffer[j] == '-'){
					while(isdigit(buffer[j])){
						j++;
					}
				}
			}
			shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros");
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
					//	Need to remove the broken number

					shift(buffer, j + 1, "LEXERROR", LEXERROR, FRACTION_SMALL, "Need digit after .");
					whitespace(buffer);
				}else{
					while(isdigit(buffer[j])){
						//keep incrementing
						j++;
					}
					//found long
					//	Something like: 1.23E+13
					if(buffer[j] == 'E' && ((buffer[j + 1] == '+' && isdigit(buffer[j + 2]))|| (buffer[j + 1] == '-'  && isdigit(buffer[j + 2]))|| isdigit(buffer[j + 1]))){
						j++;
						if(buffer[j] == '+' || buffer[j] == '-'){
							j++;
							while(isdigit(buffer[j])){
								j++;
							}
							int k = 0;

							//Shift buffer and print out the long real

							//First do checks:
							int temp = j;
							j = 0;
							while(isdigit(buffer[j])){
								//just keep count of j
								j++;
							}
							if(j > 5){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "First part too long.");
								whitespace(buffer);
								return 1;
							}else{
								//check the second part before the E and make sure it's not too large
								k = 0;
								j = j + 1;
								while(isdigit(buffer[j])){
									//just keep count of j
									j++;
									k++;
								}
								if(k > 5){
									j = temp;
									shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Last part too long.");
									whitespace(buffer);
									return 1;
								}
								//Now check if the part after the E is too long.
								k = 0;
								//increment by 2 because this is of the type 1.23E+21
								j = j + 2;
								while(j < temp){
									j++;
									k++;
								}
								if(k > 2){
									j = temp;
									shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Last part too long.");
									whitespace(buffer);
									return 1;
								}
								temp = j;
								j = 0;
								while(isdigit(buffer[j])){
									j++;
								}
								j++; //342.
								while(isdigit(buffer[j])){
									j++;
								}
								j++; //342.321E
								j++; //342.321E+
								k = 0;
								while(j < temp){
									k++;
									j++;
								}
								if(k == 2){
									if(buffer[temp - k] - '0' == 0 && buffer[temp - k + 1] - '0' == 0){
										j = temp;
										shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero exponent.");
										whitespace(buffer);
										return 1;
									}
									if(buffer[temp - k] - '0' == 0){
										shift(buffer, temp - k - 2, "REAL", REAL, -2, "value");
										whitespace(buffer);
										return -1;
									}
								}else if(k == 1){
									if(buffer[temp - k] - '0' == 0){
										j = temp;
										shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero exponent.");
										whitespace(buffer);
										return 1;
									}
								}

								shift(buffer, j, "LONG_REAL", LONG_REAL, -3, "value");
								whitespace(buffer);
								return 1;
							}
						}else if(isdigit(buffer[j])){
							//Assume it is a plus:
							j++;
							while(isdigit(buffer[j])){
								j++;
							}
							int k = 0;

							//First check
							int temp = j;
							j = 0;
							while(isdigit(buffer[j])){
								//just keep count of j
								j++;
							}
							if(j > 5){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "First part too long.");
								whitespace(buffer);
								return 1;
							}else{
								//check the second part before the E and make sure it's not too large
								k = 0;
								j = j + 1;
								while(isdigit(buffer[j])){
									//just keep count of j
									j++;
									k++;
								}
								if(k > 5){
									j = temp;
									shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Last part too long.");
									whitespace(buffer);
									return 1;
								}
								//Now check if the part after the E is too long.
								k = 0;

								j = j + 1;
								while(j < temp){
									j++;
									k++;
								}
								if(k > 2){
									j = temp;
									shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Last part too long.");
									whitespace(buffer);
									return 1;
								}
								if(k == 2){
									if(buffer[temp - k] - '0' == 0 && buffer[temp - k + 1] - '0' == 0){
										j = temp;
										shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT,"Zero exponent.");
										whitespace(buffer);
										return 1;
									}
//new addition
									if(buffer[temp - k] - '0' == 0){
										shift(buffer, temp - k - 1, "REAL", REAL, -2, "value");
										whitespace(buffer);
										return -1;
									}

								}else if(k == 1){
									if(buffer[temp - k] - '0' == 0){
										j = temp;
										shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero exponent.");
										whitespace(buffer);
									return 1;
								}

									j = temp;
					 				shift(buffer, j, "LONG_REAL", LONG_REAL, -3, "value");
									whitespace(buffer);
							 		return 1;
								}
							}

						}else{
							//printf("Error in formatting of long real. ");
							shift(buffer, j + 1, "LEXERROR", LEXERROR, FORMAT_ERROR, "Error in formatting of real.");
							whitespace(buffer);
						}
					}else{
					//	if(buffer[j - 1] == '0'){
					//		shift(buffer, j, "LEXERROR", LEXERROR, TRAILING_ZEROS, "Trailing zeros");
					//	}else{
							//found a floating point real
							//Check the size of it.
							int temp = j;
							j = 0;
							while(isdigit(buffer[j])){
								//just keep count of j
								j++;
							}
							if(j > 5){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "First half of float too long.");
								whitespace(buffer);
								return 1;
							}else{
								//check the last half and make sure it's not too large
								int k = 0;
								while(j < temp){
									//just keep count of j
									j++;
									k++;
								}
								if(k > 6){
									j = temp;
									shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Last part too long.");
									whitespace(buffer);
									return 1;
								}
								//else, it's fine
								j = temp;
								shift(buffer, j, "REAL", REAL, -1, "value");
								whitespace(buffer);
							}
					//	}
					}
				}
			//	Something like 10E+13
			}else if(buffer[j] == 'E'){
		//	}else if(buffer[j] == 'E' && ((buffer[j + 1] == '+' && isdigit(buffer[j + 2]))|| (buffer[j + 1] == '-'  && isdigit(buffer[j + 2]))|| isdigit(buffer[j + 1]))){

				j++;

				if(buffer[j] == '+' || buffer[j] == '-'){
					j++;
					while(isdigit(buffer[j])){
						j++;
					}
					int k = 0;

					//Check the size of it.
					int temp = j;
					j = 0;
					while(isdigit(buffer[j])){
						//just keep count of j
						j++;
					}
					if(j > 5){
						j = temp;
						shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "First part too long.");
						whitespace(buffer);
						return 1;
					}else{
						//check the last half and make sure it's not too large
						int k = 0;
						j++; // skip over the E portion
						j++; //skip over the + / - portion
						while(j < temp){
							//just keep count of j
							j++;
							k++;
						}
						if(k > 2){
							j = temp;
							shift(buffer, j, "LEXERROR", LEXERROR, E_LONG, "Part after E too long.");
							whitespace(buffer);
							return 1;
						}
						//else, it's probably fine. But first check if the exponents are 0 or 00
						temp = j;
						j = 0;
						while(isdigit(buffer[j])){
							j++;
						}
						j++;
						j++;
						k = 0;
						while(j < temp){
							k++;
							j++;
						}
						if(k == 2){
							if(buffer[temp - k] - '0' == 0 && buffer[temp - k + 1] - '0' == 0){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero Exponent.");
								whitespace(buffer);
								return 1;
							}
//new addition
							if(buffer[temp - k] - '0' == 0){
								shift(buffer, temp - k - 2, "REAL", REAL, -1, "value");
								whitespace(buffer);
								return -1;
							}

						}else if(k == 1){
							if(buffer[temp - k] - '0' == 0){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT,"Zero Exponent.");
								whitespace(buffer);
								return 1;
							}
						}

						j = temp;
						shift(buffer, j, "LONG_REAL", LONG_REAL, -3, "value");
						whitespace(buffer);
					}
					//Something like 123E21
				}else if(isdigit(buffer[j])){
					//Assume it is a plus:
					j++;
					while(isdigit(buffer[j])){
						j++;
					}

					//Check the size of it.
					int temp = j;
					j = 0;
					while(isdigit(buffer[j])){
						//just keep count of j
						j++;
					}
					if(j > 5){
						j = temp;
						shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "First part too long.");
						whitespace(buffer);
						return 1;
					}else{
						//check the last half and make sure it's not too large
						int k = 0;
						j++; // skip E portion
						while(j < temp){
							//just keep count of j
							j++;
							k++;
						}
						if(k > 2){
							j = temp;
							shift(buffer, j, "LEXERROR", LEXERROR, E_LONG, "Part after E too long.");
							whitespace(buffer);
							return 1;
						}
						//else, it's probably fine. But first check if the exponents are 0 or 00
						temp = j;
						j = 0;
						while(isdigit(buffer[j])){
							j++;
						}
						j++;
						k = 0;
						while(j < temp){
							k++;
							j++;
						}
						if(k == 2){
							if(buffer[temp - k] - '0' == 0 && buffer[temp - k + 1] - '0' == 0){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT,"Zero Exponent.");
								whitespace(buffer);
								return 1;
							}
						}else if(k == 1){
							if(buffer[temp - k] - '0' == 0){
								j = temp;
								shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero Exponent.");
								whitespace(buffer);
								return 1;
							}
						}
						j = temp;
						shift(buffer, j, "LONG_REAL", LONG_REAL, -3, "value");
						whitespace(buffer);
						return 1;
					}

				}
			}else{
				//integer. check length
				if (j >= 10){
					shift(buffer, j, "LEXERROR", LEXERROR, INTEGER_LONG, "Integer too long.");
					whitespace(buffer);
					return 1;
				}
				shift(buffer, j, "INT", INT, -1, "value");
				whitespace(buffer);
				return 1;
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
	if(isalpha(buffer[0])){
		j++;

		while((isalpha(buffer[j]) || isdigit(buffer[j]))){
			j++;
		}
		if(j < 11){
			shift(buffer, j, "ID", IDENTIFIER, IDENTIFIER, 0);
			whitespace(buffer);
			return 1;

		}else{
			//identifier too long
			//Still remove the identifier that is too long
			shift(buffer, j, "LEXERROR", LEXERROR, ID_LONG, "Identifier too long.");
			whitespace(buffer);
			return 1;
		}

	}else{
		//else the thing was not an identifier, so send it to the number checker
		long_real(buffer);
		return -1;
	}

}
int relop(char* buffer){
	int j = 0;
	if(buffer[j] == '>' && buffer[j + 1] == '='){
		//Found (relop, GE)
		//Now remove it
		shift(buffer, j + 2, "RELOP", RELOP, GE, "GE");
		whitespace(buffer);
	}else if(buffer[j] == '>'){
		//printf("Found (relop, GT)\n");
		shift(buffer, j + 1, "RELOP", RELOP, GT, "GT");
		whitespace(buffer);
	}else if(buffer[j] == '='){
		//printf("Found (relop, EQ)\n");
		shift(buffer, j + 1, "RELOP", RELOP, EQ, "EQ");
		whitespace(buffer);
	}else if(buffer[j] == '<' && buffer[j + 1] == '='){
		//printf("Found (relop, LE)\n");
		shift(buffer, j + 2, "RELOP", RELOP, LE, "LE");
		whitespace(buffer);

	}else if(buffer[j] == '<' && buffer[j + 1] == '>'){
		//printf("Found (relop, NE)\n");
		shift(buffer, j + 2, "RELOP", RELOP, NE, "NE");
		whitespace(buffer);

	}else if(buffer[j] == '<'){
		//printf("Found (relop, LT)\n");
		shift(buffer, j + 1, "RELOP", RELOP, LT, "LT");
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
	buffer[i + 1] = '\0';
	if(ch != EOF){
		fprintf(outfile,"%i    %s", line,buffer);
	}
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
				line--;
				line--;
				//printf("End of file.\n");
				struct token t;
				t.lexeme = (char*)malloc(sizeof(char) * sizeof("EOF"));
				t.lexeme = "EOF";
				t.i_attribute = 0;
				t.int_flag = 1;
				t.attribute_c = NULL;
				t.token_type = EOF;
				t.token = (char*)malloc(sizeof(char) * sizeof("EOF"));
				t.token = "EOF";
				print_token(t);
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
	printf("%*s %*s           %*s           %*s\n", 20, "LINE NUMBER" , 20, "LEXEME", 20, "TOKEN TYPE", 20, "ATTRIBUTE");
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
//	printf("End of file.\n");
	//print linked list
	print_list();

}
