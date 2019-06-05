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
#include "vmachine.h"

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
    stackInit(&CODE);      // 初始化CODE，用于存放汇编代码
    stackInit(&DATA);      // 初始化DATA，用于存放初始化过的数据
    stackInit(&STACK);     // 初始化STACK， 用于处理函数调用相关数据

    BP = SP = (int *)(int)STACK;
    ax = 0;
}