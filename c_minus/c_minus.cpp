//main function
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
using namespace std;

//symbal table, using hash to find an id quickly
// hash value is key, ID info is the value
//symbols[layer] means the 
unordered_map<int,ID> symtab; 

int token;//type of the next token
int layer = 0;//scope of id, 0 means global, the larger, the deeper
int lineno = 1;//line number of program
int decl_type;//type of a declaration
int token_in_val = 0;
double token_d_val = 0;//notice we onlt deal with dec int and double
char token_str_val[64];//store literal constant of string
char *src;//buffer of file input

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
    if((fd = open(*argv, 0) < 0)
    {
        cout << "couldn't open source file!\n";
        exit(1);
    }
    src = new char[BUFSIZE];
    if((n = read(fd,src,BUFSIZE)) < 0)
    {
        cout << "read file error\n";
        exit(1);
    }
    src[n] = '\0';//add EOF
    close(fd);
}


void init_symtab() //put the reserve words into symtab
{
    char *temp[9] = {"char", "int", "double", "else", "enum", 
                    "if", "return", "sizeof", "while"};
    int j = Char;
    for (int i = 0; i < 9; i++)
    {
        int hash = hash_str(temp[i]);
        symtab[hash].Token = j++;
        strcpy(symtab[hash].Name, temp[i]);
    }

    char *sys[8] = {"open", "read", "close", "printf",
                    "malloc", "memset", "memcmp", "exit"};
    int j = OPEN;
    for (int i = 0; i < 8; i++)
    {
        int hash = hash_str(temp[i]);
        symtab[hash].Token = Id;
        symtab[hash].info.push_back(id_info(INT,0,j++));
        symtab[hash].info[0].Class = Sys;
        strcpy(symtab[hash].Name, temp[i]);
    }

    //not sure how to handle "void" and "main" 
}

int main(int argc, char** argv)
{
    argc--;argv++;
    open_src();
    init_symtab();
    program();
}