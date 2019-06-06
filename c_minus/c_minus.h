#ifndef _C_MINUS_H
#define _C_MINUS_H

#define BUFSIZE 1048576//1024 * 1024

// instructions of virtual machine
enum { 
  LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
  OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
  OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT //system fuction
};

// tokens and classes ,start from 128 to avoid system char
enum {
  Con_Int = 128, Con_Double, Con_Char, Con_Str, //types for literal constant
  Func, Sys, Var, Id,//class

  Char, Int, Double, Else, Enum, If, 
  Return, Sizeof, While, Lan, Lor, Not,//reserve words
  
  Assign, Xor, Eq, And, Or, Ne, Lt, Gt, Le, Ge, 
  Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec
};

//var type
//char and ptr value can be store as int
enum { CHAR, INT, DOUBLE, PTR };

// infos of an identifier
struct ID{
  int Class;            //function or variable
  int Type;             //type of variable, for function is INT
  //TODO void* addr;
  int len;              //array size
  int In_value;         //value of int type or address or char
  double D_value;       //value of double type
  char Name[64];        //id name string, max length is 63
};

//get the next token
void next();

//match charactor
void match(int tk);

void program();
//start scanning program
//program : {glo_decl}+

void glo_decl();
//glo_decl : enum_decl | var_decl | func_decl

void enum_decl();
//enum_decl : 'enum' id '{' id '=''num' {',' id '=''num'} '}'

void var_decl();
//var_decl : type {'*'} id {'=' num}{ ',' id {'=' num}} ';'

void func_decl();
//func_decl : type {'*'} id '(' func_para ')' '{' func_body '}'

void func_para();
//func_para : type {'*'} id {',' type {'*'} id}

void func_body();
//func_body : {var_decl} { stmt }

void stmt();
//stmt : empty_stmt | no_empty_stmt
//no_empty_stmt : if_stmt | while_stmt 
//                |'{' stmt '}' | 'return' exp ';'| exp ';'

void exp();



#endif // !_C_MINUS_H