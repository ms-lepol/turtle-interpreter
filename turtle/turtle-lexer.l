%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

NUMBER (?:(?:0|[1-9]+[0-9]*)?\.[0-9]*(?:[eE][+-]?)?[0-9]*)|(?:0|[1-9]+[0-9]*)
ANGLE -?{NUMBER}

OP_MIN_UNI \-{NUMBER}
OP_MIN_BIN {NUMBER}\-{NUMBER}
OP_ADD {NUMBER}\+{NUMBER}
OP_MULT {NUMBER}\*{NUMBER}
OP_DIV {NUMBER}\/{NUMBER}
OP_POW \^{NUMBER}

KW_COLOR color
KW_FORWARD fw|forward
KW_LEFT left
KW_RIGHT right
KW_SIN sin
KW_COS cos
KW_TAN tan
KW_SQRT sqrt
KW_RANDOM random

COLORS red|green|blue|cyan|magenta|yellow|black|grey|white
PARENTHESIS_OPEN \(
PARENTHESIS_CLOSE \)
COMMENT_START #
COMMENT_END \n

%%

{NUMBER}              { yylval.value = strtod(yytext, NULL); printf("FLOAT %i\n", VALUE); return VALUE; }
{KW_COLOR}            { printf("KW_COLOR\n"); return KW_COLOR;}
{COLORS}              { printf("COLOR %c\n", yytext); return COLOR; }
{FORWARD}             { printf("FORWARD\n"); return KW_FORWARD;}

[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
