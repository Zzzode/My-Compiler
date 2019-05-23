%{
    /*a lex program for SPL language*/
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
Token yylval;
int lineno = 1;

int yywrap();
int make_token_val(int tokentype, int tokenname);// return token_name, fill the yylval for yacc
int datatype_val(int tokenname);
void printtoken(Token yylval);
void comment();
void lex_error(const char* error_info);
%}
//only support the escape charactor of \n \t \0 \r, in other cases \letter is regareded as letter
//regular expression
digit [0-9]
letter [a-zA-Z]
id (_|{letter})(_|{letter}|{digit})*    //identifier
integer {digit}+  
real {digit}*\.{digit}+     //real number 
newline \n
commet ^\/\/     // c++-style comment: // in linehead
whitespace [ \t]+

%%
{newline}   {lineno++;}
{comment}   {comment();lineno++;}
{whitespace}   

"("     {return make_token_val(OPERATOR,LP);}
")"     {return make_token_val(OPERATOR,RP);}
"["     {return make_token_val(OPERATOR,LB);}
"]"     {return make_token_val(OPERATOR,RB);}
"."     {return make_token_val(OPERATOR,DOT);}
","     {return make_token_val(OPERATOR,COMMA);}
":"     {return make_token_val(OPERATOR,COLON);}
"*"     {return make_token_val(OPERATOR,MUL);}
"/"     {return make_token_val(OPERATOR,DIV_R);}
"<>"    {return make_token_val(OPERATOR,NE);}
"+"     {return make_token_val(OPERATOR,PLUS);}
"-"     {return make_token_val(OPERATOR,MINUS);}
">="    {return make_token_val(OPERATOR,GE);}
">"     {return make_token_val(OPERATOR,GT);}
"<="    {return make_token_val(OPERATOR,LE);}
"<"     {return make_token_val(OPERATOR,LT);}
"="     {return make_token_val(OPERATOR,EQUAL);}
":="    {return make_token_val(OPERATOR,ASSIGN);}
".."    {return make_token_val(OPERATOR,DOTDOT);}
";"     {return make_token_val(OPERATOR,SEMI);}
"and"   {return make_token_val(OPERATOR,AND);}
"div"   {return make_token_val(OPERATOR,DIV);}
"or"    {return make_token_val(OPERATOR,OR);}
"not"   {return make_token_val(OPERATOR,NOT);}
"mod"|"%"   {return make_token_val(OPERATOR,MOD);}

"array"     {return make_token_val(reserved_word,ARRAY);}
"begin"     {return make_token_val(reserved_word,BEGIN);}
"case"      {return make_token_val(reserved_word,CASE);}
"const"     {return make_token_val(reserved_word,CONST);}
"downto"    {return make_token_val(reserved_word,DOWNTO);}
"do"        {return make_token_val(reserved_word,DO);}
"else"      {return make_token_val(reserved_word,ELSE);}
"end"       {return make_token_val(reserved_word,END);}
"for"       {return make_token_val(reserved_word,FOR);}
"function"  {return make_token_val(reserved_word,FUNCT);}
"goto"      {return make_token_val(reserved_word,GOTO);}
"if"        {return make_token_val(reserved_word,IF);}
"in"        {return make_token_val(reserved_word,IN);}
"of"        {return make_token_val(reserved_word,OF);}
"packed"    {return make_token_val(reserved_word,PACKED);}
"procedure" {return make_token_val(reserved_word,PROC);}
"program"   {return make_token_val(reserved_word,PROGRAM);}
"record"    {return make_token_val(reserved_word,RECORD);}
"repeat"    {return make_token_val(reserved_word,REPEAT);}
"set"       {return make_token_val(reserved_word,SET);}
"then"      {return make_token_val(reserved_word,THEN);}
"to"        {return make_token_val(reserved_word,TO);}
"type"      {return make_token_val(reserved_word,TYPE);}
"until"     {return make_token_val(reserved_word,UNTIL);}
"var"       {return make_token_val(reserved_word,VAR);}
"while"     {return make_token_val(reserved_word,WHILE);}
"with"      {return make_token_val(reserved_word,WITH);}
"read"      {return make_token_val(reserved_word,READ);}

