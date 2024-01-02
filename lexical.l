%{
#include "parse-tree.h"
#include "syntax.tab.h"
int lineno = 1;
int yycolumn = 1;
extern int cnt_error;

#define YYSTYPE TreeNode*

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = lineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

letter [a-zA-Z]
digit [0-9]
OctDigit [0-7]
HexDigit [0-9A-Fa-f]
PosNum [1-9]

%%

"//" {
        char c=input(); while(c!='\n') c=input(); lineno++; yycolumn=1;
    }
"/*" {
        char a=input(); char b=input();
        if(a=='\n') lineno++;
        if(b=='\n') lineno++;
        while(a!='*'||b!='/'){
            a = b;
            b = input();
            if(b=='\n'){
                lineno++;
                yycolumn = 1;
            }
            if(b=='\0'){
                printf("Error type A at Line %d: No match comment symbol\n", lineno);
                break;
            }
        }
    }

[\t \r]+ {}
\n {lineno++; yycolumn=1;}
";" { yylval=TreeNodeInit("SEMI", lineno, yytext); return SEMI;}
"," { yylval=TreeNodeInit("COMMA", lineno, yytext); return COMMA;}
"=" { yylval=TreeNodeInit("ASSIGNOP", lineno, yytext); return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" { yylval=TreeNodeInit("RELOP", lineno, yytext); return RELOP;}
"+" { yylval=TreeNodeInit("PLUS", lineno, yytext); return PLUS;}
"-" { yylval=TreeNodeInit("MINUS", lineno, yytext); return MINUS;}
"*" { yylval=TreeNodeInit("STAR", lineno, yytext); return STAR;}
"/" { yylval=TreeNodeInit("DIV", lineno, yytext); return DIV;}
"&&" { yylval=TreeNodeInit("AND", lineno, yytext); return AND;}
"||" { yylval=TreeNodeInit("OR", lineno, yytext); return OR;}
"." { yylval=TreeNodeInit("DOT", lineno, yytext); return DOT;}
"!" { yylval=TreeNodeInit("NOT", lineno, yytext); return NOT;}
"int"|"float" { yylval=TreeNodeInit("TYPE", lineno, yytext); return TYPE;}
"(" { yylval=TreeNodeInit("LP", lineno, yytext); return LP;}
")" { yylval=TreeNodeInit("RP", lineno, yytext); return RP;}
"[" { yylval=TreeNodeInit("LB", lineno, yytext); return LB;}
"]" { yylval=TreeNodeInit("RB", lineno, yytext); return RB;}
"{" { yylval=TreeNodeInit("LC", lineno, yytext); return LC;}
"}" { yylval=TreeNodeInit("RC", lineno, yytext); return RC;}
"struct" { yylval=TreeNodeInit("STRUCT", lineno, yytext); return STRUCT;}
"return" { yylval=TreeNodeInit("RETURN", lineno, yytext); return RETURN;}
"if" { yylval=TreeNodeInit("IF", lineno, yytext); return IF;}
"else" { yylval=TreeNodeInit("ELSE", lineno, yytext); return ELSE;}
"while" { yylval=TreeNodeInit("WHILE", lineno, yytext); return WHILE;}

{PosNum}{digit}*|0 { yylval=TreeNodeInit("DECE", lineno, yytext); return INT;}
0{OctDigit}+ {yylval=TreeNodeInit("OCT", lineno, yytext); return INT;}
0[Xx]{HexDigit}+ {yylval=TreeNodeInit("HEX", lineno, yytext); return INT;}

({PosNum}{digit}*|0)\.{digit}+ { yylval=TreeNodeInit("FLOAT", lineno, yytext); return FLOAT;}
("_"|{letter})("_"|{digit}|{letter})* { yylval=TreeNodeInit("ID", lineno, yytext); return ID;}
.  {cnt_error++; printf("Error type A at Line %d: Mysterious token \'%s\' \n", lineno, yytext);}

%%
