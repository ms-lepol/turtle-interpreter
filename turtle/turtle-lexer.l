%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

NUMBER -?(?:(?:0|[1-9]+[0-9]*)?\.[0-9]*(?:[eE][+-]?)?[0-9]*)|(?:0|[1-9]+[0-9]*)

OP_MIN_UNI \-{NUMBER}
OP_MIN_BIN {NUMBER}\-{NUMBER}
OP_ADD {NUMBER}\+{NUMBER}
OP_MULT {NUMBER}\*{NUMBER}
OP_DIV {NUMBER}\/{NUMBER}
OP_POW \^{NUMBER}

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
PARENTHESIS_OPEN \(
PARENTHESIS_CLOSE \)
COMMENT \#.*$

%%

{NUMBER}                 { yylval.value = strtod(yytext, NULL); printf("FLOAT %s\n", yytext); return VALUE; }
{KW_COLOR}               { printf("KW_COLOR\n"); return KW_COLOR;}
{COLORS}                 { yylval.name = strdup(yytext) ; printf("COLOR %s\n", yytext); return COLORS; }
{KW_FORWARD}             { printf("FORWARD\n"); return KW_FORWARD;}
{KW_BACKWARD}            { printf("BACKWARD\n"); return KW_BACKWARD;}
{KW_LEFT}                { printf("LEFT\n"); return KW_LEFT;}
{KW_RIGHT}               { printf("RIGHT\n"); return KW_RIGHT;}

{OP_ADD}                { printf("ADD\n"); return '+'; }
{OP_MIN_BIN}            { printf("MIN\n"); return '-'; }
{OP_MULT}               { printf("MULT\n"); return '*'; }
{OP_DIV}                { printf("DIV\n"); return '/'; }
{OP_POW}                { printf("POW\n"); return '^'; }
{OP_MIN_UNI}            { printf("MIN_UNI\n"); return '-'; }

{COMMENT}               /* ignore comments */


[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
