#include "headers2.h"

void parse(struct token *t, char* buffer){
  printf("\n\n\n*****************IN PARSE*****************\n\n\n");
  getnexttoken(t, buffer);
  program(t, buffer);
  if(match(EOF, t, buffer) == 0){
    //just keep lex analyzing
    while(t->token_type != EOF){
      getnexttoken(t, buffer);
    }
  }
}
int match(int type, struct token *t, char* buffer){
  //case 1
  if(type == t->token_type){
			if(type == EOF){
        printf("\n\n$$$$$$$$ End of parse. $$$$$$$ \n\n");
				return -1; // end of parse
			}else{
				getnexttoken(t, buffer);
				return 1;
			}
  }else{
      if(t->token_type != LEXERROR){
        printf("\n        SYNTAX ERROR on line %i. Expecting %i but received %s.\n\n", line, type, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting %s but received %s.\n", convert_define(type), t->lexeme);
      }
      return 0;
  }
}

int program(struct token *t, char* buffer){
  //prog -> program id ( idlst ) ; prog'
  switch(t->token_type){
    case PROGRAM:
      match(PROGRAM, t, buffer);
      match(IDENTIFIER, t, buffer);
      match(OPEN_PAREN, t, buffer);

      idlist(t, buffer);

      match(CLOSE_PAREN, t, buffer);
      match(SEMICOLON, t, buffer);

      printf("Token before program 1 = %s\n", t->lexeme);
      program1(t, buffer);

      return 1;

    default:
      if(t->token_type != LEXERROR){
        printf("\nSYNTAX ERROR: Ended the file before finding a match WITHIN PROGRAM.\n\n");
        fprintf(outfile, "    SYNTAX ERROR: Expecting program but received %s\n", t->lexeme);
      }
      while(t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int idlist(struct token *t, char* buffer){
    /*
      Adding a switch statement to this function.
      Previously it just did match and called idlist1.
      The reason I did this is to incorporate the
      sync set in the default.

    */
		printf("\n\n\n*****************IN IDLIST*****************\n\n\n");
    switch(t->token_type){
      case IDENTIFIER:
        //ID : idlst -> id idlst'
        match(IDENTIFIER, t, buffer);
        idlist1(t, buffer);
        return 1;
      default:
        if(t->token_type != LEXERROR){
          fprintf(outfile, "SYNTAX ERROR: Expected IDENTIFIER but received %s\n", t->lexeme);
        }
        while(t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
        }
        return 0;

    }
}
int idlist1(struct token *t, char* buffer){
		printf("\n\n\n*****************IN IDLIST1*****************\n\n\n");
    switch(t->token_type){
      case CLOSE_PAREN:
        //epsilon
        //match(CLOSE_PAREN, t, buffer);
        return 1;
      case COMMA:
        //case idlst' -> , id idlst'
        match(COMMA, t, buffer);
        match(IDENTIFIER, t, buffer);
        idlist1(t, buffer);
        return 1;

      default:
        if(t->token_type != LEXERROR){
          printf("\nError on line %i within IDLIST1.Expected ) or , and received %s\n\n", line, t->lexeme);
          fprintf(outfile, "SYNTAX ERROR: Expected CLOSE_PAREN or COMMA received %s\n", t->lexeme);
        }
        /*
          Follow = )
        */
        while(t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
        }
        return 0;
    }
}

int declarations(struct token *t, char* buffer){
		printf("\n\n\n****************IN DEC*****************\n\n\n");

    switch(t->token_type){
      //VAR: decs -> var id : type ; decs'
      case VAR:
        match(VAR, t, buffer);
        match(IDENTIFIER, t, buffer);
        match(COLON, t, buffer);
        type(t, buffer);
        match(SEMICOLON, t, buffer);
        declarations1(t, buffer);
        return 1;
      default:
        if(t->token_type != LEXERROR){
          fprintf(outfile, "        SYNTAX ERROR: Expecting VAR and received %s.\n", t->lexeme);
          printf("Error in declarations. Expecting VAR but received %s.\n", t->lexeme);
        }
        while(t->token_type != FUNCTION && t->token_type != BEGIN && t->token_type != EOF){
            getnexttoken(t, buffer);
        }
        return 0;
    }
}
int declarations1(struct token *t, char* buffer){
		printf("\n\n\n*****************IN DEC1*****************\n\n\n");
    switch(t->token_type){
      case VAR:
        //var: decs' -> var id : type ; decs'
        match(VAR, t, buffer);
        match(IDENTIFIER, t, buffer);
        match(COLON, t, buffer);
        type(t, buffer);
        match(SEMICOLON, t, buffer);
        declarations1(t, buffer);
        return 1;
      case FUNCTION:
        //function : epsilon
        return 1;
      case BEGIN:
        //begin: epsilon
        return 1;
      default:
        if(t->token_type != LEXERROR){
          fprintf(outfile, "        SYNTAX ERROR: Expecting VAR, FUNCTION, or BEGIN and received %s.\n", t->lexeme);
          printf("Error in declarations1. Expecting VAR, FUNCTION or BEGIN but received %s.\n", t->lexeme);
        }
        while(t->token_type != FUNCTION && t->token_type != BEGIN && t->token_type != EOF){
            getnexttoken(t, buffer);
        }
        return 0;
  }
}

int program1(struct token *t, char* buffer){
		printf("\n\n\n*****************IN PROG1*****************\n\n\n");
    switch(t->token_type){
      case VAR:
        //VAR : prog’ -> decs prog’’
        declarations(t, buffer);
        program2(t, buffer);
        break;
      case FUNCTION:
        //FUNCTION: prog’ -> subprogdecs cmpdstmt .
        subprogdecs(t, buffer);
        compound_statement(t, buffer);
        match(DOT, t, buffer);
        break;
      case BEGIN:
        //BEGIN: prog’ -> cmpdstmt .
        compound_statement(t, buffer);
        match(DOT, t, buffer);
        break;
      default:
      if(t->token_type != LEXERROR){

          fprintf(outfile, "        SYNTAX ERROR: Expecting VAR, FUNCTION or BEGIN and received %s.\n", t->lexeme);
          printf("Error in Program1.\n");
        }
        while(t->token_type != EOF){
            getnexttoken(t, buffer);
        }
        return 0;
    }
}
int program2(struct token *t, char* buffer){
  printf("\n\n\n*****************IN PROG2*****************\n\n\n");
  switch(t->token_type){
    // FUNCTION: prog’’ -> subprogdecs cmpdstmt .
    case FUNCTION:
      subprogdecs(t, buffer);
      compound_statement(t, buffer);
      match(DOT, t, buffer);
      return 1;

    case BEGIN:
      // BEGIN : prog’’ -> cmpdstmt .
      compound_statement(t, buffer);
      match(DOT, t, buffer);
      return 1;

    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION or BEGIN and received %s.\n", t->lexeme);
        printf("Error in Program2 on line %i. Expecting FUNCTION OR BEGIN and received %s.\n", line, t->lexeme);
      }
      while(t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}

int subprogdecs(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SUBPROGDECS*****************\n\n\n");
  switch (t->token_type){
    case FUNCTION:
      //subprogdecs -> subprogdec ; suprogdecs’
      subprogdec(t, buffer);
      match(SEMICOLON, t, buffer);
      subprogdecs1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION and received %s.\n", t->lexeme);
        printf("Error in SUBPROGDECS on line %i. Expecting FUNCTION and received %s.\n", line, t->lexeme);
      }
      while(t->token_type != BEGIN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int subprogdecs1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SUBPROGDECS1*****************\n\n\n");
  switch (t->token_type){
    case FUNCTION:
    //function: subprog_declarations' -> subprog_declaration ; subprog_declarations'
      subprogdec(t, buffer);
      match(SEMICOLON, t, buffer);
      subprogdecs1(t, buffer);
      return 1;
    case BEGIN:
      //Begin : epsilon
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION or BEGIN and received %s.\n", t->lexeme);
        printf("Error in SUBPROGDECS1 on line %i. Expecting FUNCTION or BEGIN and received %s.\n", line, t->lexeme);
      }
      while(t->token_type != BEGIN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}

int subprogdec(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SUBPROGDEC*****************\n\n\n");
  switch (t->token_type){
    case FUNCTION:
      //subprogdec -> subproghead subprogdec’
      subproghead(t, buffer);
      subprogdec1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION and received %s.\n", t->lexeme);
        printf("Error in SUBPROGDEC on line %i. Expecting FUNCTION or BEGIN and received %s. \n", line, t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}
int subprogdec1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SUBPROGDEC1*****************\n\n\n");
  switch (t->token_type){
    case VAR:
      //VAR: subprogdec’ -> decs subprogdec’’
      declarations(t, buffer);
      subprogdec2(t, buffer);
      return 1;
    case FUNCTION:
      //function: subprogdec’ -> subprogdecs cmpdstmt
      subprogdecs(t, buffer);
      compound_statement(t, buffer);
      return 1;
    case BEGIN:
      //begin : subprogdec’ -> cmpdstmt
      compound_statement(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting VAR, FUNCTION, or BEGIN and received %s.\n", t->lexeme);
        printf("Error in SUBPROGDEC1 on line %i. Expecting FUNCTION or BEGIN or VAR and received %s.\n", line, t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}
int subprogdec2(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SUBPROGDEC2*****************\n\n\n");
  switch(t->token_type){
    case FUNCTION:
      //FUNCTION: subprogdec’’ -> subprogdecs cmpdstmt
        subprogdecs(t, buffer);
        compound_statement(t, buffer);
        return 1;
    case BEGIN:
      //BEGIN: subprogdec’’ -> cmpdstmt
      compound_statement(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION or BEGIN and received %s.\n", t->lexeme);
        printf("Error in SUBPROGDEC2 on line %i. Expecting FUNCTION or BEGIN and received %s.\n", line, t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}

int compound_statement(struct token *t, char* buffer){
  printf("\n\n\n*****************IN compound_statement*****************\n\n\n");
  // BEGIN: cmpdstmt -> begin cmpdstmt'
  switch(t->token_type){
    case BEGIN:
      match(BEGIN, t, buffer);
      compound_statement1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting BEGIN and received %s.\n", t->lexeme);
        printf("Error in compound_statement on line %i. Expected BEGIN but received %s.", line, t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != DOT && t->token_type != ELSE && t->token_type != END && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int compound_statement1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN compound_statement1*****************\n\n\n");
  switch(t->token_type){
    case BEGIN:
      //BEGIN : cmpdstmt’ -> optstmt end
      optional_statements(t, buffer);
      match(END, t, buffer);
      return 1;
    case IDENTIFIER:
      //ID : cmpdstmt’ -> optstmt end
      optional_statements(t, buffer);
      match(END, t, buffer);
      return 1;
    case END:
      //END : cmpdstmt’ -> end
      match(END, t, buffer);
      return 1;
    case IF:
      // IF : cmpdstmt’ -> optstmt end
      optional_statements(t, buffer);
      match(END, t, buffer);
      return 1;
    case WHILE:
      // WHILE : cmpdstmt’ -> optstmt end
      optional_statements(t, buffer);
      match(END, t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        printf("Error in compound_statement on line %i. Expected ID, END, IF, WHILE, or BEGIN but received %s.", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting BEGIN, IDENTIFIER, END, IF, or WHILE and received %s.\n", t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != DOT && t->token_type != ELSE && t->token_type != END && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}
int optional_statements(struct token *t, char* buffer){
  printf("\n\n\n*****************IN optional_statements*****************\n\n\n");
  switch(t->token_type){
    case IDENTIFIER:
      //id : optstmt -> stmtlst
      statement_list(t, buffer);
      return 1;
    case BEGIN:
      // begin : optstmt -> stmtlst
      statement_list(t, buffer);
      return 1;
    case IF:
      // if : optstmt -> stmtlst
      statement_list(t, buffer);
      return 1;
    case WHILE:
      //while: optstmt -> stmtlst
      statement_list(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        fprintf(outfile, "        SYNTAX ERROR: Expecting IDENTIFIER, BEGIN, IF or WHILE and received %s.\n", t->lexeme);
        printf("Error in optional_statements on line %i. Expected ID, BEGIN, IF OR WHILE but received %s.", line, t->lexeme);
      }
      while(t->token_type != END && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }

}
int subproghead(struct token *t, char* buffer){
  printf("\n\n\n*****************IN subproghead*****************\n\n\n");
  //FUNCTION : subproghead -> funct id subproghead’
  switch(t->token_type){
    case FUNCTION:
      match(FUNCTION, t, buffer);
      match(IDENTIFIER, t, buffer);
      subproghead1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        fprintf(outfile, "        SYNTAX ERROR: Expecting FUNCTION and received %s.\n", t->lexeme);
        printf("Error in SUBPROGHEAD on line %i. Expected FUNCTION but received %s.", line, t->lexeme);
      }
      while(t->token_type != VAR && t->token_type != FUNCTION && t->token_type != BEGIN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}
int subproghead1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN subproghead1*****************\n\n\n");
  switch(t->token_type){
    case OPEN_PAREN:
      //subproghead’ -> args : stndtype ;
      arguments(t, buffer);
      match(COLON, t, buffer);
      standard_type(t, buffer);
      match(SEMICOLON, t, buffer);
      return 1;
    case COLON:
      //subproghead’ -> : stndtype ;
      match(COLON, t, buffer);
      standard_type(t, buffer);
      match(SEMICOLON, t, buffer);
      return 1;
  default:
    if(t->token_type != LEXERROR){

      printf("Error in SUBPROGHEAD1 on line %i. Expected FUNCTION but received %s.", line, t->lexeme);
      fprintf(outfile, "        SYNTAX ERROR: Expecting CLOSE_PAREN or COLON and received %s.\n", t->lexeme);
    }
    while(t->token_type != VAR && t->token_type != FUNCTION && t->token_type != BEGIN && t->token_type != EOF){
        getnexttoken(t, buffer);
    }
    return 0;
  }
}
int arguments(struct token *t, char* buffer){
  printf("\n\n\n*****************IN ARGUMENTS*****************\n\n\n");
  //( args -> ( paramlst )
  switch (t->token_type){
    case OPEN_PAREN:
      match(OPEN_PAREN, t, buffer);
      parameter_list(t, buffer);
      match(CLOSE_PAREN, t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        fprintf(outfile, "        SYNTAX ERROR: Expecting OPEN_PAREN and received %s.\n", t->lexeme);
      }
      while(t->token_type != COLON && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
    }
}

int standard_type(struct token *t, char* buffer){
  printf("\n\n\n*****************IN STANDARD_TYPE*****************\n\n\n");
  switch(t->token_type){
    case 115: //integer
      match(115, t, buffer);
      return 1;
    case 123: //real
      match(123, t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        printf("Error in Standard_type on line %i. Expected integer or real but received %i.\n", line, t->token_type);
        fprintf(outfile, "        SYNTAX ERROR: Expecting INTEGER or REAL and received %s.\n", t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int parameter_list(struct token *t, char* buffer){
  printf("\n\n\n*****************IN PARAMETER_LIST*****************\n\n\n");
  switch(t->token_type){
    //ID: paramlst -> id : type paramlst'
    case IDENTIFIER:
      match(IDENTIFIER, t, buffer);
      match(COLON, t, buffer);
      type(t, buffer);
      parameter_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting IDENTIFIER and received %s.\n", t->lexeme);
      }
      while(t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int parameter_list1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN PARAMETER_LIST1*****************\n\n\n");
  switch(t->token_type){
    case CLOSE_PAREN:
      printf("Hit the close paren.\n))))))");
      // ) : epsilon
      return 1;
    case SEMICOLON:
      // ; : paramlst' -> ; id : type paramlst'
      match(SEMICOLON, t, buffer);
      match(IDENTIFIER, t, buffer);
      match(COLON, t, buffer);
      type(t, buffer);
      parameter_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("Error in PARAMETER_LIST1 on line %i. Expecting SEMICOLON or CLOSE_PAREN but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting CLOSE_PAREN or SEMICOLON and received %s.\n", t->lexeme);
      }
      while(t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}

int statement_list(struct token *t, char* buffer){
  printf("\n\n\n*****************IN STATEMENT_LIST*****************\n\n\n");
  switch(t->token_type){
    //id : stmtlst -> stmt stmtlst’
    case IDENTIFIER:
      statement(t, buffer);
      statement_list1(t, buffer);
      return 1;
    case BEGIN:
      // begin : stmtlst -> stmt stmtlst’
      statement(t, buffer);
      statement_list1(t, buffer);
      return 1;
    case IF:
      // if : stmtlst -> stmt stmtlst’
      statement(t, buffer);
      statement_list1(t, buffer);
      return 1;
    case WHILE:
      // while : stmtlst -> stmt stmtlst’
      statement(t, buffer);
      statement_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting IDENTIFIER, BEGIN, IF, or WHILE and received %s.\n", t->lexeme);
        printf("Error in STATEMENT_LIST on line %i. Expecting id, begin, if or while but received %s.\n", line, t->lexeme);
      }
      while(t->token_type != END && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int statement_list1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN STATEMENT_LIST1*****************\n\n\n");
  switch(t->token_type){
    case END:
      //END : epsilon
      return 1;
    case SEMICOLON:
      // ; : stmtlst’ -> ; stmt stmtlst’
      match(SEMICOLON, t, buffer);
      statement(t, buffer);
      statement_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){

        printf("Error in STATEMENT_LIST1 on line %i. Expecting END or ;  but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting END or SEMICOLON and received %s.\n", t->lexeme);
      }
      while(t->token_type != END && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}

int statement(struct token *t, char* buffer){
  printf("\n\n\n*****************IN STATEMENT*****************\n\n\n");
  switch(t->token_type){
    case VAR:
      //VAR : stmt -> var assignop exp
      match(VAR, t, buffer);
      match(ASSIGNOP, t, buffer);
      expression(t, buffer);
      return 1;
    case BEGIN:
      // BEGIN : stmt -> cmpdstmt
      compound_statement(t, buffer);
      return 1;
    case IF:
      // IF : stmt -> if exp then stmt stmt’
      match(IF, t, buffer);
      expression(t, buffer);
      match(THEN, t, buffer);
      statement(t, buffer);
      statement1(t, buffer);
      return 1;
    case WHILE:
      // WHILE : stmt -> while exp do stmt
      match(WHILE, t, buffer);
      expression(t, buffer);
      printf("about to try to match do");
      match(DO, t, buffer);
      statement(t, buffer);
      return 1;
    case IDENTIFIER:
      //statement -> variable assignop expression
      variable(t, buffer);
      match(ASSIGNOP, t, buffer);
      expression(t, buffer);
      return 1;
    default:
        if(t->token_type != LEXERROR){
          fprintf(outfile, "        SYNTAX ERROR: Expecting VAR, COMMA, BEGIN, IF, WHILE, or IDENTIFIER and received %s.\n", t->lexeme);
          printf("Error in STATEMENT on line %i. Expecting var, begin, if or while but received %s.\n", line, t->lexeme);
        }
        while(t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != EOF){
            getnexttoken(t, buffer);
        }
        return 0;
    }
}
int statement1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN STATEMENT1*****************\n\n\n");
  switch(t->token_type){
    case SEMICOLON:
      // ; epsilon
      return 1;
    case END:
      //end epsilon
      return 1;
    case ELSE:
      //else : else statement statment1
      match(ELSE, t, buffer);
      statement(t, buffer);
      statement1(t, buffer);
      return 1;
  default:
    if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting SEMICOLON, END, or ELSE and received %s.\n", t->lexeme);
        printf("Error in STATEMENT1 on line %i. Expecting ; end or else but received %s.\n", line, t->lexeme);
      }
      while(t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}

int variable(struct token *t, char* buffer){
  printf("\n\n\n*****************IN VARIABLE*****************\n\n\n");
  //ID
  //var -> id var'
  switch(t->token_type){
    case IDENTIFIER:
      match(IDENTIFIER, t, buffer);
      variable1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting IDENTIFIER and received %s.\n", t->lexeme);
      }
      while(t->token_type != ASSIGNOP && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}
int variable1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN VARIABLE1*****************\n\n\n");
  switch(t->token_type){
    case OPEN_BRACKET:
      //variable' -> [expression]
      match(OPEN_BRACKET, t, buffer);
      expression(t, buffer);
      match(CLOSE_BRACKET, t, buffer);
      return 1;
    case ASSIGNOP:
      //ASSIGNOP -> e
      return 1;
    default:
      if(t->token_type != LEXERROR){

          printf("Error in VAR1 on line %i. Expecting [ OR ASSIGNOP but received %s.\n", line, t->lexeme);
          fprintf(outfile, "        SYNTAX ERROR: Expecting ASSIGNOP or OPEN_BRACKET and received %s.\n", t->lexeme);
        }
        while(t->token_type != ASSIGNOP && t->token_type != EOF){
            getnexttoken(t, buffer);
        }
        return 0;
    }
}

int type(struct token *t, char* buffer){
  printf("\n\n\n*****************IN TYPE*****************\n\n\n");
  switch(t->token_type){
    case ARRAY:
      //ARRAY: type -> array [ num .. num ] of stndtype
      //TO DO
      match(ARRAY, t, buffer);
      match(OPEN_BRACKET, t, buffer);
      if(t->token_type == INT){
        match(INT, t, buffer);
        match(DOTDOT, t, buffer);
        match(INT, t, buffer);
        match(CLOSE_BRACKET, t, buffer);
        match(OF, t, buffer);
        standard_type(t, buffer);
      }else if(t->token_type == REAL){
        match(REAL, t, buffer);
        match(DOTDOT, t, buffer);
        match(REAL, t, buffer);
        match(CLOSE_BRACKET, t, buffer);
        match(OF, t, buffer);
        standard_type(t, buffer);
      }else{
        match(LONG_REAL, t, buffer);
        match(DOTDOT, t, buffer);
        match(LONG_REAL, t, buffer);
        match(CLOSE_BRACKET, t, buffer);
        match(OF, t, buffer);
        standard_type(t, buffer);
      }
      return 1;
    case 115:
      //INTEGER type -> stndtype
      //15 = integer
      standard_type(t, buffer);
      return 1;
    case 123:
      //REAL type -> stndtype
      //23 = real
      standard_type(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        fprintf(outfile, "        SYNTAX ERROR: Expecting INTEGER, REAL, or ARRAY and received %s.\n", t->lexeme);
      }
      while(t->token_type != SEMICOLON && t->token_type != CLOSE_PAREN && t->token_type != EOF){
          getnexttoken(t, buffer);
      }
      return 0;
  }
}

int expression(struct token *t, char* buffer){
  printf("\n\n\n*****************IN EXPRESSION*****************\n\n\n");
  switch(t->token_type){
    case IDENTIFIER:
      //id : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case OPEN_PAREN:
      //( : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case REAL:
      //num : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case LONG_REAL:
      //num : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case INT:
      //num : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case NOT: //not
      //not : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case PLUS:
      //plus : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    case MINUS:
      //minus : expression -> simple_expression expression'
      simple_expression(t, buffer);
      expression1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN EXPRESSION\n");
        fprintf(outfile, "        SYNTAX ERROR: Expecting MINUS, PLUS, NOT, INT, LONG_REAL, REAL, OPEN_PAREN, or ID and received %s.\n", t->lexeme);
      }
      while(t->token_type != CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type != THEN && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}
int expression1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN EXPRESSION1*****************\n\n\n");
  switch(t->token_type){
    case CLOSE_PAREN:
      // ) : exp’ -> ɛ
      return 1;
    case SEMICOLON:
      // ; : exp’ -> ɛ
      return 1;
    case COMMA:
      // , : exp’ -> ɛ
      return 1;
    case CLOSE_BRACKET:
      // ] : exp’ -> ɛ
      return 1;
    case END:
      return 1;
    case THEN:
      return 1;
    case ELSE:
      return 1;
    case DO:
      return 1;
      //end, then , else, do exp’ -> ɛ
    case RELOP:
      //relop : exp’ -> relop simpexp
      match(RELOP, t, buffer);
      simple_expression(t, buffer);
      return 1;
    // case AND:
    //   match(AND, t, buffer);
    //   simple_expression(t, buffer);
    //   return 1;
    // case OR:
    //   match(OR, t, buffer);
    //   simple_expression(t, buffer);
    //   return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("Error in VAR1 on line %i. Expecting stuff but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting RELOP, AND, DO, OR, ELSE, THEN, CLOSE_BRACKET, COMMA, SEMICOLON, or CLOSE_PAREN and received %s.\n", t->lexeme);
      }
      while(t->token_type != CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type != THEN && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int expression_list(struct token *t, char* buffer){
  printf("\n\n\n*****************IN EXPRESSION_LIST*****************\n\n\n");
  switch(t->token_type){
    case IDENTIFIER:
    //ID : expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case OPEN_PAREN:
      //(: expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case REAL:
      //real:expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case LONG_REAL:
      //long_real expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case INT:
      //int : expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case NOT: // not
      //not : expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case PLUS:
        //plus : expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    case MINUS:
      //minus: expression_list -> expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN EXPRESSION_LIST\n");
        fprintf(outfile, "        SYNTAX ERROR: Expecting MINUS, PLUS, NOT, INT, LONG_REAL, REAL, OPEN_PAREN, or IDENTIFIER and received %s.\n", t->lexeme);
      }
      while(t->token_type != CLOSE_PAREN && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}
int expression_list1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN EXPRESSION_LIST1*****************\n\n\n");
  switch(t->token_type){
    case CLOSE_PAREN:
      // CLOSE_PAREN : expression_list' -> e
      return 1;
    case COMMA:
      // , : expression_list' -> , expression expression_list'
      expression(t, buffer);
      expression_list1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN EXPRESSION_LIST1\n");
        fprintf(outfile, "        SYNTAX ERROR: Expecting CLOSE_PAREN or COMMA and received %s.\n", t->lexeme);
      }
      while(t->token_type != EOF && t->token_type != CLOSE_PAREN){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int simple_expression(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SIMPLE_EXPRESSION*****************\n\n\n");
  switch(t->token_type){
    case IDENTIFIER:
    // id : simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case OPEN_PAREN:
    //(: simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case REAL:
    //num: simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case LONG_REAL:
    //num: simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case INT:
    //num: simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case NOT: //NOT
    //not: simple_expression -> term simple_expression'
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case PLUS:
    //plus: simple_expression -> sign term simple_expression'
      sign(t, buffer);
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case MINUS:
    //minus: simple_expression -> sign term simple_expression'
      sign(t, buffer);
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN SIMPLE_EXPRESSION on line %i. Expecting PLUS, MINUS, NOT INT, LONG_REAL, REAL, OPEN_PAREN OR IDENTIFIER and recieved %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting PLUS, MINUS, NOT INT, LONG_REAL, REAL, OPEN_PAREN OR IDENTIFIER and recieved %s.\n", t->lexeme);
      }
      while(t->token_type != RELOP && t->token_type != OR && t->token_type != AND && t->token_type != CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type != THEN && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}
int simple_expression1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SIMPLE_EXPRESSION1*****************\n\n\n");
  switch(t->token_type){
    case ADDOP:
    //ADDOP simple_expression' -> addop term simple_expression'
      match(ADDOP, t, buffer);
      term(t, buffer);
      simple_expression1(t, buffer);
      return 1;
    case RELOP:
    //RELOP simple_expression' -> e
      return 1;
    // case AND:
    //   return 1;
    // case OR:
    //   return 1;
    case ELSE: //else
    //ELSE : simple_expression' -> e
      return 1;
    case DO: //do
    //DO : simple_expression' -> e
      return 1;
    case END: //end
    //END: simple_expression' -> e
      return 1;
    case COMMA:
    //COMMA: simple_expression' -> e
      return 1;
    case CLOSE_BRACKET:
    //CLOSE_BRACKET: simple_expression' -> e
      return 1;
    case SEMICOLON:
    //SEMICOLON: simple_expression' -> e
      return 1;
    case CLOSE_PAREN:
    //CLOSE_PAREN: simple_expression' -> e
      return 1;
    case THEN: //then
    //THEN simple_expression' -> e
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN Simple_expression1\n");
        fprintf(outfile, "        SYNTAX ERROR: Expecting THEN, CLOSE_BRACKET, COMMA, END, ELSE, DO, SEMICOLON or CLOSE_PAREN but received %s.\n", t->lexeme);
      }
      while(t->token_type != RELOP && t->token_type != AND && t->token_type != OR && t->token_type != CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type != THEN && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int sign(struct token *t, char* buffer){
  printf("\n\n\n*****************IN SIGN*****************\n\n\n");
  switch(t->token_type){
    case PLUS:
      match(PLUS, t, buffer);
      return 1;
    case MINUS:
      match(MINUS, t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("Error in PARAMETER_LIST1 on line %i. Expecting SEMICOLON or CLOSE_PAREN but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting SEMICOLON or CLOSE_PAREN but received %s.\n", t->lexeme);
      }
      while(t->token_type != IDENTIFIER && t->token_type != REAL && t->token_type != LONG_REAL && t->token_type != INT && t->token_type != OPEN_PAREN && t->token_type != NOT && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int term(struct token *t, char* buffer){
  printf("\n\n\n*****************IN TERM*****************\n\n\n");
      printf("lexeme = %s\n", t->lexeme);
  switch(t->token_type){

    case IDENTIFIER:
    //ID: term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case OPEN_PAREN:
    //term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case REAL:
    //term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case LONG_REAL:
    //term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case INT:
    //term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case 14://not
    //term -> factor term'
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    default:
      if(t->token_type != LEXERROR){
        printf("SYNTAX ERROR IN TERM on line %i. Expecting number, not, ID or ( and received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting number, not, ID or ( and received %s.\n", t->lexeme);
      }
      while(t->token_type != RELOP  && t->token_type != OR && t->token_type != AND &&t->token_type != CLOSE_PAREN && t->token_type != CLOSE_BRACKET && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != THEN && t->token_type != ADDOP && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
    }
}
int term1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN TERM1*****************\n\n\n");
  switch(t->token_type){
    case CLOSE_PAREN:
      return 1;
    case SEMICOLON:
      return 1;
    case COMMA:
      return 1;
    case CLOSE_BRACKET:
      return 1;
    case THEN:
      return 1;
    case ELSE:
      return 1;
    case DO:
      return 1;
    case RELOP:
      return 1;
    // case AND:
    //   return 1;
    // case OR:
    //   return 1;
    case ADDOP:
      return 1;
    case MULOP:
      match(MULOP, t, buffer);
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    // case MOD:
    //   printf("token type of mod = %i. attribute = %i\n", t->token_type, t->i_attribute);
    //   match(MOD, t, buffer);
    //   factor(t, buffer);
    //   term1(t, buffer);
    //   return 1;
    case DIVISION:
      match(DIVISION, t, buffer);
      factor(t, buffer);
      term1(t, buffer);
      return 1;
    case END:
      return 1;
    // ) : epsilon
    // ; : epsilon
    // , : epsilon
    // ] : epsilon
    // then : epsilon
    // else : epsilon
    // do : epsilon
    // relop : epsilon
    // addop : epsilon
    // mulop : term’ -> mulop factor term’
    //relop & ) & ] & do & else & end & ; & , & then & addop
    default:
      if(t->token_type != LEXERROR){
        printf("Error in TERM1 on line %i. Expecting ) : , ] then else do relop addop or mulop but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting ) : , ] then else do relop addop or mulop but received %s.\n", t->lexeme);
      }
      while(t->token_type != RELOP  && t->token_type != AND && t->token_type != OR && t->token_type != CLOSE_PAREN && t->token_type != CLOSE_BRACKET && t->token_type != DO && t->token_type != ELSE && t->token_type != END && t->token_type != SEMICOLON && t->token_type != COMMA && t->token_type != THEN && t->token_type != ADDOP && t->token_type != EOF){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

int factor(struct token *t, char* buffer){
  printf("\n\n\n*****************IN FACTOR*****************\n\n\n");
    switch(t->token_type){
      case OPEN_PAREN:
        //OPEN_PAREN: factor -> ( exp )
        match(OPEN_PAREN, t, buffer);
        expression(t, buffer);
        match(CLOSE_PAREN, t, buffer);
        return 1;
      case IDENTIFIER:
        //ID: factor -> id factor'
        match(IDENTIFIER, t, buffer);
        factor1(t, buffer);
        return 1;
      case REAL:
        //NUM: factor -> num factor
        match(REAL, t, buffer);
        //factor(t, buffer);
        return 1;
      case LONG_REAL:
        //NUM: factor -> num factor
        match(LONG_REAL, t, buffer);
        //factor(t, buffer);
        return 1;
      case INT:
        //NUM: factor -> num factor
        match(INT, t, buffer);
        //factor(t, buffer);
        return 1;
      case NOT:
        //NOT factor -> not factor
        match(NOT, t, buffer);
        factor(t, buffer);
        return 1;
      default:
        if(t->token_type != LEXERROR){
          printf("Error in FACTOR on line %i. Expecting NOT, NUM, ID OR OPEN_PAREN but received %s.\n", line, t->lexeme);
          fprintf(outfile,"        SYNTAX ERROR: Expecting NOT, NUM, ID OR OPEN_PAREN but received %s.\n", t->lexeme);
        }
        while(t->token_type != MULOP && t->token_type != MOD && t->token_type != DIVISION && t->token_type != AND && t->token_type != OR && t->token_type != RELOP && t->token_type != AND && t->token_type != OR && t->token_type != ADDOP && t->token_type !=  CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type !=  DO && t->token_type !=  ELSE && t->token_type !=  END && t->token_type !=  SEMICOLON && t->token_type !=  COMMA && t->token_type != THEN){
          getnexttoken(t, buffer);
        }
        return 0;
    }
}
int factor1(struct token *t, char* buffer){
  printf("\n\n\n*****************IN FACTOR1*****************\n\n\n");
    switch(t->token_type){
    // OPEN_PAREN: factor' -> ( explst )
    case OPEN_PAREN:
      match(OPEN_PAREN, t, buffer);
      expression_list(t, buffer);
      match(CLOSE_PAREN, t, buffer);
      return 1;
    case OPEN_BRACKET:
      match(OPEN_BRACKET, t, buffer);
      expression(t, buffer);
      match(CLOSE_BRACKET, t, buffer);
      return 1;
    case ADDOP:
    //ADDOP : epsilon
      return 1;
    case MULOP:
    //MULOP : epsilon
      return 1;
    // case MOD:
    //   return 1;
    case DIVISION:
      return 1;
    case NOT:
    //NOT : epsilon
      return 1;
    case RELOP:
      return 1;
    // case AND:
    //   return 1;
    // case OR:
    //   return 1;
    case CLOSE_PAREN:
      return 1;
    case CLOSE_BRACKET:
      return 1;
    case DO:
      return 1;
  //mulop & relop & addop & ) & ] & do & else & end & ; & , & then
    case ELSE:
      return 1;
    case THEN:
      return 1;
    case END:
      return 1;
    case SEMICOLON:
      return 1;
    case COMMA:
      return 1;
    default:
      if(t->token_type != LEXERROR){

        printf("Error in FACTOR1 on line %i. Expecting OPEN_PAREN, ADDOP, MULOP, NOT but received %s.\n", line, t->lexeme);
        fprintf(outfile, "        SYNTAX ERROR: Expecting OPEN_PAREN, ADDOP, MULOP, NOT but received %s.\n", t->lexeme);
      }
      while(t->token_type != MULOP && t->token_type != DIVISION && t->token_type != MOD && t->token_type != RELOP && t->token_type != AND && t->token_type != OR &&t->token_type != ADDOP && t->token_type !=  CLOSE_PAREN && t->token_type !=  CLOSE_BRACKET && t->token_type !=  DO && t->token_type !=  ELSE && t->token_type !=  END && t->token_type !=  SEMICOLON && t->token_type !=  COMMA && t->token_type != THEN){
        getnexttoken(t, buffer);
      }
      return 0;
  }
}

char* convert_define(int integer){
  if(integer == DOT){
    return ("PERIOD");
  }
  if(integer == OPEN_PAREN){
    return ("OPEN_PAREN");
  }
  if(integer == CLOSE_PAREN){
    return ("CLOSE_PAREN");
  }
  if(integer == OPEN_BRACKET){
    return ("OPEN_BRACKET");
  }
  if(integer ==   COLON){
    return ("COLON");
  }
  if(integer == CLOSE_BRACKET){
    return ("CLOSE_BRACKET");
  }
  if(integer == DOUBLE_PERIOD){
    return ("..");
  }
  if(integer == OPEN_S_BRACKET){
    return ("{");
  }
  if(integer == COMMENT_OPEN){
    return ("(*");
  }
  if(integer == CLOSE_S_BRACKET){
    return ("}");
  }
  if(integer == COMMENT_CLOSE){
    return ("*)");
  }
  if(integer ==   COMMA){
    return (",");
  }
  if(integer == SEMICOLON){
    return (";");
  }
  if(integer == STAR){
    return ("*");
  }
  if(integer == SLASH){
    return ("/");
  }
  if(integer == MINUS){
    return ("-");
  }
  if(integer == PLUS){
    return ("+");
  }
  if(integer == IDENTIFIER){
    return ("IDENTIFIER");
  }
  if(integer == INT){
    return ("INTEGER");
  }
  if(integer == REAL){
    return ("REAL");
  }
  if(integer ==   LONG_REAL){
    return ("LONG_REAL");
  }
  if(integer == RELOP){
    return ("RELOP");
  }
  if(integer == ADDOP){
    return ("ADDOP");
  }
  if(integer == MULOP){
    return ("MULOP");
  }
  if(integer == ASSIGNOP){
    return ("ASSIGNOP");
  }
  if(integer == LEXERROR){
    return ("LEXERROR");
  }
  if(integer == 110){
    return ("DO");
  }else if(integer == EOF){
    return ("EOF");
  }else{
    return ("token");
  }
}
