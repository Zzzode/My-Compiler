/**
 * @Author: Zzzcode
 * @Date: 2019-05-27 18:08:48
 * @Desc: 汇编代码虚拟机，实现并运行汇编代码
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vmachine.h"

/**
 * @Author: Zzzcode
 * @Desc:  虚拟机内存空间初始化
 */
void initVirtulMachine()
{
    poolsize = 256 * 1024; // 默认内存区域大小为256*1024

    // allocate memory for virtual machine
    // 给虚拟机分配空间
    if (!(text = old_text = malloc(poolsize)))
    {
        cout << "could not malloc " << poolsize << "for text area" << endl;
        exit(1);
    }
    if (!(data = malloc(poolsize)))
    {
        cout << "could not malloc " << poolsize << "for data area" << endl;
        exit(1);
    }
    if (!(stack = malloc(poolsize)))
    {
        cout << "could not malloc " << poolsize << "for stack area" << endl;
        exit(1);
    }
    if (!(symbols = malloc(poolsize)))
    {
        cout << "could not malloc " << poolsize << "for symbol table" << endl;
        exit(1);
    }

    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);
    memset(symbols, 0, poolsize);
    BP = SP = (int *)((int)stack + poolsize);
    AX = 0;

    src = "char else enum if int return sizeof while "
          "open read close printf malloc memset memcmp exit void main";
}

// 虚拟机
int eval()
{
    int op, *temp;
    while (1)
    {
        op = *PC++; // get next operation code

        if (op == IMM)
        {
            AX = *PC++;
        } // load immediate value to AX
        else if (op == LC)
        {
            AX = *(char *)AX;
        } // load character to AX, address in AX
        else if (op == LI)
        {
            AX = *(int *)AX;
        } // load integer to AX, address in AX
        else if (op == SC)
        {
            AX = *(char *)*SP++ = AX;
        } // save character to address, value in AX, address on stack
        else if (op == SI)
        {
            *(int *)*SP++ = AX;
        } // save integer to address, value in AX, address on stack
        else if (op == PUSH)
        {
            *--SP = AX;
        } // push the value of AX onto the stack
        else if (op == JMP)
        {
            PC = (int *)*PC;
        } // jump to the address
        else if (op == JZ)
        {
            PC = AX ? PC + 1 : (int *)*PC;
        } // jump if AX is zero
        else if (op == JNZ)
        {
            PC = AX ? (int *)*PC : PC + 1;
        } // jump if AX is not zero
        else if (op == CALL)
        {
            *--SP = (int)(PC + 1);
            PC = (int *)*PC;
        } // call subroutine
        //else if (op == RET)  {PC = (int *)*SP++;}                              // return from subroutine;
        else if (op == ENT)
        {
            *--SP = (int)BP;
            BP = SP;
            SP = SP - *PC++;
        } // make new stack frame
        else if (op == ADJ)
        {
            SP = SP + *PC++;
        } // add eSP, <size>
        else if (op == LEV)
        {
            SP = BP;
            BP = (int *)*SP++;
            PC = (int *)*SP++;
        } // restore call frame and PC
        else if (op == ENT)
        {
            *--SP = (int)BP;
            BP = SP;
            SP = SP - *PC++;
        } // make new stack frame
        else if (op == ADJ)
        {
            SP = SP + *PC++;
        } // add esp, <size>
        else if (op == LEV)
        {
            SP = BP;
            BP = (int *)*SP++;
            PC = (int *)*SP++;
        } // restore call frame and PC
        else if (op == LEA)
        {
            AX = (int)(BP + *PC++);
        } // load address for arguments.

        else if (op == OR)
            AX = *SP++ | AX;
        else if (op == XOR)
            AX = *SP++ ^ AX;
        else if (op == AND)
            AX = *SP++ & AX;
        else if (op == EQ)
            AX = *SP++ == AX;
        else if (op == NE)
            AX = *SP++ != AX;
        else if (op == LT)
            AX = *SP++ < AX;
        else if (op == LE)
            AX = *SP++ <= AX;
        else if (op == GT)
            AX = *SP++ > AX;
        else if (op == GE)
            AX = *SP++ >= AX;
        else if (op == SHL)
            AX = *SP++ << AX;
        else if (op == SHR)
            AX = *SP++ >> AX;
        else if (op == ADD)
            AX = *SP++ + AX;
        else if (op == SUB)
            AX = *SP++ - AX;
        else if (op == MUL)
            AX = *SP++ * AX;
        else if (op == DIV)
            AX = *SP++ / AX;
        else if (op == MOD)
            AX = *SP++ % AX;

        else if (op == EXIT)
        {
            printf("exit(%d)", *SP);
            return *SP;
        }
        else if (op == OPEN)
        {
            AX = open((char *)SP[1], SP[0]);
        }
        else if (op == CLOS)
        {
            AX = close(*SP);
        }
        else if (op == READ)
        {
            AX = read(SP[2], (char *)SP[1], *SP);
        }
        else if (op == PRTF)
        {
            temp = SP + PC[1];
            AX = printf((char *)temp[-1], temp[-2], temp[-3], temp[-4], temp[-5], temp[-6]);
        }
        else if (op == MALC)
        {
            AX = (int)malloc(*SP);
        }
        else if (op == MSET)
        {
            AX = (int)memset((char *)SP[2], SP[1], *SP);
        }
        else if (op == MCMP)
        {
            AX = memcmp((char *)SP[2], (char *)SP[1], *SP);
        }
        else
        {
            printf("unknown instruction:%d\n", op);
            return -1;
        }
    }
    return 0;
}
