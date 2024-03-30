%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

NUMBER (?:(?:0\.|[1-9]+[0-9]*\.?)[0-9]*(?:[eE][+-]?[1-9]+[0-9]*)?|0)

OP_MIN  \-
OP_ADD \+
OP_MULT \*
OP_DIV \/
OP_POW \^

KW_CMD_COLOR color
KW_CMD_FORWARD fw|forward
KW_CMD_BACKWARD bw|backward
KW_CMD_LEFT lt|left
KW_CMD_RIGHT rt|right
KW_CMD_HEADING hd|heading
KW_CMD_POSITION pos|position
KW_CMD_UP up
KW_CMD_DOWN down
KW_CMD_HOME home
KW_CMD_PRINT print
KW_REPEAT repeat
KW_SET set
KW_PROC proc
KW_CALL call

KW_FUNC_SIN sin
KW_FUNC_COS cos
KW_FUNC_TAN tan
KW_FUNC_SQRT sqrt
KW_FUNC_RANDOM random

KW_CONST_PI PI
KW_CONST_SQRT2 SQRT2
KW_CONST_SQRT3 SQRT3

COLORS red|green|blue|cyan|magenta|yellow|black|grey|white
COMMENT \#.*$
VAR_NAME [A-Z][A-Z0-9]*


%%
{NUMBER}                       { yylval.value = strtod(yytext, NULL); return VALUE; }
{VAR_NAME}                     { yylval.name = strdup(yytext); return NAME; }


{KW_CMD_COLOR}                 { return KW_CMD_COLOR; }
{COLORS}                       { yylval.name = strdup(yytext) ; return COLOR; }
{KW_CMD_FORWARD}               { return KW_CMD_FORWARD; }
{KW_CMD_BACKWARD}              { return KW_CMD_BACKWARD; }
{KW_CMD_HEADING}               { return KW_CMD_HEADING; }
{KW_CMD_POSITION}              { return KW_CMD_POSITION; }
{KW_CMD_HOME}                  { return KW_CMD_HOME; }
{KW_CMD_PRINT}                 { return KW_CMD_PRINT; }
{KW_CMD_LEFT}                  { return KW_CMD_LEFT; }
{KW_CMD_RIGHT}                 { return KW_CMD_RIGHT; }
{KW_CMD_UP}                    { return KW_CMD_UP; }
{KW_CMD_DOWN}                  { return KW_CMD_DOWN; }

{KW_FUNC_SIN}                  { return KW_FUNC_SIN; }
{KW_FUNC_COS}                  { return KW_FUNC_COS; }
{KW_FUNC_TAN}                  { return KW_FUNC_TAN; }
{KW_FUNC_SQRT}                 { return KW_FUNC_SQRT; }
{KW_FUNC_RANDOM}               { return KW_FUNC_RANDOM; }

{KW_REPEAT}                    { return KW_REPEAT; }
{KW_SET}                       { return KW_SET; }
{KW_PROC}                      { return KW_PROC; }
{KW_CALL}                      { return KW_CALL; }



","                            { return ','; }
{OP_ADD}                       { return '+'; }
{OP_MULT}                      { return '*'; }
{OP_DIV}                       { return '/'; }
{OP_POW}                       { return '^'; }
{OP_MIN}                       { return '-'; }
"("                            { return '('; }
")"                            { return ')'; }
"{"                            { return '{'; }
"}"                            { return '}'; }

{COMMENT}                      /* ignore comments */


[\n\t ]*                       /* whitespace */
.                              { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
