%{
    /*a lex program for SPL language*/
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

Token yylval;
int lineno = 1;

int yywrap();
int make_token(int tokentype, int tokenname);// return token_index, fill the yylval for yacc
int make_const_val(int tokenname);
int make_id(int tokenname);
int make_string();
int make_char();
void lex_error(const char* error_info);
void printtoken(Token yylval);
%}

digit [0-9]
letter [A-Za-z]
id  (_|{letter})(_|{letter}|{digit})*    
integer {digit}+  
real {digit}*\.{digit}+    
char ('({letter}|{digit})')|('\\[0ntr]')
string \".*\"
newline \n
comment  \/\/.*\n    
whitespace [ \t]+

%%
{newline}   {lineno++;}
{comment}  {lineno++;}
{whitespace}   

"("     {return make_token(OPERATOR,LP);}
")"     {return make_token(OPERATOR,RP);}
"["     {return make_token(OPERATOR,LSB);}
"]"     {return make_token(OPERATOR,RSB);}
"{"     {return make_token(OPERATOR,LCB);}
"}"     {return make_token(OPERATOR,RCB);}
","     {return make_token(OPERATOR,COMMA);}
"*"     {return make_token(OPERATOR,MUL);}
"/"     {return make_token(OPERATOR,DIV);}
"!="    {return make_token(OPERATOR,NE);}
"+"     {return make_token(OPERATOR,PLUS);}
"-"     {return make_token(OPERATOR,MINUS);}
">="    {return make_token(OPERATOR,GE);}
">"     {return make_token(OPERATOR,GT);}
"<="    {return make_token(OPERATOR,LE);}
"<"     {return make_token(OPERATOR,LT);}
"=="     {return make_token(OPERATOR,EQUAL);}
"="    {return make_token(OPERATOR,ASSIGN);}
";"     {return make_token(OPERATOR,SEMI);}
"and"   {return make_token(OPERATOR,AND);}
"or"    {return make_token(OPERATOR,OR);}
"not"   {return make_token(OPERATOR,NOT);}
"%"   {return make_token(OPERATOR,MOD);}

"void"      {return make_token(RESERVE,VOID);}
"const"     {return make_token(RESERVE,CONST);}
"else"      {return make_token(RESERVE,ELSE);}
"if"        {return make_token(RESERVE,IF);}
"var"       {return make_token(RESERVE,VAR);}
"while"     {return make_token(RESERVE,WHILE);}

"false"     {return make_token(SYS_CON,FALSE);}
"true"      {return make_token(SYS_CON,TRUE);}
"MAX_INT"    {return make_token(SYS_CON,MAXINT);}

"abs"       {return make_token(SYS_FUN,ABS);}
"sqrt"      {return make_token(SYS_FUN,SQRT);}
"max"       {return make_token(SYS_FUN,MAX);}
"min"      {return make_token(SYS_FUN,MIN);}

"bool"   {return make_token(SYS_TYPE,BOOL);}
"char"      {return make_token(SYS_TYPE,CHAR);}
"int"   {return make_token(SYS_TYPE,INT);}
"double"      {return make_token(SYS_TYPE,REAL);}
"string"    {return make_token(SYS_TYPE,STR);}
   
{string}        {return make_string();}
{char}     {return make_char();}
{real}      {return make_const_val(REAL);}
{integer}  {return make_const_val(INT);}
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

int make_const_val(int token_index) //in case of literal constant or id
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

int make_string()
{
    yylval->token_type = CONST_STR;
    if(yyleng > 32)
        lex_error("max-length is 32!\n");
    char * str = yylval->token_string;
    int j = 0;
    for(int i = 1; i < yyleng - 1; i++)
    {
        if(yytext[i] != '\\')
            str[j++] =  yytext[i];
        else
        {
            switch(yytext[++i])
            {
                case '0':
                    str[j++] = '\0';
                    break;
                case 't':
                    str[j++] = '\t';
                    break;
                case 'n':
                    str[j++] = '\n';
                    break;
                case 'r':
                    str[j++] = '\r';
                    break;
                default:
                    lex_error("escape charactor error!");
            }
        }
    }
    str[j] = '\0';
    return CONST_STR;
}

int make_char()
{
    yylval->token_type = CONST_CHAR;
    strcpy(yylval->token_string, yytext);
    if(yyleng == 3)
        yylval->token_val.ch = yytext[1];
    else
    {
        switch(yytext[2])
        {
            case '0':
                yylval->token_val.ch = '\0';
                break;
            case 't':
                yylval->token_val.ch = '\t';
                break;
            case 'n':
                yylval->token_val.ch = '\n';
                break;
            case 'r':
                yylval->token_val.ch = '\r';
                break;
        }
    }
    return CONST_CHAR;
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