%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

NUMBER -?(?:(?:0|[1-9]+[0-9]*)?\.[0-9]*(?:[eE][+-]?)?[0-9]*)|(?:0|[1-9]+[0-9]*)

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


FUNC_SIN sin
FUNC_COS cos
FUNC_TAN tan
FUNC_SQRT sqrt
FUNC_RANDOM random

COLORS red|green|blue|cyan|magenta|yellow|black|grey|white
COMMENT \#.*$

%%

{NUMBER}                 { yylval.value = strtod(yytext, NULL); return VALUE; }
{KW_COLOR}               { return KW_COLOR;}
{COLORS}                 { yylval.name = strdup(yytext) ; return COLOR; }
{KW_FORWARD}             { return KW_FORWARD;}
{KW_BACKWARD}            { return KW_BACKWARD;}
{KW_LEFT}                { return KW_LEFT;}
{KW_RIGHT}               { return KW_RIGHT;}

{OP_ADD}                { return '+'; }
{OP_MULT}               { return '*'; }
{OP_DIV}                { return '/'; }
{OP_POW}                { return '^'; }
{OP_MIN}                { return '-'; }
'\('                    { return '('; }
'\)'                    { return ')'; }
'\{'                    { return '{'; }
'\}'                    { return '}'; }

{COMMENT}               /* ignore comments */


[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
