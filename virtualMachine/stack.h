#ifndef __STACK_H__
#define __STACK_H__

typedef struct Node // 节点
{ 
    int data;
    struct Node *pNext;
} * PNODE, NODE;

typedef struct Stack // 栈
{ 
    PNODE pTop;
    PNODE pBottom;
} V_STACK, *PSTACK;

// 栈的初始化
void stackInit(PSTACK);
//压栈
void stackPush(PSTACK, int);
//出栈
int stackPop(PSTACK, int *);
//遍历打印栈
void stackTraverse(PSTACK);
//是否为空栈
int stackIsEmpty(PSTACK);
//清空栈
void clearStack(PSTACK);

#endif // ! STACK.H