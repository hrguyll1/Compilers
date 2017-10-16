#include "headers.h"
#include "P2.c"
//struct token reserved_words[20];
//struct identifier ll_start;
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

int getnexttoken(struct token *t, char* buffer){
					printf("Buffer = %s \n", buffer);
					return whitespace(buffer, t);
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
	if(t.token_type == LEXERROR){
		fprintf(outfile,"	%s!  %s :  %s\n", t.token, t.attribute_c, t.lexeme);
	}
        if(t.float_flag == 1){
                fprintf(tokenfile, "%*i %*s        (%i) %*s           %*f   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.f_attribute, t.attribute_c);
        }else if(t.int_flag == 1){
                fprintf(tokenfile, "%*i %*s        (%i) %*s           %*i   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.i_attribute, t.attribute_c);
	}else if(t.ptr_flag == 1){
                fprintf(tokenfile, "%*i %*s        (%i) %*s           %*i   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.i_attribute, t.attribute_c);
        }else{
                fprintf(tokenfile, "%*i %*s        (%i) %*s           %*s   (%s)   \n", 20, line, 20, t.lexeme, t.token_type, 20, t.token, 20, t.c_attribute, t.attribute_c);
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


int catch_all(char* buffer, struct token *t){
		printf("IN CATCH ALL MACHINE!\n");
	int j = 0;
	if(buffer[j] == ';'){
		*t = shift(buffer, j + 1, "SEMICOLON", SEMICOLON, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '+'){
		*t = shift(buffer, j + 1, "ADDOP", ADDOP, PLUS, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '-'){
		*t = shift(buffer, j + 1, "ADDOP", ADDOP, MINUS, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == ':' && buffer[j + 1] == '='){
		*t = shift(buffer, j + 2, "ASSIGNOP", ASSIGNOP, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == ','){
		*t = shift(buffer, j + 1, "COMMA", COMMA, 0, NULL);
		return 1;
	//	whitespace(buffer);
	}else if(buffer[j] == '.' && buffer[j + 1] == '.'){
		*t = shift(buffer, j + 2, "DOTDOT", DOTDOT, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '.'){
		printf("FOUND DOT!\n");
		*t = shift(buffer, j + 1, "DOT", DOT, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == ':'){
		*t = shift(buffer, j + 1, "COLON", COLON, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '['){
		*t = shift(buffer, j + 1, "OPEN_BRACKET", OPEN_BRACKET, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == ']'){
		*t = shift(buffer, j + 1, "CLOSE_BRACKET", CLOSE_BRACKET, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '{'){
		*t = shift(buffer, j + 1, "OPEN_S_BRACKET", OPEN_S_BRACKET, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '}'){
		*t = shift(buffer, j + 1, "CLOSE_S_BRACKET", CLOSE_S_BRACKET, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '(' && buffer[j + 1] == '*'){
		*t = shift(buffer, j + 1, "COMMENT_OPEN", COMMENT_OPEN, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '*' && buffer[j + 1] == ')'){
		*t = shift(buffer, j + 2, "COMMENT_CLOSE", COMMENT_CLOSE, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '*'){
		*t = shift(buffer, j + 1, "MULOP", MULOP, STAR, NULL);
		return 1;
		// whitespace(buffer);
	}else if(buffer[j] == '/'){
		*t = shift(buffer, j + 1, "MULOP", MULOP, SLASH, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '('){
		*t = shift(buffer, j + 1, "OPEN_PAREN", OPEN_PAREN, 0, NULL);
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == ')'){
		*t = shift(buffer, j + 1, "CLOSE_PAREN", CLOSE_PAREN, CLOSE_PAREN, NULL);
		return 1;
		//whitespace(buffer);
	}else{
		//Symbol unrecognized LEXERROR
		*t = shift(buffer, j + 1, "LEXERROR",  LEXERROR, UNRECOGNIZED_SYMBOL, "Unrecognized Symbol.");
		return 1;
		//whitespace(buffer);
	}
}
int clear(char* buffer){
	int j = 0;
	for(j = 0; j < 72; j++){
		buffer[j] = '\0';
	}
}
//struct token shfit(char* buffer, int j, char* token_type, int attribute){
struct token shift(char* buffer, int j, char* token_type, int token_i, int attribute, char* attribute_ch){
	printf("IN SHIFT!!!\n");
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
				return reserved_words[k];
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
		return new_token;
}

int long_real(char* buffer, struct token *t){
		printf("IN LONG_REAL MACHINE!\n");
  int exponent_counter = 0;
  int before_decimal_counter = 0;
  int after_decimal_counter = 0;
  int last_decimal_index = 0;
  int first_exponent_index = 0;

  int j = 0;
  if(isdigit(buffer[0])){
    //proceed to state 2
    j++;
    before_decimal_counter++;
    //can keep getting digits
    while(isdigit(buffer[j])){
      before_decimal_counter++;
      j++;
    }
    //go to state 3
    if(buffer[j] == '.'){
      j++;
      //go to state 4
      if(isdigit(buffer[j])){
        j++;
        after_decimal_counter++;
        while(isdigit(buffer[j])){
          j++;
          after_decimal_counter++;
        }
        last_decimal_index = j - 1;
        if(buffer[j] == 'E'){
          //proceed to state 5
          j++;
          if(buffer[j] == '+' || buffer[j] == '-'){
            j++;
            if(isdigit(buffer[j])){
              first_exponent_index = j;
              exponent_counter++;
              j++;
              while(isdigit(buffer[j])){
                exponent_counter++;
                j++;
              }
            //  printf("before decimal = %i after decimal = %i exponent counter = %i \n", before_decimal_counter, after_decimal_counter,exponent_counter);
              /*
                TO DO: Error Checking
                1. Check lengths of fields.
                2. Check for trailing zeros.
                3. Check for leading zeros.
                4. Check for zero exponents.
              */
              //1. Length Checks
              if(before_decimal_counter > 5){
                //error condition
                *t = shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "Fraction part too long.");
                return 1;
              }else if(after_decimal_counter > 5){
                //error condition 2
                *t = shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Decimal part too long.");
                return 1;
              }else if(exponent_counter > 2){
                *t = shift(buffer, j, "LEXERROR", LEXERROR, EXPONENT_LONG, "Exponent part too long.");
                return 1;
              }
             //2. Leading Zeros
              if(before_decimal_counter > 1){
                //see if it has leading zeros
                if(buffer[0] - '0' == 0){
                  *t = shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros.");
                  return 1;
                }
              }
              //3. Trailing zeros
              if(after_decimal_counter > 1){
                if(buffer[last_decimal_index] - '0' == 0){
                  *t = shift(buffer, j, "LEXERROR", LEXERROR, TRAILING_ZEROS, "Trailing Zeros.");
                  return 1;
                }
              }
              //4.Check for 0 Exponents
              if(buffer[first_exponent_index] - '0' == 0){
                *t = shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero exponent.");
                return 1;
              }
              *t = shift(buffer, j, "LONG_REAL", LONG_REAL, -2, "value");
              return 1;
            }else{
              return real(buffer, t);
            }
  //--------------------------ELSE, NO SIGN SO WE ASSUME IT'S A PLUS--------------------------------------------
          }else if(isdigit(buffer[j])){
            while(isdigit(buffer[j])){
              first_exponent_index = j;
              exponent_counter++;
              j++;
            }
            /*
              TO DO: Error Checking
              1. Check lengths of fields.
              2. Check for trailing zeros.
              3. Check for leading zeros.
              4. Check for zero exponents.
            */
            //1. Length Checks
            if(before_decimal_counter > 5){
              //error condition
              *t = shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "Fraction part too long.");
              return 1;
            }else if(after_decimal_counter > 5){
              //error condition 2
							*t = shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Decimal part too long.");
              return 1;
            }else if(exponent_counter > 2){
              *t = shift(buffer, j, "LEXERROR", LEXERROR, EXPONENT_LONG, "Exponent part too long.");
              return 1;
            }
            //2. Leading Zeros
            if(before_decimal_counter > 1){
              //see if it has leading zeros
              if(buffer[0] - '0' == 0){
                *t = shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros.");
                return 1;
              }
            }
            //3. Trailing zeros
            if(after_decimal_counter > 1){
              if(buffer[last_decimal_index] - '0' == 0){
                *t = shift(buffer, j, "LEXERROR", LEXERROR, TRAILING_ZEROS, "Trailing Zeros.");
                return 1;
              }
            }
            //4.Check for 0 Exponents
            if(buffer[first_exponent_index] - '0' == 0){
              *t = shift(buffer, j, "LEXERROR", LEXERROR, ZERO_EXPONENT, "Zero exponent.");
              return 1;
            }
            //printf("before decimal = %i after decimal = %i exponent counter = %i \n", before_decimal_counter, after_decimal_counter,exponent_counter);
					 	*t = shift(buffer, j, "LONG_REAL", LONG_REAL, -2, "value");
            return 1;
          }else{
            return real(buffer, t);
          }
        }else{
          return real(buffer, t);
        }
      }else{
        return real(buffer, t);
      }
    }else{
      return real(buffer, t);
    }
  }else{
    return real(buffer, t);
  }
  return real(buffer, t);
}

//Check for real. Of the form x.y where |x| <= 5 and |y| <= 5
int real(char* buffer, struct token *t){
		printf("IN REAL MACHINE!\n");
  int before_decimal_counter = 0;
  int after_decimal_counter = 0;

  int j = 0;
  if(isdigit(buffer[0])){
    j++;
    while(isdigit(buffer[j])){
      j++;
    }
    before_decimal_counter = j;
    if(buffer[j] == '.'){
      j++;
      if(isdigit(buffer[j])){
        j++;
        after_decimal_counter++;
        while(isdigit(buffer[j])){
          j++;
          after_decimal_counter++;
        }
        //tokenize the thing.
        /*
          Error Checking:
          1. Check amount of digits before decimal.
          2. Check amount of digits after decimal.
          3. Check for leading zeros.
          4. Check for trailing zeros.
        */
        if(before_decimal_counter > 5){
          *t = shift(buffer, j, "LEXERROR", LEXERROR, FRACTION_LONG, "Fraction part too long.");
          return 1;
        }
        if(after_decimal_counter > 5){
          *t = shift(buffer, j, "LEXERROR", LEXERROR, DECIMAL_LONG, "Decimal part too long.");
          return 1;
        }
        if(before_decimal_counter > 1){
          if(buffer[0] - '0' == 0){
            *t = shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading Zeros.");
            return 1;
          }
        }
        if(after_decimal_counter > 2){
          if(buffer[j - 1] -'0' == 0){
            *t = shift(buffer, j, "LEXERROR", LEXERROR, TRAILING_ZEROS, "Trailing Zeros.");
            return 1;
          }
        }
      	*t = shift(buffer, j, "REAL", REAL, REAL, "value");
        return 1;
      }else{
        return integer(buffer, t);
      }
    }else{
      return integer(buffer, t);
    }
  }else{
    return integer(buffer, t);
  }
  return integer(buffer, t);
}

//Check for integers. If # digits > 10, error. If leadings zeros occur, error.
int integer(char* buffer, struct token *t){
		printf("IN INTEGER MACHINE!\n");
  int j = 0;
  if(isdigit(buffer[0])){
    j++;
    while(isdigit(buffer[j])){
      j++;
    }
    //tokenize
    // to do: add error Checking
    // leading zeros is the only issue and that's only an issue if the length is longer than 1 or if the integer is greater than 10 digits.
    if(j > 10){
				*t = shift(buffer, j, "LEXERROR", LEXERROR, INTEGER_LONG, "Integer too long.");
        return 1;
    }if(buffer[0] - '0' == 0){
      *t = shift(buffer, j, "LEXERROR", LEXERROR, LEADING_ZEROS, "Leading zeros.");
      return 1;
    }else{
	    *t = shift(buffer, j, "INT", INT, -1, "value");
	    return 1;
		}
  }else{
    return catch_all(buffer, t);
  }
  return catch_all(buffer, t);
}

//	Identifer:
//	Up to 10 character
//	Starts character
//	Can have numbers after

int identifier(char* buffer, struct token *t){
	printf("IN IDENTIFIER MACHINE");
	int j = 0;
	if(isalpha(buffer[0])){
		j++;

		while((isalpha(buffer[j]) || isdigit(buffer[j]))){
			j++;
		}
		if(j < 11){
			*t = shift(buffer, j, "ID", IDENTIFIER, IDENTIFIER, 0);
			//whitespace(buffer);
			return 1;
		}else{
			//identifier too long
			//Still remove the identifier that is too long
			*t = shift(buffer, j, "LEXERROR", LEXERROR, ID_LONG, "Identifier too long.");
			return 1;
		}
	}else{
		return relop(buffer, t);
	}
}
int relop(char* buffer, struct token *t){
		printf("IN RELOP MACHINE!\n");
	int j = 0;
	if(buffer[j] == '>' && buffer[j + 1] == '='){
		//Found (relop, GE)
		*t = shift(buffer, j + 2, "RELOP", RELOP, GE, "GE");
		return 1;
	}else if(buffer[j] == '>'){
		//printf("Found (relop, GT)\n");
		*t = shift(buffer, j + 1, "RELOP", RELOP, GT, "GT");
		return 1;
	}else if(buffer[j] == '='){
		//printf("Found (relop, EQ)\n");
		*t = shift(buffer, j + 1, "RELOP", RELOP, EQ, "EQ");
		return 1;
	}else if(buffer[j] == '<' && buffer[j + 1] == '='){
		//printf("Found (relop, LE)\n");
		*t = shift(buffer, j + 2, "RELOP", RELOP, LE, "LE");
		return 1;
		//whitespace(buffer);
	}else if(buffer[j] == '<' && buffer[j + 1] == '>'){
		//printf("Found (relop, NE)\n");
		*t = shift(buffer, j + 2, "RELOP", RELOP, NE, "NE");
		return 1;
		//whitespace(buffer);

	}else if(buffer[j] == '<'){
		//printf("Found (relop, LT)\n");
		*t = shift(buffer, j + 1, "RELOP", RELOP, LT, "LT");
		return 1;
		//whitespace(buffer);

	}else{
		//printf("Not a relop\n");
		//return 0;
		return long_real(buffer, t);
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
int whitespace(char* buffer, struct token *t){
	printf("IN WHITESPACE MACHINE!\n");
	int i = 0;
	while(1){
		//Look for something other than whitespace
		if(buffer[i] == '\n'){
			clear(buffer);
			line++;

			int status = readline(buffer);
			if(status == -1){
				//line--;
				//line--;
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
      //  line++;
      //  line++;
        fprintf(outfile,"%i     EOF\n", line++);

				return -1;
				//break;
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
			printf("buffer[i] = %c", buffer[i]);
			char new_buffer[SIZE];
			int z = 0;
			int k = 0;
			for(k = i; k < 72; k++){
				new_buffer[z] = buffer[k];
				z++;
			}
			//buffer = new_buffer;
			strcpy(buffer, new_buffer);
			printf("new buffer[i] = %c", buffer[0]);
			struct token t;
			return identifier(buffer, &t);
		}
	}

	return 0;
}

int main(){
	file = fopen("sourcefile.txt", "r");
	outfile = fopen("outfile.txt", "w");
  tokenfile = fopen("tokenfile.txt", "w");
  if( file == NULL || outfile == NULL || tokenfile == NULL){
    printf("Error opening files.\n");
    return -1;
  }
	get_reserved_words();
  fprintf(tokenfile, "%*s %*s           %*s           %*s\n", 20, "LINE NUMBER" , 20, "LEXEME", 20, "TOKEN TYPE", 20, "ATTRIBUTE");


	char buffer[SIZE];
	int i = 0;
	int j = 0;
	int status = 0;
	int ret = 0;
	struct token t;// = malloc(sizeof(struct token));
	readline(buffer);
	do{
		ret = getnexttoken(&t, buffer);
		printf("RETURN = %i\n", ret);
	}while(ret == 1);
	/*
	while(readline(buffer) != -1){
		whitespace(buffer);
		clear(buffer);
	}
	*/
//	printf("End of file.\n");
	//print linked list
	print_list();
  fclose(tokenfile);
  fclose(outfile);
  fclose(file);
}
