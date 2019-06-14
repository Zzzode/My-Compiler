/** 
 * @Author: Zzzcode 
 * @Date: 2019-06-05 23:23:15 
 * @Desc: 链表型栈结构，用于虚拟机堆栈
 */
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// 栈的初始化
void stackInit(PSTACK pS)
{
    pS->pTop = (PNODE)malloc(sizeof(NODE));
    if (pS->pTop == NULL)
    {
        printf("内存分配失败!!");
        exit(1);
    }
    else
    {
        pS->pBottom = pS->pTop;
        pS->pTop->pNext = NULL;
    }
}

/**
 压栈
 @param pS 执行压栈的栈指针
 @param val 被压栈的值
 */
void stackPush(PSTACK pS, int val)
{
    // 创建新节点，放到栈顶
    PNODE pNew = (PNODE)malloc(sizeof(NODE));
    pNew->data = val;
    pNew->pNext = pS->pTop;

    pS->pTop = pNew; // 栈顶指针指向新元素
}

/**
 出栈
 @param pS 执行出栈的栈地址
 @param val 出栈值的地址
 @return 是否出栈成功
 */
int stackPop(PSTACK pS, int *val)
{
    if (stackIsEmpty(pS))
    {
        printf("空栈 ，出栈失败");
        return 0;
    }
    else
    {
        PNODE p = pS->pTop;
        pS->pTop = p->pNext;

        if (val != NULL)
        {
            *val = p->data;
        }
        free(p); // 释放原来top内存
        p = NULL;
        return 1;
    }
}

// 是否为空栈
int stackIsEmpty(PSTACK pS)
{
    if (pS->pTop == pS->pBottom)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**清空栈*/
void clearStack(PSTACK pS)
{

    if (stackIsEmpty(pS))
    {
        return;
    }
    else
    {
        PNODE p = pS->pTop;
        PNODE q = NULL;

        while (p != pS->pBottom)
        {
            q = p->pNext;
            free(p); // 释放原栈顶元素空间
            p = q;
        }
        pS->pTop = pS->pBottom;
    }
}

// 遍历打印栈
void stackTraverse(PSTACK pS)
{
    if (stackIsEmpty(pS))
    {
        printf("空栈！");
        return;
    }

    // 只要不是空栈，就一直输出
    PNODE p = pS->pTop;
    while (p != pS->pBottom)
    {
        printf("%d ", p->data);
        p = p->pNext; // 把top的下一个节点付给top，继续遍历
    }
    printf("\n");
}

// 测试用，测试完注释掉
/* int main(void)
{
    V_STACK stack;  // 声明一个栈
    stackInit(&stack); // 初始化

    // 压栈
    stackPush(&stack, 10);
    stackPush(&stack, 20);
    stackPush(&stack, 30);
    stackPush(&stack, 40);
    stackPush(&stack, 50);

    stackTraverse(&stack); // 遍历打印栈

    int val;
    int isPopSuccess = stackPop(&stack, &val);
    if (isPopSuccess)
    {
        printf("pop 的值为 %d\n", val);
    }

    stackTraverse(&stack);

    clearStack(&stack); // 清空栈
    stackTraverse(&stack);

    return 0;
} */