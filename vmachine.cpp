/**
 * @Author: Zzzcode
 * @Date: 2019-05-27 18:08:48
 * @Desc: 汇编代码虚拟机，实现并运行汇编代码
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vmachine.h"

using namespace std;

int debug;                         // print the executed instructions
int assembly;                      // print out the assembly and source
int poolsize;                      // 内存区域大小
// int line;                          // 行号

int index_of_bp;                   // 栈bp指针的index

int *text,                         // text segment
        *old_text,                     // for dump text segment
        *stack;                        // stack

char *data;                        // data segment

int *PC, *BP, *SP, AX, cycle;      // 寄存器
int *current_Id, *symbols;         // ID和符号表
int *ID_MAIN;                      // main函数

char *src, *oldsrc;                // 指向代码字符串的指针
char *DATA;                        // 数据段

/**
 * @Author: Zzzcode
 * @Desc:  虚拟机内存空间初始化
 */
void initVirtulMachine()
{
    poolsize = 256 * 1024; // 默认内存区域大小为256*1024

    // allocate memory for virtual machine
    // 给虚拟机分配空间
    if (!(text = (int*)malloc(poolsize))) {
        printf("could not malloc(%d) for text area\n", poolsize);
        exit(1);
    }
    if (!(data = (char*)malloc(poolsize))) {
        printf("could not malloc(%d) for data area\n", poolsize);
        exit(1);
    }
    if (!(stack = (int*)malloc(poolsize))) {
        printf("could not malloc(%d) for stack area\n", poolsize);
        exit(1);
    }
    if (!(symbols = (int*)malloc(poolsize))) {
        printf("could not malloc(%d) for symbol table\n", poolsize);
        exit(1);
    }

    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);
    memset(symbols, 0, poolsize);

    old_text = text;

    BP = SP = (int *)((int* )stack + poolsize);
    AX = 0;

    src = "char else enum if int return sizeof while "
          "open read close printf malloc memset memcmp exit void main";
}

// 虚拟机
int eval() {
    int op, *tmp;
    cycle = 0;
    while (1) {
        cycle ++;
        op = *PC++; // get next operation code

        // print debug info
        if (debug) {
            printf("%d> %.4s", cycle,
                   & "LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,"
                     "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                     "OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT"[op * 5]);
            if (op <= ADJ)
                printf(" %d\n", *PC);
            else
                printf("\n");
        }
        if (op == IMM)       {AX = *PC++;}                                     // load immediate value to AX
        else if (op == LC)   {AX = *(char *)AX;}                               // load character to AX, address in AX
        else if (op == LI)   {AX = *(int *)AX;}                                // load integer to AX, address in AX
        else if (op == SC)   {AX = *(char *)*SP++ = AX;}                       // save character to address, value in AX, address on stack
        else if (op == SI)   {*(int *)*SP++ = AX;}                             // save integer to address, value in AX, address on stack
        else if (op == PUSH) {*--SP = AX;}                                     // push the value of AX onto the stack
        else if (op == JMP)  {PC = (int *)*PC;}                                // jump to the address
        else if (op == JZ)   {PC = AX ? PC + 1 : (int *)*PC;}                   // jump if AX is zero
        else if (op == JNZ)  {PC = AX ? (int *)*PC : PC + 1;}                   // jump if AX is not zero
        else if (op == CALL) {*--SP = (long long)(PC+1); PC = (int *)*PC;}           // call subroutine
            //else if (op == RET)  {PC = (int *)*SP++;}                              // return from subroutine;
        else if (op == ENT)  {*--SP = (long long)BP; BP = SP; SP = SP - *PC++;}      // make new stack frame
        else if (op == ADJ)  {SP = SP + *PC++;}                                // add eSP, <size>
        else if (op == LEV)  {SP = BP; BP = (int *)*SP++; PC = (int *)*SP++;}  // restore call frame and PC
        else if (op == LEA)  {AX = (long long)(BP + *PC++);}                         // load address for arguments.

        else if (op == OR)  AX = *SP++ | AX;
        else if (op == XOR) AX = *SP++ ^ AX;
        else if (op == AND) AX = *SP++ & AX;
        else if (op == EQ)  AX = *SP++ == AX;
        else if (op == NE)  AX = *SP++ != AX;
        else if (op == LT)  AX = *SP++ < AX;
        else if (op == LE)  AX = *SP++ <= AX;
        else if (op == GT)  AX = *SP++ >  AX;
        else if (op == GE)  AX = *SP++ >= AX;
        else if (op == SHL) AX = *SP++ << AX;
        else if (op == SHR) AX = *SP++ >> AX;
        else if (op == ADD) AX = *SP++ + AX;
        else if (op == SUB) AX = *SP++ - AX;
        else if (op == MUL) AX = *SP++ * AX;
        else if (op == DIV) AX = *SP++ / AX;
        else if (op == MOD) AX = *SP++ % AX;

        else if (op == EXIT) {
            printf("exit(%d)", *SP);
            return *SP;
        }
        else if (op == OPEN) { AX = open((char *)SP[1], SP[0]); }
        else if (op == CLOS) { AX = close(*SP);}
        else if (op == READ) { AX = read(SP[2], (char *)SP[1], *SP); }
        else if (op == PRTF) { tmp = SP + PC[1]; AX = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
        else if (op == MALC) { AX = (long long)malloc(*SP);}
        else if (op == MSET) { AX = (long long)memset((char *)SP[2], SP[1], *SP);}
        else if (op == MCMP) { AX = memcmp((char *)SP[2], (char *)SP[1], *SP);}
        else {
            printf("unknown instruction:%d\n", op);
            return -1;
        }
    }
}
