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
%left '^'



%token <value>    VALUE       "value"
%token <name>     NAME        "name"
%token <name>     COLOR       "color"
%token <name>     PRINT_MSG   "print_msg"

/*Movement Tokens*/
%token            KW_FORWARD  
%token            KW_BACKWARD 
%token            KW_LEFT     
%token            KW_RIGHT    
%token            KW_HEADING
%token            KW_POSITION

/*Color Tokens*/
%token            KW_COLOR    


%token            KW_REPEAT
%token            KW_HOME
%token            KW_PRINT

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
     KW_FORWARD expr                    { $$ = make_cmd_forward($2);      }
  |  KW_BACKWARD expr                   { $$ = make_cmd_backward($2);     }
  |  KW_LEFT expr                       { $$ = make_cmd_left($2);         }
  |  KW_RIGHT expr                      { $$ = make_cmd_right($2);        }
  |  KW_COLOR expr                      { $$ = make_cmd_color($2);        }
  |  KW_HEADING expr                    { $$ = make_cmd_heading($2);      }
  |  KW_HOME                            { $$ = make_cmd_home();           }
  |  KW_POSITION expr expr              { $$ = make_cmd_position($2, $3); }
  |  KW_PRINT PRINT_MSG                 { $$ = make_cmd_print($2);        }
  |  KW_COLOR expr ',' expr ',' expr    { $$ = make_cmd_color_rgb($2,$4,$6);}
  |  KW_REPEAT expr cmd                 { $$ = make_cmd_repeat($2,$3); }
  |  '{' cmds '}'                       { $$ = make_cmd_block($2); }
;

expr:
  VALUE                { $$ = make_expr_value($1);}
  | COLOR              { $$ = make_expr_color($1);}
  | NAME               { $$ = make_expr_name($1); }
  |  expr '+' expr     { $$ = make_expr_binop('+',$1, $3); }
  |  expr '-' expr     { $$ = make_expr_binop('-',$1, $3); }
  |  expr '*' expr     { $$ = make_expr_binop('*',$1, $3); }
  |  expr '/' expr     { $$ = make_expr_binop('/',$1, $3); }
  |  expr '^' expr     { $$ = make_expr_binop('^',$1, $3); }
  |  '(' expr ')'      { $$ = $2; }
  |  '-' expr          { $$ = make_expr_neg($2); }
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
