/*parser.c
  This recursive-descent parser for arithmetic expressions is part of the term
  project for the MIT SIPB Crash Course in C for IAP 1999.  Copyright (c) 1998
  Matthew Belmonte.*/

/*There is, in general, one parsing function for each nonterminal symbol in the
  grammar.  Refer to the grammar (or syntax chart) to understand how these
  routines interact with each other.  Some regions of the code are commented
  very lightly, because their structure and function follow from the grammar.
 -There are two important and related invariant conditions over all these
  parsing functions.  On entry, the first token in the phrase about to be
  parsed is in 'token'.  On exit, the token immediately following the
  last token in the expression just parsed is in 'token' (except in the
  case of parse_program, where, if all is well, there is nothing following the
  '.').  To maintain these invariants, it is necessary to keep track of tokens
  that delimit expressions and to eat them where appropriate.
 -In other words, each parsing function assumes that the input stream of tokens
  has been prepared for it, and it in turn prepares the input stream for
  whatever parsing function may be called next.*/

#include "scanner.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
//#include <errno.h>

/*Build an internal tree node, which contains an operator and points to two
  subtrees.*/
static Tree *NewTree(Token op, Tree * left,Tree * right)
//Token op;
//Tree *left, *right;
  {
  Tree *new_tree;
  new_tree = (Tree *)malloc(sizeof(*new_tree));
  if(new_tree == (Tree *)0)		/*out of memory?*/
    {
//    perror((char *)0);
//    exit(errno);
      exit(1);
    }
  TreeOper(new_tree) = op;
  TreeLeft(new_tree) = left;
  TreeRight(new_tree) = right;
  return(new_tree);
  }

/*Build a new leaf, which contains an operator of NumberToken, and a number.*/
static Tree *NewLeaf(int value)
//int value;
  {
  Tree *new_tree;
  new_tree = (Tree *)malloc(sizeof(*new_tree));
  if(new_tree == (Tree *)0)		/*out of memory?*/
    {
//    perror((char *)0);
//   exit(errno);
      exit(1);

    }
  TreeOper(new_tree) = NumberToken;
  TreeValue(new_tree) = number;
  }

static Tree *parse_leaf()
  {
  Tree *ptr;
  ptr = NewLeaf(number);
  next_token();
  return(ptr);
  }

/*Parse a list of expressions, for example factors separated by multiplication
  and division operators or terms separated by addition and subtraction
  operators.  'parsing_fn' is the parsing function for the expressions that
  comprise the list, and 'op0' and 'op1' are the operators that separate those
  expressions within the list.*/
static Tree *parse_expressions(Tree *(*parsing_fn)(),Token op0,Token op1)
//Tree *(*parsing_fn)();
//Token op0, op1;
  {
  Tree *left_tree, *tree;
  left_tree = (*parsing_fn)();
/*invariant: 'left_tree' points to the root of a parse tree that represents all
  the tokens in the expression that are to the left of 'token'.*/
/*bound: the number of expressions that remain to be parsed*/
  while((token == op0) || (token == op1))
    {
    tree = NewTree(token, left_tree, NULLTREE);
    next_token();
    TreeRight(tree) = (*parsing_fn)();
    left_tree = tree;
    }
  return(left_tree);
  }

static Tree *parse_arith_expr();

static Tree *parse_factor()
  {
  Tree *factor;
  if(token == LeftParenToken)
    {
    next_token();			/*eat the '('*/
    factor = parse_arith_expr();
    if(token != RightParenToken)
      {
      printf("expected \")\" in factor, found ");
      print_token(token);
      printf("\n");
      exit(1);
      }
    next_token();			/*eat the ')'*/
    return(factor);
    }
  else if(token == NumberToken)
    return(parse_leaf());
  else
    {
    printf("unexpected factor ");
    print_token(token);
    printf("\n");
    exit(1);
    }
  }

static Tree *parse_term()
  {
  return(parse_expressions(&parse_factor, TimesToken, DivideToken));
  }

static Tree *parse_arith_expr()
  {
  return(parse_expressions(&parse_term, PlusToken, MinusToken));
  }

/*This is the top-level parsing function.*/
Tree *parse_expr()
  {
  Tree *expr;
  init_scanner();
  next_token();
  expr = parse_arith_expr();
  if(token != PeriodToken)
    {
    printf("unexpected token at end of input: ");
    print_token(token);
    printf("\n");
    exit(1);
    }
  return(expr);
  }

/*print an indented subtree - used by print_tree() below*/
static void print_subtree(Tree * t,int indent)
//Tree *t;
//int indent;
  {
//  register int spaces;
int spaces;

/*invariant: all nodes to the left of 't' have been printed*/
  while(TreeOper(t) != NumberToken)
    {
    for(spaces = 0; spaces != indent; spaces++)
      putchar(' ');
    print_token(TreeOper(t));
    putchar('\n');
    print_subtree(TreeLeft(t), 2+indent);
    indent += 2;
    t = TreeRight(t);
    }
  for(spaces = 0; spaces != indent; spaces++)
    putchar(' ');
  printf("%d\n", TreeValue(t));
  }

/*print the contents of a tree - useful for debugging*/
void print_tree(Tree * t)
//Tree *t;
  {
  print_subtree(t, 0);
  }