"false"     {return make_token_val(SYS_CON,FALSE);}
"maxint"    {return make_token_val(SYS_CON,MAXINT);}
"true"      {return make_token_val(SYS_CON,TRUE);}
"abs"       {return make_token_val(SYS_FUNCT,ABS);}
"chr"       {return make_token_val(SYS_FUNCT,CHR);}
"odd"       {return make_token_val(SYS_FUNCT,ODD);}
"ord"       {return make_token_val(SYS_FUNCT,ORD);}
"pred"      {return make_token_val(SYS_FUNCT,PRED);}
"sqr"       {return make_token_val(SYS_FUNCT,SQR);}
"sqrt"      {return make_token_val(SYS_FUNCT,SQRT);}
"succ"      {return make_token_val(SYS_FUNCT,SUCC);}
"write"     {return make_token_val(SYS_PROC,WRITE);}
"writeln"   {return make_token_val(SYS_PROC,WRITELN);}
"boolean"   {return make_token_val(SYS_TYPE,BOOL);}
"char"      {return make_token_val(SYS_TYPE,CHAR);}
"integer"   {return make_token_val(SYS_TYPE,INT);}
"real"      {return make_token_val(SYS_TYPE,REAL);}
"string"    {return make_token_val(SYS_TYPE,STR);}
   
{real}      {return datatype_val(REAL);}
{interger}  {return datatype_val(INT);}
{id}        {return datatype_val(ID);}
["].*       {return datatype_val(STR);}
['].*       {return datatype_val(CHAR);}        
.*          {lex_error("wrong identifier");}

%%

int main()
{
    int tag = 1, num = 0;
    printf("scanner: \n");
    while(tag)
    {
        if(num = yylex())
        {
            printf("yylex() get %d chars\n", num);
            printtoken(yylval);
        }
        else
            tag = 0;
    }
}

int make_token_val(int tokentype, int tokenname)
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval-> token_type = tokentype;
    yylval-> token_name = tokenname;
    return tokenname;
}

int datatype_val(int token_name) //in case of literal constant or id
{
    yylval = (Token)malloc(sizeof(struct token_attri));
    yylval->token_type = token_name;
    char *str = yylval->token_val.token_string;
    int n = yyleng; 
    int j = 0;
    if(n > 32)
    {
        n = 32;
        lex_error("max-length is 32!");
        return -1;
    }
    switch(token_name)
    {
        case:REAL
            sscanf(yytext,"%lf", &(yylval->token_val.real_num));
            return CONST_REAL;
        case:INT
            sscanf(yytext,"%d", &(yylval->token_val.int_num));
            return CONST_INT;
        case:CHAR
            if(yytext[1] == '\\' && yytext[3] == '\'')
            {
                switch(yytext[2])
                {
                    case:'0'
                        ch = '\0';
                        break;
                    case:'n'
                        ch = '\n';
                        break;
                    case:'r'
                        ch = '\r';
                        break;
                    case:'t'
                        ch = '\t';
                        break;
                    default:
                        ch = yytext[2];
                }
            }
            else
            {
                if(yytext[2] == '\'')
                    ch = yytext[1];
                else
                    lex_error("wrong charactor!");
            }
            else
                lex_error("wrong charactor!");
            return CONST_CHAR;
        case:STR
            if(yytext[n-1] != '\"')
                lex_error("wring string!");
            for(int i = 1; i < n; i++)
            {
                if(yytext[i] != '\\')
                    str[j++] = yytext[i];
                else
                {
                    switch(yytext[++i])
                    {
                        case:'0'
                            ch = '\0';
                            break;
                        case:'n'
                            ch = '\n';
                            break;
                        case:'r'
                            ch = '\r';
                            break;
                        case:'t'
                            ch = '\t';
                            break;
                        default:
                            str[j++] = yytext[i];
                    }
                }
            }
            str[j] = '\0';
            return CONST_STR;
        case:ID
            strcpy(str, yytext);
            return ID;
    }
    return tokenname;
}

void printtoken(Token yylval) //displaying the result of lex
{

}

void lex_error(const char* error_info)
{
    printf("error in line %d: %s\n", lineno, error_info);
    exit(1);
}

void comment() //ignore the comment
{
    char c;
    while((c = getchar()) != '\n');
}
