%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

NUMBER (?:-?(?:0\.|[1-9]+[0-9]*\.?)[0-9]*(?:[eE][+-]?[1-9]+[0-9]*)?|0)

OP_MIN  \-
OP_ADD \+
OP_MULT \*
OP_DIV \/
OP_POW \^

KW_COLOR color
KW_FORWARD fw|forward
KW_BACKWARD bw|backward
KW_LEFT lt|left
KW_RIGHT rt|right
KW_HEADING hd|heading
KW_POSITION pos|position
KW_HOME home
KW_PRINT print
KW_REPEAT repeat
KW_SET set
KW_PROC proc
KW_CALL call

FUNC_SIN sin
FUNC_COS cos
FUNC_TAN tan
FUNC_SQRT sqrt
FUNC_RANDOM random

COLORS red|green|blue|cyan|magenta|yellow|black|grey|white
COMMENT \#.*$
VAR_NAME [A-Z][A-Z0-9]*
PRINT_MSG [A-Za-z0-9]*

%%
{NUMBER}                 { yylval.value = strtod(yytext, NULL); return VALUE; }
{VAR_NAME}               { yylval.name = strdup(yytext); return NAME; }

{KW_COLOR}               { return KW_COLOR;}
{COLORS}                 { yylval.name = strdup(yytext) ; return COLOR; }
{KW_FORWARD}             { return KW_FORWARD;}
{KW_BACKWARD}            { return KW_BACKWARD;}
{KW_HEADING}             { return KW_HEADING;}
{KW_POSITION}            { return KW_POSITION;}
{KW_HOME}                { return KW_HOME;}
{KW_PRINT}               { return KW_PRINT;}
{KW_LEFT}                { return KW_LEFT;}
{KW_RIGHT}               { return KW_RIGHT;}
{KW_REPEAT}              { return KW_REPEAT;}
{KW_SET}                 { return KW_SET;}
{KW_PROC}                { return KW_PROC;}
{KW_CALL}                { return KW_CALL;}



","                     { return ','; }
{OP_ADD}                { return '+'; }
{OP_MULT}               { return '*'; }
{OP_DIV}                { return '/'; }
{OP_POW}                { return '^'; }
{OP_MIN}                { return '-'; }
"("                    { return '('; }
")"                    { return ')'; }
"{"                    { return '{'; }
"}"                    { return '}'; }


{PRINT_MSG}              { yylval.name = strdup(yytext); printf("%s\n",yytext);return PRINT_MSG; }
{COMMENT}               /* ignore comments */


[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
