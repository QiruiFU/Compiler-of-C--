%{
#include "syntax.tab.h"
#include "include.h"
#include<string.h>
#include<stdlib.h>
int lineno = 1;
int yycolumn = 1;
extern int cnt_False;

#define YYSTYPE struct tree_node*

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = lineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

struct tree_node* new_node(char* name, int line, char* val);

%}

letter [a-zA-Z]
digit [0-9]
PosNum [1-9]

%%

"//" {char c=input(); while(c!='\n') c=input(); lineno++; yycolumn=1;}
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

[\t ]+ {}
\n {lineno++; yycolumn=1;}
";" { yylval=new_node("SEMI", lineno, yytext); return SEMI;}
"," { yylval=new_node("COMMA", lineno, yytext); return COMMA;}
"=" { yylval=new_node("ASSIGNOP", lineno, yytext); return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" { yylval=new_node("RELOP", lineno, yytext); return RELOP;}
"+" { yylval=new_node("PLUS", lineno, yytext); return PLUS;}
"-" { yylval=new_node("MINUS", lineno, yytext); return MINUS;}
"*" { yylval=new_node("STAR", lineno, yytext); return STAR;}
"/" { yylval=new_node("DIV", lineno, yytext); return DIV;}
"&&" { yylval=new_node("AND", lineno, yytext); return AND;}
"||" { yylval=new_node("OR", lineno, yytext); return OR;}
"." { yylval=new_node("DOT", lineno, yytext); return DOT;}
"!" { yylval=new_node("NOT", lineno, yytext); return NOT;}
"int"|"float" { yylval=new_node("TYPE", lineno, yytext); return TYPE;}
"(" { yylval=new_node("LP", lineno, yytext); return LP;}
")" { yylval=new_node("RP", lineno, yytext); return RP;}
"[" { yylval=new_node("LB", lineno, yytext); return LB;}
"]" { yylval=new_node("RB", lineno, yytext); return RB;}
"{" { yylval=new_node("LC", lineno, yytext); return LC;}
"}" { yylval=new_node("RC", lineno, yytext); return RC;}
"struct" { yylval=new_node("STRUCT", lineno, yytext); return STRUCT;}
"return" { yylval=new_node("RETURN", lineno, yytext); return RETURN;}
"if" { yylval=new_node("IF", lineno, yytext); return IF;}
"else" { yylval=new_node("ELSE", lineno, yytext); return ELSE;}
"while" { yylval=new_node("WHILE", lineno, yytext); return WHILE;}
{PosNum}{digit}*|0 { yylval=new_node("INT", lineno, yytext); return INT;}
0{digit}+ {printf("Error type A at Line %d: invalid 0\n", lineno);}
({PosNum}{digit}*|0)\.{digit}+ { yylval=new_node("FLOAT", lineno, yytext); return FLOAT;}
("_"|{letter})("_"|{digit}|{letter})* { yylval=new_node("ID", lineno, yytext); return ID;}
.  {cnt_False++; printf("Error type A at Line %d: Mysterious characters \'%s\' \n", lineno, yytext);}

%%

struct tree_node* new_node(char name[], int line, char val[]){
    // printf("%s %d %s\n", name, line, val);
    struct tree_node* new_pnt = (struct tree_node*)malloc(sizeof(struct tree_node));
    strcpy(new_pnt->name, name);
    new_pnt->first_line = line;
    new_pnt->father = new_pnt->first_child = new_pnt->brother = NULL;

    if(strcmp(name, "INT")==0){
        new_pnt->compos.val_int = atoi(val);
    }
    else if(strcmp(name, "FLOAT")==0){
        new_pnt->compos.val_float = (float)atof(val);
    }
    else{
        strcpy(new_pnt->compos.id, val);
    }

    return new_pnt;
}