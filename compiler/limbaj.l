%{
#include <stdio.h>
#include "limbaj.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string"|"bool" {yylval.string = strdup(yytext);return TYPE;}
"begin_progr" {return BGIN;}
"end_progr" {return END;}
"begin_class" {return CLASS;}
"end_class" {return ENDCLASS;}
"begin_func" {return FUNC;}
"end_func" {return ENDFUNC;}
"return" {return RETURN;}
"const" {yylval.string = strdup(yytext);return CONST;}
"if" {return IF;}
"else" {return ELSE;}
"eif" {return EIF;}
"for" {return FOR;}
"efor" {return EFOR;}
"do" {return DO;}
"while" {return WHILE;}
"ewhile" {return EWHILE;}
"Eval" {return EVAL;}
"TypeOf" {return TYPEOF;}
"<=" {yylval.string = strdup(yytext);return LEQ;}
">=" {yylval.string = strdup(yytext);return GEQ;}
"!" {yylval.string = strdup(yytext);return NOT;}
"==" {yylval.string = strdup(yytext);return EQ;}
"&&" {yylval.string = strdup(yytext);return AND;}
"||" {yylval.string = strdup(yytext);return OR;}
"true" {return TRUE;}
"false" {return FALSE;}
[a-zA-Z][a-zA-Z0-9]* {yylval.string = strdup(yytext); return ID;}
"=" {return ASSIGN;}
-?[1-9][0-9]*|0 {yylval.val=atoi(yytext); return NR;}
-?[1-9][0-9]*\.[0-9]+|-?0\.[0-9]+  {yylval.fval=atof(yytext); return FLOATNR;}
\"[_a-zA-Z0-9 ]+\" {yylval.string = strdup(yytext); yylval.string++; yylval.string[strlen(yylval.string)-1] = 0; return STR;}
\'[_a-zA-Z0-9 ]\' {yylval.string = strdup(yytext); yylval.string++; yylval.string[strlen(yylval.string)-1] = 0; return CHR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
