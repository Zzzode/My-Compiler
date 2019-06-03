#ifndef _C_MINUS_H
#define _C_MINUS_H
#include <vector>
using namespace std;
//headfile of the compiler

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
  Func, Sys, Var, Id, Num,//class
  Char, Int, Double, Else, Enum, If, Return, Sizeof, While,//reserve words
  Assign, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, 
  Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec
};

//var type
//char and ptr value can be store as int
enum { CHAR, INT, DOUBLE, PTR };

// infos of an identifier
struct ID{
  int Token;            //id or reserve word
  int Class;            //function or variable
  int Type;             //type of variable, for function is INT
  int In_value;         //value of int type or address
  double D_value;       // value of double type
  char Name[64];        //id name string, max length is 63
};

//get the next token
void next();

//match charactor
void match(int tk);

//start scanning program
void program();
void glo_decl();
void enum_decl();
void var_decl();
void func_decl();
void para_decl();
void body_decl();
void stmt();
void exp();


#endif // !_C_MINUS_H