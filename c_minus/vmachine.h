#ifndef _VMACHINE_H_
#define _VMACHINE_H_

#include "stack.h"
// #define int long long // 修改int长度为64位，用于存放地址

// int token, token_val;              // 当前token和token值
int poolsize;                      // 内存区域大小
int line;                          // 行号

int index_of_bp;                   // 栈bp指针的index

V_STACK CODE, STATIC, HEAP, STACK; // 四大内存区域

int *text,                         // text segment
    *old_text,                     // for dump text segment
    *stack;                        // stack

char *data;                        // data segment

int *PC, *BP, *SP, AX, cycle;      // 寄存器
int *current_Id, *symbols;         // ID和符号表
int *ID_MAIN;                      // main函数

char *src, *oldsrc;                // 指向代码字符串的指针
char *DATA;                        // 数据段

// 基础汇编指令集
enum instructions_e{
    LEA, IMM, JMP, CALL, JZ, JNZ, ENT,
    ADJ, LEV, LI,  LC,   SI, SC,  PUSH,
    OR,  XOR, AND, EQ,   NE, LT,  GT, LE, GE,
    SHL, SHR, ADD, SUB,  MUL,DIV, MOD,
    OPEN, READ, CLOS, PRTF,
    MALC, MSET, MCMP, EXIT
};

/* enum {
  LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
  OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,
  DIV ,MOD ,OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT //system fuction
}; */

// fields of identifier
enum identifier_e{
    Token, Hash,  Name,   Type,   Class,
    Value, BType, BClass, BValue, IdSize
};

// types of variable/function
/* enum variableTypes_e{
    CHAR, INT, PTR
};
 */
#endif // ! _VMACHINE_H_
