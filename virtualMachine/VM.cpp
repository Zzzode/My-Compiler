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

#include "VM.h"

int main(int argc, char **argv)
{
    int fd;
    line = 1;              // 首行标号为1

    // 读取命令行的文件名，并打开（Linux操作）
    // TODO 添加Windows下的文件操作
    if ((fd = open(argv[1], 0)) < 0)
    {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    initVirtulMachine() ；
}

/** 
 * @Author: Zzzcode 
 * @Desc:  虚拟机内存空间初始化
 */
void initVirtulMachine()
{
    poolsize = 256 * 1024; // 默认内存区域大小为256*1024

    // 给虚拟机分配空间
    if (!(CODE = malloc(poolsize)))
    {
        printf("could not malloc(%d) for code area\n", poolsize);
        exit(1);
    }
    if (!(DATA = malloc(poolsize)))
    {
        printf("could not malloc(%d) for data area\n", poolsize);
        exit(1);
    }
    if (!(STACK = malloc(poolsize)))
    {
        printf("could not malloc(%d) for stack area\n", poolsize);
        exit(1);
    }
    if (!(symbols = malloc(poolsize)))
    {
        printf("could not malloc(%d) for symbol table\n", poolsize);
        exit(1);
    }

    memset(CODE, 0, poolsize);
    memset(DATA, 0, poolsize);
    memset(STACK, 0, poolsize);
    memset(symbols, 0, poolsize);

    BP = SP = (int *)((int)STACK + poolsize);
    ax = 0;
}