%{
#include <stdio.h>

#include "turtle-ast.h"

int yylex();
void yyerror(struct ast *ret, const char *);

%}

%debug
%defines

%define parse.error verbose

%parse-param { struct ast *ret }

%union {
  double value;
  char *name;
  struct ast_node *node;
}

%left '+' '-'
%left '*' '/'


%token <value>    VALUE       "value"
%token <name>     NAME        "name"

/*Movement Tokens*/
%token            KW_FORWARD  "forward"
%token            KW_BACKWARD "backward"
%token            KW_LEFT     "left"
%token            KW_RIGHT    "right"

/*Color Tokens*/
%token            KW_COLOR    "color"
/* TODO: add other tokens */

%type <node> unit cmds cmd expr


%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; }
;

cmd:
    KW_FORWARD expr    { $$ = make_cmd_forward($2); }
  |  KW_BACKWARD expr  { $$ = make_cmd_backward($2); }
  |  KW_LEFT expr      { $$ = make_cmd_left($2); }
  |  KW_RIGHT expr     { $$ = make_cmd_right($2); }
  |  KW_COLOR expr     { /* TODO */ }

;

expr:
    VALUE              { $$ = make_expr_value($1); }
  |  expr '+' expr     { $$ = make_expr_mul($1, $3); }
  |  expr '-' expr     { $$ = make_expr_sub($1, $3); }
  |  expr '*' expr     { $$ = make_expr_mul($1, $3); }
  |  expr '/' expr     { $$ = make_expr_div($1, $3); }
  |  '(' expr ')'      { $$ = $2; }
  |  '-' expr          { $$ = make_expr_neg($2); }

;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
