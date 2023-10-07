%{
#include "syntax.tab.h"
#include "ParseTree.h"
#include "config.h"
#include<string.h>
#include<stdlib.h>
#include<assert.h>
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
OctDigit [0-7]
HexDigit [0-9A-Fa-f]
PosNum [1-9]

%%

"//" {
    char c=input(); while(c!='\n') c=input(); lineno++; yycolumn=1;
    #ifndef LAB1_3 
        cnt_False++;
        printf("Error type B at Line %d: comments are not allowed\n", lineno-1);
    #endif
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
    #ifndef LAB1_3
        cnt_False++;
        printf("Error type B at Line %d: comments are not allowed\n", lineno);
    #endif
    
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

{PosNum}{digit}*|0 { yylval=new_node("DECE", lineno, yytext); return INT;}
0{OctDigit}+ {yylval=new_node("OCT", lineno, yytext); return INT;}
0[Xx]{HexDigit}+ {yylval=new_node("HEX", lineno, yytext); return INT;}

({PosNum}{digit}*|0)\.{digit}+ { yylval=new_node("FLOAT", lineno, yytext); return FLOAT;}
("_"|{letter})("_"|{digit}|{letter})* { yylval=new_node("ID", lineno, yytext); return ID;}
~  {cnt_False++; printf("Error type A at Line %d: Mysterious characters \'%s\' \n", lineno, yytext);}
.  {cnt_False++; /*printf("Error type A at Line %d: Mysterious characters \'%s\' \n", lineno, yytext);*/}

%%

int Hex2Dec(char ch){
    if(ch>='0' && ch<='9'){
        return ch - '0';
    }
    else if(ch>='a' && ch<='f'){
        return ch - 'a' + 10;
    }
    else if(ch>='A' && ch<='F'){
        return ch - 'A' + 10;
    }
    else{
        printf("wrong hex\n");
        assert(0);
    }
}

struct tree_node* new_node(char name[], int line, char val[]){
    // printf("%s %d %s\n", name, line, val);
    struct tree_node* new_pnt = (struct tree_node*)malloc(sizeof(struct tree_node));
    strcpy(new_pnt->name, name);
    new_pnt->first_line = line;
    new_pnt->father = new_pnt->first_child = new_pnt->brother = NULL;

    if(strcmp(name, "DECE")==0){
        new_pnt->compos.val_int = atoi(val);
        strcpy(new_pnt->name, "INT");
    }
    else if(strcmp(name, "OCT")==0){
        int l = strlen(val)-1;
        int res = 0, base = 1;
        while(l){
            res += (val[l]-'0') * base;
            base *= 8;
            l--;
        }
        new_pnt->compos.val_int = res;
        strcpy(new_pnt->name, "INT");
    }
    else if(strcmp(name, "HEX")==0){
        int l = strlen(val)-1;
        int res = 0, base = 1;
        while(l>1){
            res += Hex2Dec(val[l])*base;
            base *= 16;
            l--;
        }
        new_pnt->compos.val_int = res;
        strcpy(new_pnt->name, "INT");
    }
    else if(strcmp(name, "FLOAT")==0){
        new_pnt->compos.val_float = (float)atof(val);
    }
    else{
        strcpy(new_pnt->compos.id, val);
    }

    return new_pnt;
}