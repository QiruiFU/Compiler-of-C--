 %{
#define YYSTYPE TreeNode*
#include <stdio.h>
#include "lex.yy.c"
#include "parse-tree.h"
extern ParseTree* parse_tree;
extern int yyerror(char*);

// #define YYDEBUG 1

int cnt_error = 0;
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

/*=========== High-level ========*/

Program : ExtDefList {TreeNode* p[1]={$1}; $$=Fatherize("Program", @$.first_line, 1, p); parse_tree = ParseTreeInit($$);}
    ;

ExtDefList : ExtDef ExtDefList {TreeNode* p[2]={$1,$2}; $$=Fatherize("ExtDefList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

ExtDef : Specifier ExtDecList SEMI {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("ExtDef", @$.first_line, 3, p);}
    | Specifier SEMI {TreeNode* p[2]={$1,$2}; $$=Fatherize("ExtDef", @$.first_line, 2, p);}
    | Specifier FunDec CompSt {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("ExtDef", @$.first_line, 3, p);}
    | Specifier ExtDecList error SEMI 
    | Specifier error SEMI 
    | Specifier error CompSt 
    | Specifier FunDec error SEMI
    | error SEMI
    ;

ExtDecList : VarDec {TreeNode* p[1]={$1}; $$=Fatherize("ExtDecList", @$.first_line, 1, p);}
    | VarDec COMMA ExtDecList {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("ExtDecList", @$.first_line, 3, p);}
    ;

/*=========== Specifiers ========*/

Specifier : TYPE {TreeNode* p[1]={$1}; $$=Fatherize("Specifier", @$.first_line, 1, p);}
    | StructSpecifier {TreeNode* p[1]={$1}; $$=Fatherize("Specifier", @$.first_line, 1, p);}
    ;

StructSpecifier : STRUCT OptTag LC DefList RC {TreeNode* p[5]={$1,$2,$3,$4,$5}; $$=Fatherize("StructSpecifier", @$.first_line, 5, p);}
    | STRUCT Tag {TreeNode* p[2]={$1,$2}; $$=Fatherize("StructSpecifier", @$.first_line, 2, p);}
    | STRUCT OptTag error
    | STRUCT OptTag LC error RC
    | error OptTag LC DefList RC
    | error Tag
    ;

OptTag : ID {TreeNode* p[1]={$1}; $$=Fatherize("OptTag", @$.first_line, 1, p);}
    |  /* empty */ {$$=NULL;}
    ;

Tag : ID {TreeNode* p[1]={$1}; $$=Fatherize("Tag", @$.first_line, 1, p);}
    ;

/*=========== Declarators ========*/

VarDec : ID {TreeNode* p[1]={$1}; $$=Fatherize("VarDec", @$.first_line, 1, p);}
    | VarDec LB INT RB {TreeNode* p[4]={$1,$2,$3,$4}; $$=Fatherize("VarDec", @$.first_line, 4, p);}
    | VarDec LB error RB
    | error LB INT RB
    ;

FunDec : ID LP VarList RP {TreeNode* p[4]={$1,$2,$3,$4}; $$=Fatherize("FunDec", @$.first_line, 4, p);}
    | ID LP RP {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("FunDec", @$.first_line, 3, p);}
    | ID LP error RP
    | error LP RP
    ;

VarList : ParamDec COMMA VarList {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("VarList", @$.first_line, 3, p);}
    | ParamDec {TreeNode* p[1]={$1}; $$=Fatherize("VarList", @$.first_line, 1, p);}
    | ParamDec COMMA error
    ;

ParamDec : Specifier VarDec {TreeNode* p[2]={$1,$2}; $$=Fatherize("ParamDec", @$.first_line, 2, p);}

/*=========== Statements ========*/

CompSt : LC StmtList RC {TreeNode* p[4]={$1,$2,$3}; $$=Fatherize("CompSt", @$.first_line, 3, p);}
    | LC error RC
    ;

StmtList : Stmt StmtList {TreeNode* p[2]={$1,$2}; $$=Fatherize("StmtList", @$.first_line, 2, p);}
    | Def StmtList {TreeNode* p[2]={$1,$2}; $$=Fatherize("StmtList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

Stmt : Exp SEMI {TreeNode* p[2]={$1,$2}; $$=Fatherize("Stmt", @$.first_line, 2, p);}
    | CompSt {TreeNode* p[1]={$1}; $$=Fatherize("Stmt", @$.first_line, 1, p);}
    | RETURN Exp SEMI {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Stmt", @$.first_line, 3, p);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {TreeNode* p[5]={$1,$2,$3,$4,$5}; $$=Fatherize("Stmt", @$.first_line, 5, p);}
    | IF LP Exp RP Stmt ELSE Stmt {TreeNode* p[7]={$1,$2,$3,$4,$5,$6,$7}; $$=Fatherize("Stmt", @$.first_line, 7, p);}
    | WHILE LP Exp RP Stmt {TreeNode* p[5]={$1,$2,$3,$4,$5}; $$=Fatherize("Stmt", @$.first_line, 5, p);}
    | IF error Stmt
    | IF error ELSE Stmt
    | WHILE error Stmt
    | RETURN error SEMI
    | error SEMI
    ;

/*=========== Local Definition ========*/

DefList : Def DefList {TreeNode* p[2]={$1,$2}; $$=Fatherize("DefList", @$.first_line, 2, p);}
    |  /* empty */ {$$=NULL;}
    ;

Def : Specifier DecList SEMI {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Def", @$.first_line, 3, p);}
    | Specifier error SEMI
    ;

DecList : Dec {TreeNode* p[1]={$1}; $$=Fatherize("DecList", @$.first_line, 1, p);}
    | Dec COMMA DecList {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("DecList", @$.first_line, 3, p);}
    | error DecList
    ;

Dec : VarDec {TreeNode* p[1]={$1}; $$=Fatherize("Dec", @$.first_line, 1, p);}
    | VarDec ASSIGNOP Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Dec", @$.first_line, 3, p);}
    | VarDec error Exp
    | VarDec ASSIGNOP error
    ;

/*=========== Expression ========*/

Exp : Exp ASSIGNOP Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp AND Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp OR Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp RELOP Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp PLUS Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp MINUS Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp STAR Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp DIV Exp {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | LP Exp RP {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | MINUS Exp {TreeNode* p[2]={$1,$2}; $$=Fatherize("Exp", @$.first_line, 2, p);}
    | NOT Exp {TreeNode* p[2]={$1,$2}; $$=Fatherize("Exp", @$.first_line, 2, p);}
    | ID LP Args RP {TreeNode* p[4]={$1,$2,$3,$4}; $$=Fatherize("Exp", @$.first_line, 4, p);}
    | ID LP RP {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | Exp LB Exp RB {TreeNode* p[4]={$1,$2,$3,$4}; $$=Fatherize("Exp", @$.first_line, 4, p);}
    | Exp DOT ID {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Exp", @$.first_line, 3, p);}
    | ID {TreeNode* p[1]={$1}; $$=Fatherize("Exp", @$.first_line, 1, p);}
    | INT {TreeNode* p[1]={$1}; $$=Fatherize("Exp", @$.first_line, 1, p);}
    | FLOAT {TreeNode* p[1]={$1}; $$=Fatherize("Exp", @$.first_line, 1, p);}
    | Exp error Exp
    | Exp error
    | ID LP error RP
    ;

Args : Exp COMMA Args {TreeNode* p[3]={$1,$2,$3}; $$=Fatherize("Args", @$.first_line, 3, p);}
    | Exp {TreeNode* p[1]={$1}; $$=Fatherize("Args", @$.first_line, 1, p);}
    ;


%%

int yyerror(char* msg){
    printf("Error type B at Line %d: %s \n", lineno, msg);
    cnt_error++;
}