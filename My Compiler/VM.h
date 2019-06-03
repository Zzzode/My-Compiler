#ifndef __VM_H__
#define __VM_H__

#define int long long // 修改int长度为64位，用于存放地址

int token, token_val;              // 当前token和token值
int poolsize;                      // 内存区域大小
int line;                          // 行号
int baseType, exprType;            // 声明类型以及表达式类型
int index_of_bp;                   // 栈bp指针的index
int *CODE, *STATIC, *HEAP, *STACK; // 四大内存区域
int *PC, *BP, *SP, AX, cycle;      // 寄存器
int *current_Id, *Symbols;         // ID和符号表
int *ID_MAIN;                      // main函数

char *src, *oldsrc;                // 指向代码字符串的指针
char *DATA;                        // 数据段

// 基础汇编指令集
enum Instructions{
    LEA, IMM, JMP, CALL, JZ, JNZ, ENT,
    ADJ, LEV, LI,  LC,   SI, SC,  PUSH, 
    OR,  XOR, AND, EQ,   NE, LT,  GT, LE, GE,
    SHL, SHR, ADD, SUB,  MUL,DIV, MOD,
    OPEN, READ, CLOS, PRTF,
    MALC, MSET, MCMP, EXIT
};

// tokens and classes (operators last and in precedence order)
enum Tokens{
    Num = 128,
    Fun,  Sys,  Glo,  Loc, Id, 
    Char, Else, Enum, If,  Int, Return, Sizeof, While, 
    Assign,  Cond, Lor, Lan, Or,  Xor, And, Eq,  Ne,  Lt, Gt, Le,
    Ge, Shl, Shr,  Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

// fields of identifier
enum Identifier{
    Token, Hash,  Name,   Type,   Class,
    Value, BType, BClass, BValue, IdSize
};

// types of variable/function
enum Variable_Types{
    CHAR, INT, PTR
};

#endif // ! VM.h