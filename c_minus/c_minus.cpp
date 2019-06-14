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

void open_src()
{
    int fd, n;

    if( ( fd = open(*argv, 0) ) < 0) // TODO Fix it: *argv?
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

void init_symtab() //put build-in function into symtab
{
    int j = OPEN;
    for (int i = 0; i < 8; i++)
    {
        int hash = hash_str(sys[i]);   // TODO Fix it: temp?
        symtab[0][hash].Class = Sys;    //system function
        symtab[0][hash].Type = INT;     //variable data type or function return type
        symtab[0][hash].In_value = j++; //build-in function type
        strcpy(symtab[0][hash].Name, sys[i]);
    }
    //TODO"void" and "main"
    //TODO add system function source
}

int main(int argc, char **argv)
{
    argc--;
    argv++;
    open_src();
    init_symtab();
    program();
    //TODO initiate stack and code area
}
