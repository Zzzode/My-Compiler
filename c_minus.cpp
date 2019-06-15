// main function
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <unordered_map>
// system call about files
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
// user defile
#include "c_minus.h"
using namespace std;

/**
 * symbal table, using hash to find an id quickly
 * hash value is key, ID info is the value
 * symtab.back() is the current layer
 * search id from back to begin
 * initiate only one global layer
 * push back when enter a local area
 * pop back when leave a local area
 */
vector<unordered_map<int, ID>> symtab(1);

//calculat the hash value of an id
int hash_str(char *s)
{
    int ans = 0;
    while (*s != '\0')
        ans = ans * 147 + *s++;
    return ans;
}

void open_src(int fd, char **argv)
{
    int n;

    if( ( fd = open(*argv, 0) ) < 0)
    { // fail to open target file
        cout << "couldn't open source file!\n";
        exit(1);
    }
    src = new char[BUFSIZE];
    if((n = read(fd, src, BUFSIZE)) < 0)
    { // faile to read target file
        cout << "read file error\n";
        exit(1);
    }
    src[n] = '\0'; //add EOF
    close(fd);

    //TODO check buffer malloc, change to windows API
}

void init_symtab(int fd) //put build-in function into symtab
{
    int i;
    /* int i = Char;
    while (i <= While)
    {
        next();
        current_id[Token] = i++;
    }*/
    int j = OPEN;

    for (i = 0; i < 8; i++)
    {
        int hash = hash_str(sys[i]);
        symtab[0][hash].Class = Sys;    //system function
        symtab[0][hash].Type = INT;     //variable data type or function return type
        symtab[0][hash].In_value = j++; //build-in function type
        strcpy(symtab[0][hash].Name, sys[i]);
    }
    //TODO"void" and "main"
    //TODO add system function source

    // read the source file
    if ((fd = open(*argv, 0)) < 0)
    {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(poolsize)))
    {
        printf("could not malloc(%d) for source area\n", poolsize);
        return -1;
    }
    // read the source file
    if ((i = read(fd, src, poolsize - 1)) <= 0)
    {
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0; // add EOF character
    close(fd);
}

int main(int argc, char **argv)
{
    int fd;
    int *temp;

    argc--;
    argv++;

    open_src(fd, argv);
    initVirtulMachine();
    init_symtab(fd);

    program();

    // 如果没有main()函数
    if (!(pc = (int *)idmain[Value]))
    {
        cout << "main() not defined" << endl;
        exit(1);
    }
    // 初始化虚拟机的栈，当 main 函数结束时退出进程
    SP = (int *)((int)stack + poolsize);
    *--SP = EXIT; // call exit if main returns
    *--SP = PUSH;
    temp = SP;
    *--SP = argc;
    *--SP = (int)argv;
    *--SP = (int)tmp;

    return eval();
}
