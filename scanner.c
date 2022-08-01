/*scanner.c
  This lexical scanner for arithmetic expressions is part of the term project
  for the MIT SIPB Crash Course in C for IAP 1999.  Copyright (c) 1998 Matthew
  Belmonte.*/

#include "scanner.h"
//#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int is_space( int c) {
        /*
         * Depends on ASCII-like character ordering.
         */
        return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int isdigit( int c) {
        /*
         * Depends on ASCII-like character ordering.
         */
        return ((c >= 48  && c <= 57) ? 1 : 0);
}

static char current_char;		/*next character to be scanned*/
Token token;				/*the token most recently scanned*/
int number;				/*if token==NumberToken, here's the #*/

static char next_char()			/*all input comes through here*/
  {
  current_char = getchar();
  }

/*sets the token type and gets the next character*/
static void make_token(Token t)
//Token t;
  {
  next_char();
  token = t;
  }

/*On entry, 'current_char' is a digit.  Set the global variable 'number' to
  the integer represented by the string of digits, and set the global
  variable 'token' to NumberToken.*/
static void string_to_number()
  {
  token = NumberToken;
  number = 0;
/*invariant: The portion of the digit string that is to the left of the
  position of 'current_char' is the base-ten representation of 'number'.
  (For ease of specification, define the null string as a valid base-ten
  representation of the integer 0.)*/
/*In other, less declarative words, 'number' is accumulating the
  digits by place value.*/
/*bound: the number of digits that remain to be processed*/
  do
    {
    number = 10*number + (current_char - '0');
    next_char();
    } while(isdigit(current_char));
  }

/*initialise the scanner by loading the first character*/
void init_scanner()
  {
  next_char();
  }

/*top-level scanner function*/
void next_token()
  {
  while(is_space(current_char))
    next_char();			/*skip blanks*/
  if(isdigit(current_char))
    string_to_number();
  else switch(current_char)
    {
    case '+': make_token(PlusToken); break;
    case '-': make_token(MinusToken); break;
    case '*': make_token(TimesToken); break;
    case '/': make_token(DivideToken); break;
    case '(': make_token(LeftParenToken); break;
    case ')': make_token(RightParenToken); break;
    case '.': token = PeriodToken; break; /*don't wait for more input*/
    default: printf("ILLEGAL CHAR %c (HEX %x)\n", current_char, current_char);
    }
  }

/*print the type and value of the token - useful in debugging*/
void print_token(Token t)
//Token t;
  {
  switch(t)
    {
    case NumberToken: printf("%d", number); break;
    case PlusToken: putchar('+'); break;
    case MinusToken: putchar('-'); break;
    case TimesToken: putchar('*'); break;
    case DivideToken: putchar('/'); break;
    case LeftParenToken: putchar('('); break;
    case RightParenToken: putchar(')'); break;
    case PeriodToken: putchar('.'); break;
    default: printf("undefined token %d", (int)t);
    }
  }
