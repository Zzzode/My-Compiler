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
  Func, Sys, Var, Id,
  Char, Int, Double, Else, Enum, If, Return, Sizeof, While,//reserve words
  Assign, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, 
  Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec
};

// type marks of variable/function
//char and ptr value can be store as int
enum { CHAR, INT, DOUBLE, PTR };

struct id_info{
  int Type;//data type
  int Layer;//scope of the id
  int Class;//system function or function
  int In_value;//value of int type or address
  double D_value; // value of double type
  id_info() : Type(0), Layer(0), In_value(0), D_value(0) {}
  id_info(int a, int b, int c): Type(a), Layer(b), In_value(c){}
  id_info(int a, int b, double c) : Type(a), Layer(b), D_value(c) {}
  id_info(int a, int b, int c, double d) : Type(a), Layer(b), In_value(c), D_value(d) {}
};

// infos of an identifier
struct ID{
    int Token;//id or reserve word
    char Name[64];//id name string, max length is 63
    vector<id_info> info;//info[0] means global scope, the larger index , the deeper scope
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