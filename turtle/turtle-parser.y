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
%right '^'
%left NEGATIVE

%token <value>    VALUE       "value"
%token <name>     NAME        "name"
%token <name>     COLOR       "color"


/*Movement Tokens*/
%token            KW_CMD_FORWARD  
%token            KW_CMD_BACKWARD 
%token            KW_CMD_LEFT     
%token            KW_CMD_RIGHT    
%token            KW_CMD_HEADING
%token            KW_CMD_POSITION

/*Color Tokens*/
%token            KW_CMD_COLOR

/*Control Tokens*/
%token            KW_REPEAT
%token            KW_PROC
%token            KW_SET
%token            KW_CALL

/*Misc Tokens*/
%token            KW_CMD_HOME
%token            KW_CMD_PRINT
%token            KW_CMD_DOWN
%token            KW_CMD_UP

/*Function Tokens*/
%token            KW_FUNC_SIN
%token            KW_FUNC_COS
%token            KW_FUNC_TAN
%token            KW_FUNC_SQRT
%token            KW_FUNC_RANDOM

/*Constants token*/
%token            KW_CONST_PI
%token            KW_CONST_SQRT2
%token            KW_CONST_SQRT3

%type <node> unit cmds cmd expr


%%

unit:
  //Our axiom is a list of commands that will be executed
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
  //A list of commands can be a command followed by a list of commands or empty
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; }
;

cmd:
  //A command can be a movement command, a color command, a control command, a print command, a set command, a call command, a block command or a repeat command
     KW_CMD_FORWARD expr                     { $$ = make_cmd_forward($2); }
  |  KW_CMD_BACKWARD expr                    { $$ = make_cmd_backward($2); }
  |  KW_CMD_LEFT expr                        { $$ = make_cmd_left($2); }
  |  KW_CMD_RIGHT expr                       { $$ = make_cmd_right($2); }
  //A color command can be a color name or an RGB color value, we use the make_cmd_color_rgb function to create the command or the make_cmd_color function to keep the color name
  |  KW_CMD_COLOR expr ',' expr ',' expr     { $$ = make_cmd_color_rgb($2,$4,$6); }
  |  KW_CMD_COLOR expr                       { $$ = make_cmd_color($2); }
  |  KW_CMD_HEADING expr                     { $$ = make_cmd_heading($2); }
  |  KW_CMD_HOME                             { $$ = make_cmd_home(); }
  |  KW_CMD_DOWN                             { $$ = make_cmd_down(); }
  |  KW_CMD_UP                               { $$ = make_cmd_up(); }
  |  KW_CMD_POSITION expr ',' expr           { $$ = make_cmd_position($2, $4); }
  |  KW_REPEAT expr cmd                      { $$ = make_cmd_repeat($2,$3); }
    //A procedure command is a command that defines a procedure with a name and a block of commands that will be executed when the procedure is called
    //It is stocked in the procedure hashmap of the turtle context and can be called by a call command
  |  KW_PROC NAME cmd                        { $$ = make_cmd_proc($2, $3); }
    //A set command is a command that sets a variable with a value

  |  KW_SET NAME expr                        { $$ = make_cmd_set($2, $3); }
  |  KW_CALL NAME                            { $$ = make_cmd_call($2); }
  //A block command is a command that contains a block of commands
  |  '{' cmds '}'                            { $$ = make_cmd_block($2); }
  |  KW_CMD_PRINT expr                       { $$ = make_cmd_print($2); }


;

expr:
  //An expression can be a value, a color, a name, a negative expression, a parenthesized expression, a binary operation, a function call or a constant
  VALUE                                      { $$ = make_expr_value($1); }
  //COLOR is a token that represents a color name
  | COLOR                                    { $$ = make_expr_color($1); }
  | NAME                                     { $$ = make_expr_name($1); }
  //Here we use the NEGATIVE token to represent a negative expression and give it a higher precedence than the other binary operations
  |  '-' expr %prec NEGATIVE                 { $$ = make_expr_neg($2); }
  |  '(' expr ')'                            { $$ = $2; }
  |  expr '+' expr                           { $$ = make_expr_binop('+',$1, $3); }
  |  expr '-' expr                           { $$ = make_expr_binop('-',$1, $3); }
  |  expr '*' expr                           { $$ = make_expr_binop('*',$1, $3); }
  |  expr '/' expr                           { $$ = make_expr_binop('/',$1, $3); }
  |  expr '^' expr                           { $$ = make_expr_binop('^',$1, $3); }
  |  KW_FUNC_COS '(' expr ')'                { $$ = make_expr_func_cos($3); }
  |  KW_FUNC_SIN '(' expr ')'                { $$ = make_expr_func_sin($3); }
  |  KW_FUNC_TAN '(' expr ')'                { $$ = make_expr_func_tan($3); }
  |  KW_FUNC_SQRT '(' expr ')'               { $$ = make_expr_func_sqrt($3); }
  |  KW_FUNC_RANDOM '(' expr ',' expr ')'    { $$ = make_expr_func_random($3,$5); }
  |  KW_CONST_PI                             { $$ = make_expr_name("PI"); }
  |  KW_CONST_SQRT2                          { $$ = make_expr_name("SQRT2"); }
  |  KW_CONST_SQRT3                          { $$ = make_expr_name("SQRT3"); }

;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
