%{
    /*a lex program for SPL language*/
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

Token yylval;
int lineno = 1;

int yywrap();
int make_token(int token_type, int token_index);// return token_index, fill the yylval for yacc
int make_const_val(int token_type);
int make_sys_con(int token_type, int token_val);
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
double {digit}*\.{digit}+    
char ('({letter}|{digit})')|('\\[0ntr]')
newline \n
comment  \/\/.*\n    
whitespace [ \t]+

%%
{newline}   {lineno++;}
{comment}  {lineno++;}
{whitespace}   

"("     {return LP;}
")"     {return RP;}
"["     {return LSB;}
"]"     {return RSB;}
"{"     {return LCB;}
"}"     {return RCB;}
","     {return COMMA;}
"*"     {return MUL;}
"/"     {return DIV;}
"+"     {return PLUS;}
"-"     {return MINUS;}
">="    {return GE;}
">"     {return GT;}
"<="    {return LE;}
"<"     {return LT;}
"=="    {return EQUAL;}
"!="    {return NE;}
"="     {return ASSIGN;}
";"     {return SEMI;}
"and"   {return AND;}
"or"    {return OR;}
"not"   {return NOT;}
"%"     {return MOD;}
"&"     {return ADDR;}

"void"      {return VOID;}
"else"      {return ELSE;}
"if"        {return IF;}
"while"     {return WHILE;}
"return"    {return RETURN;}

"false"     {return make_sys_con(CONST_BOOL,FALSE);}
"true"      {return make_sys_con(CONST_BOOL,TRUE);}
"MAX_INT"   {return make_sys_con(CONST_INT,MAX_INT);}
"MIN_INT"   {return make_sys_con(CONST_INT,MIN_INT);}

"abs"       {return make_token(SYS_FUN,ABS);}
"sqrt"      {return make_token(SYS_FUN,SQRT);}
"max"       {return make_token(SYS_FUN,MAX);}
"min"       {return make_token(SYS_FUN,MIN);}

"bool"      {return make_token(SYS_TYPE,BOOL);}
"char"      {return make_token(SYS_TYPE,CHAR);}
"int"       {return make_token(SYS_TYPE,INT);}
"double"    {return make_token(SYS_TYPE,DOUBLE);}
"void"      {return make_token(SYS_TYPE,VOID);}

{char}      {return make_char();}
{double}    {return make_const_val(CONST_DOUBLE);}
{integer}   {return make_const_val(CONST_INT);}
{id}        {return make_id(ID);}
.           {lex_error("wrong identifier");}      

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

int make_sys_con(int token_type, int token_val)
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval-> token_type = token_type;
    switch(token_type)
    {
        case CONST_BOOL:
            yylval->token_val.bool_num = token_val;
            break;
        case CONST_INT:
            yylval->token_val.int_num = token_val;
            break;
    }
    return token_type;
}

int make_const_val(int token_type) //in case of literal constant or id
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval->token_type = token_type;
    if(yyleng > 32)
        lex_error("max-length is 32!");
    strcpy(yylval->token_string, yytext);
    switch(token_type)
    {
        case CONST_DOUBLE:
            sscanf(yytext,"%lf", &(yylval->token_val.double_num));
            return CONST_DOUBLE;
        case CONST_INT:
            sscanf(yytext,"%d", &(yylval->token_val.int_num));
            return CONST_INT;
    }
    return token_type;
}

int make_id(int tokenname)
{
    if(yyleng > 32)
        lex_error("max-length is 32!");
    yylval->token_type = ID;
    strcpy(yylval->token_string, yytext);
    return ID;
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