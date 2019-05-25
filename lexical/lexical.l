%{
    /*a lex program for SPL language*/
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

Token yylval;
int lineno = 1;

int yywrap();
int make_token(int tokentype, int tokenname);// return token_index, fill the yylval for yacc
int make_data_val(int tokenname);
int make_id(int tokenname);
void lex_error(const char* error_info);
%}

digit [0-9]
letter [A-Za-z]
id  (_|{letter})(_|{letter}|{digit})*    
integer {digit}+  
real {digit}*\.{digit}+    
newline \n
comment  \/\/.*\n    
whitespace [ \t]+

%%
{newline}   {lineno++;}
{comment}  {lineno++;}
{whitespace}   

"("     {return make_token(OPERATOR,LP);}
")"     {return make_token(OPERATOR,RP);}
"["     {return make_token(OPERATOR,LB);}
"]"     {return make_token(OPERATOR,RB);}
"."     {return make_token(OPERATOR,DOT);}
","     {return make_token(OPERATOR,COMMA);}
":"     {return make_token(OPERATOR,COLON);}
"*"     {return make_token(OPERATOR,MUL);}
"/"     {return make_token(OPERATOR,DIV_R);}
"<>"    {return make_token(OPERATOR,NE);}
"+"     {return make_token(OPERATOR,PLUS);}
"-"     {return make_token(OPERATOR,MINUS);}
">="    {return make_token(OPERATOR,GE);}
">"     {return make_token(OPERATOR,GT);}
"<="    {return make_token(OPERATOR,LE);}
"<"     {return make_token(OPERATOR,LT);}
"="     {return make_token(OPERATOR,EQUAL);}
":="    {return make_token(OPERATOR,ASSIGN);}
".."    {return make_token(OPERATOR,DOTDOT);}
";"     {return make_token(OPERATOR,SEMI);}
"and"   {return make_token(OPERATOR,AND);}
"div"   {return make_token(OPERATOR,DIV);}
"or"    {return make_token(OPERATOR,OR);}
"not"   {return make_token(OPERATOR,NOT);}
"mod"|"%"   {return make_token(OPERATOR,MOD);}

"array"     {return make_token(RESERVED,ARRAY);}
"begin"     {return make_token(RESERVED,MY_BEGIN);}
"case"      {return make_token(RESERVED,CASE);}
"const"     {return make_token(RESERVED,CONST);}
"downto"    {return make_token(RESERVED,DOWNTO);}
"do"        {return make_token(RESERVED,DO);}
"else"      {return make_token(RESERVED,ELSE);}
"end"       {return make_token(RESERVED,END);}
"for"       {return make_token(RESERVED,FOR);}
"function"  {return make_token(RESERVED,FUNCT);}
"goto"      {return make_token(RESERVED,GOTO);}
"if"        {return make_token(RESERVED,IF);}
"in"        {return make_token(RESERVED,IN);}
"of"        {return make_token(RESERVED,OF);}
"packed"    {return make_token(RESERVED,PACKED);}
"procedure" {return make_token(RESERVED,PROC);}
"program"   {return make_token(RESERVED,PROGRAM);}
"record"    {return make_token(RESERVED,RECORD);}
"repeat"    {return make_token(RESERVED,REPEAT);}
"set"       {return make_token(RESERVED,SET);}
"then"      {return make_token(RESERVED,THEN);}
"to"        {return make_token(RESERVED,TO);}
"type"      {return make_token(RESERVED,TYPE);}
"until"     {return make_token(RESERVED,UNTIL);}
"var"       {return make_token(RESERVED,VAR);}
"while"     {return make_token(RESERVED,WHILE);}
"with"      {return make_token(RESERVED,WITH);}
"read"      {return make_token(RESERVED,READ);}

"false"     {return make_token(SYS_CON,FALSE);}
"maxint"    {return make_token(SYS_CON,MAXINT);}
"true"      {return make_token(SYS_CON,TRUE);}
"abs"       {return make_token(SYS_FUN,ABS);}
"chr"       {return make_token(SYS_FUN,CHR);}
"odd"       {return make_token(SYS_FUN,ODD);}
"ord"       {return make_token(SYS_FUN,ORD);}
"pred"      {return make_token(SYS_FUN,PRED);}
"sqr"       {return make_token(SYS_FUN,SQR);}
"sqrt"      {return make_token(SYS_FUN,SQRT);}
"succ"      {return make_token(SYS_FUN,SUCC);}
"write"     {return make_token(SYS_PROC,WRITE);}
"writeln"   {return make_token(SYS_PROC,WRITELN);}
"boolean"   {return make_token(SYS_TYPE,BOOL);}
"char"      {return make_token(SYS_TYPE,CHAR);}
"integer"   {return make_token(SYS_TYPE,INT);}
"real"      {return make_token(SYS_TYPE,REAL);}
"string"    {return make_token(SYS_TYPE,STR);}
   
{real}      {return make_data_val(REAL);}
{integer}  {return make_data_val(INT);}
{id}        {return make_id(ID);}
.         {lex_error("wrong identifier");}

%%

int main()
{
    int tag = 1, num = 0;
    printf("scanner: \n");
    while(tag)
    {
        if(num = yylex())
        {
            printf("yylex() = %4d ", num);
            printtoken(yylval);
        }
        else
            tag = 0;
    }
    return 0;
}

int make_token(int token_type, int token_index)
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval-> token_type = token_type;
    yylval-> token_index = token_index;
    strcpy(yylval->token_string, yytext);
    return token_index;
}

int make_data_val(int token_index) //in case of literal constant or id
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval->token_type = token_index;
    if(yyleng > 32)
        lex_error("max-length is 32!");
    switch(token_index)
    {
        case REAL:
            sscanf(yytext,"%lf", &(yylval->token_val.real_num));
            return CONST_REAL;
        case INT:
            sscanf(yytext,"%d", &(yylval->token_val.int_num));
            return CONST_INT;
    }
    return token_index;
}

int make_id(int tokenname)
{
    if(yyleng > 32)
        lex_error("max-length is 32!");
    yylval->token_type = ID;
    strcpy(yylval->token_string, yytext);
    return ID;
}

void printtoken(Token yylval) //displaying the result of lex
{
    printf("token type: %4d ", yylval->token_type);
    printf("token index: %4d ", yylval->token_index);
    printf("token name: %s\n", yylval->token_string);
}

void lex_error(const char* error_info)
{
    printf("error in line %d: %s\n", lineno, error_info);
    exit(1);
}

int yywrap()
{
    return 1;
}