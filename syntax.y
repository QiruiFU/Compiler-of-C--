 %{
#define YYSTYPE struct tree_node*
#include <stdio.h>
#include "lex.yy.c"
#include "include.h"
extern struct tree_node* ROOT;
extern struct tree_node* fatherize(char* name, int line, int cnt_child, struct tree_node** children);
extern int yyerror(char*);

// #define YYDEBUG 1

int cnt_False = 0;
%}

/* declared tokens */
%token SEMI COMMA ASSIGNOP RELOP 
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token TYPE 
%token LP RP LC RC LB RB
%token STRUCT RETURN IF ELSE WHILE
%token INT FLOAT ID


%right ASSIGNOP
%left OR
%left AND 
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT 
%left DOT 
%left LB RB 
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%

// TODO : build the tree for all recursion

/*=========== High-level ========*/

Program : ExtDefList {struct tree_node* p[1]={$1}; $$=fatherize("Program", @$.first_line, 1, p); ROOT=$$;}
    ;

ExtDefList : ExtDef ExtDefList {struct tree_node* p[2]={$1,$2}; $$=fatherize("ExtDefList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

ExtDef : Specifier ExtDecList SEMI {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("ExtDef", @$.first_line, 3, p);}
    | Specifier SEMI {struct tree_node* p[2]={$1,$2}; $$=fatherize("ExtDef", @$.first_line, 2, p);}
    | Specifier FunDec CompSt {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("ExtDef", @$.first_line, 3, p);}
    ;

ExtDecList : VarDec {struct tree_node* p[1]={$1}; $$=fatherize("ExtDecList", @$.first_line, 1, p);}
    | VarDec COMMA ExtDecList {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("ExtDecList", @$.first_line, 3, p);}
    ;

/*=========== Specifiers ========*/

Specifier : TYPE {struct tree_node* p[1]={$1}; $$=fatherize("Specifier", @$.first_line, 1, p);}
    | StructSpecifier {struct tree_node* p[1]={$1}; $$=fatherize("Specifier", @$.first_line, 1, p);}
    ;

StructSpecifier : STRUCT OptTag LC DefList RC {struct tree_node* p[5]={$1,$2,$3,$4,$5}; $$=fatherize("StructSpecifier", @$.first_line, 5, p);}
    | STRUCT Tag {struct tree_node* p[2]={$1,$2}; $$=fatherize("StructSpecifier", @$.first_line, 2, p);}
    ;

OptTag : ID {struct tree_node* p[1]={$1}; $$=fatherize("OptTag", @$.first_line, 1, p);}
    |  /* empty */ {$$=NULL;}
    ;

Tag : ID {struct tree_node* p[1]={$1}; $$=fatherize("Tag", @$.first_line, 1, p);}
    ;

/*=========== Declarators ========*/

VarDec : ID {struct tree_node* p[1]={$1}; $$=fatherize("VarDec", @$.first_line, 1, p);}
    | VarDec LB INT RB {struct tree_node* p[4]={$1,$2,$3,$4}; $$=fatherize("VarDec", @$.first_line, 4, p);}
    ;

FunDec : ID LP VarList RP {struct tree_node* p[4]={$1,$2,$3,$4}; $$=fatherize("FunDec", @$.first_line, 4, p);}
    | ID LP RP {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("FunDec", @$.first_line, 3, p);}
    ;

VarList : ParamDec COMMA VarList {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("VarList", @$.first_line, 3, p);}
    | ParamDec {struct tree_node* p[1]={$1}; $$=fatherize("VarList", @$.first_line, 1, p);}
    ;

ParamDec : Specifier VarDec {struct tree_node* p[2]={$1,$2}; $$=fatherize("ParamDec", @$.first_line, 2, p);}

/*=========== Statements ========*/

CompSt : LC DefList StmtList RC {struct tree_node* p[4]={$1,$2,$3,$4}; $$=fatherize("CompSt", @$.first_line, 4, p);}
    ;

StmtList : Stmt StmtList {struct tree_node* p[2]={$1,$2}; $$=fatherize("StmtList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

Stmt : Exp SEMI {struct tree_node* p[2]={$1,$2}; $$=fatherize("Stmt", @$.first_line, 2, p);}
    | CompSt {struct tree_node* p[1]={$1}; $$=fatherize("Stmt", @$.first_line, 1, p);}
    | RETURN Exp SEMI {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Stmt", @$.first_line, 3, p);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {struct tree_node* p[5]={$1,$2,$3,$4,$5}; $$=fatherize("Stmt", @$.first_line, 5, p);}
    | IF LP Exp RP Stmt ELSE Stmt {struct tree_node* p[7]={$1,$2,$3,$4,$5,$6,$7}; $$=fatherize("Stmt", @$.first_line, 7, p);}
    | WHILE LP Exp RP Stmt {struct tree_node* p[5]={$1,$2,$3,$4,$5}; $$=fatherize("Stmt", @$.first_line, 5, p);}
    ;

/*=========== Local Definition ========*/

DefList : Def DefList {struct tree_node* p[2]={$1,$2}; $$=fatherize("DefList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

Def : Specifier DecList SEMI {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Def", @$.first_line, 3, p);}
    ;

DecList : Dec {struct tree_node* p[1]={$1}; $$=fatherize("DecList", @$.first_line, 1, p);}
    | Dec COMMA DecList {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("DecList", @$.first_line, 3, p);}
    ;

Dec : VarDec {struct tree_node* p[1]={$1}; $$=fatherize("Dec", @$.first_line, 1, p);}
    | VarDec ASSIGNOP Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Dec", @$.first_line, 3, p);}
    ;

/*=========== Expression ========*/

Exp : Exp ASSIGNOP Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp AND Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp OR Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp RELOP Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp PLUS Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp MINUS Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp STAR Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp DIV Exp {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | LP Exp RP {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | MINUS Exp {struct tree_node* p[2]={$1,$2}; $$=fatherize("Exp", @$.first_line, 2, p);}
    | NOT Exp {struct tree_node* p[2]={$1,$2}; $$=fatherize("Exp", @$.first_line, 2, p);}
    | ID LP Args RP {struct tree_node* p[4]={$1,$2,$3,$4}; $$=fatherize("Exp", @$.first_line, 4, p);}
    | ID LP RP {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | Exp LB Exp RB {struct tree_node* p[4]={$1,$2,$3,$4}; $$=fatherize("Exp", @$.first_line, 4, p);}
    | Exp DOT ID {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Exp", @$.first_line, 3, p);}
    | ID {struct tree_node* p[1]={$1}; $$=fatherize("Exp", @$.first_line, 1, p);}
    | INT {struct tree_node* p[1]={$1}; $$=fatherize("Exp", @$.first_line, 1, p);}
    | FLOAT {struct tree_node* p[1]={$1}; $$=fatherize("Exp", @$.first_line, 1, p);}
    ;

Args : Exp COMMA Args {struct tree_node* p[3]={$1,$2,$3}; $$=fatherize("Args", @$.first_line, 3, p);}
    | Exp {struct tree_node* p[1]={$1}; $$=fatherize("Args", @$.first_line, 1, p);}
    ;


%%

int yyerror(char* msg){
    printf("Error type B at Line %d : %s \n", lineno, msg);
    cnt_False++;
}