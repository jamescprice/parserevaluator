/*testparse.c
  Test routine for parser.c.
  Copyright (c) 1998 by Matthew Belmonte.*/

#include "scanner.h"
#include "parser.h"

int main()
  {
  Tree *p;
  p = parse_expr();
  print_tree(p);
  }
