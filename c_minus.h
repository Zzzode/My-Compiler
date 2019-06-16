#ifndef _C_MINUS_H
#define _C_MINUS_H

#define BUFSIZE 1048576 // 1024 * 1024

extern int token;      //type of the token we got
extern int decl_type;  //type of the whole declaration
extern int exp_type;   //type of expression answer
extern int Hash;       //mark the cuurent id in lexical
extern int layer;      //mark which layer an id belongs to in symtab

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
    // int TOKEN;
    int Class;            //function or variable
    int Type;             //type of variable, for function is INT
    int* addr;
    int len;              //array size
    int In_value;         //value of int type or address or char
    double D_value;       //value of double type
    char Name[64];        //id name string, max length is 63
};

//get the next token
void next();

//match this charactor then go next
void match(int tk);

//start scanning program
//program : {glo_decl}+
void program();

//glo_decl : enum_decl | var_decl | func_decl
void glo_decl();

//enum_decl : 'enum' id '{' id '=''num' {',' id '=''num'} '}'
void enum_decl();

//var_decl : type {'*'} id {'=' num}{ ',' id {'=' num}} ';'
void var_decl();

//func_decl : type {'*'} id '(' func_para ')' '{' func_body '}'
// void func_decl();

//func_para : type {'*'} id {',' type {'*'} id}
void func_para();

//func_body : {var_decl} { stmt }
void func_body();

//stmt : empty_stmt | no_empty_stmt
//no_empty_stmt : if_stmt | while_stmt
//                |'{' stmt '}' | 'return' exp ';'| exp ';'
void stmt();

// exp
void exp(int level);


#endif // !_C_MINUS_H
