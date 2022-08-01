/*parser.h
  Definitions for parser.c.
  Copyright (c) 1998 by Matthew Belmonte.*/

typedef struct _Tree {
  Token oper;
  union {
    int value;
    struct {struct _Tree *left, *right;} branches;
    } u;
  } Tree;

/*data abstractors for trees*/
#define TreeOper(t) ((t)->oper)
#define TreeValue(t) ((t)->u.value)
#define TreeLeft(t) ((t)->u.branches.left)
#define TreeRight(t) ((t)->u.branches.right)
#define NULLTREE ((Tree *)0)

extern Tree *parse_expr();
extern void print_tree(Tree*);
