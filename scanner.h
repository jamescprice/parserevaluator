/*scanner.h
  Definitions for scaled-back version of scanner.c.
  Copyright (c) 1998 by Matthew Belmonte.*/

typedef enum {
    /* ATOMS */
  NumberToken,		/*a base-ten integer*/
    /* BINARY OPERATORS */
  PlusToken,		/* + */
  MinusToken,		/* - */
  TimesToken,		/* * */
  DivideToken,		/* / */
  LeftParenToken,	/* ( */
  RightParenToken,	/* ) */
  PeriodToken		/* . */
  } Token;

void init_scanner();
extern Token token;			/*the most recently scanned token*/
extern int number;			/*if token==NumberToken, here's the #*/
void next_token();			/*gets next token from input*/
void print_token(Token);		/*prints contents of node*/
