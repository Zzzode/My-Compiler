#include <vector>
using namespace std;
//headfile of the compiler

#define BUFSIZE (1024 * 1024)

// instructions of virtual machine
enum { 
  LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
  OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
  OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT 
};

// tokens and classes ,start from 128 to avoid system char
enum {
  Int = 128, Double, Char, Str, //these are types for literal constant
  Fun, Sys, Glo, Loc, Id,
  Else, Enum, If, Return, Sizeof, While,
  Assign, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, 
  Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec
};

// type marks of variable/function
//char and ptr value can be store as int
enum { CHAR, INT, DOUBLE, PTR };

struct id_info{
  int Type;//data type
  int In_value;//value of int type or address
  double D_value; // value of double type
  id_info(){}
  id_info(int a, int b): Type(a), In_value(b){}
  id_info(int a, double b): Type(a), D_value(b){}
};

// infos of an identifier
struct ID{
    int Token;//type of the word, id or sys or...
    int Hash;//hash of the word
    const char* Name;//id name string
    vector<id_info> info;//info[0] means global scope, the larger index , the deeper scope
};

//calculat the hash value of an id
int hash_str(char* s);

//get the next token
void next();