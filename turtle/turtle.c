#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "turtle-ast.h"
#include "turtle-lexer.h"
#include "turtle-parser.h"

int main() {
  srand(time(NULL));

  struct ast root;
  int ret = yyparse(&root);
  if (ret != 0) {
    return ret;
  }
  
  yylex_destroy();
  
  assert(root.unit);
  
  struct context ctx;
  context_create(&ctx);
  
  printf("----------------\n"); 
  ast_eval(&root, &ctx);
  printf("----------------\n"); 
  ast_print(&root);

  ast_destroy(&root);
  context_destroy(&ctx);

  return ret;
}
