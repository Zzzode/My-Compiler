//main function
#include <iostream>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
using namespace std;

//symbal table, using hash to find an id quickly
//hash value is key, ID info is the value
//symtab.back() is the current layer
//search id from back to begin
//initiate only one global layer
//push back when enter a local area
//pop back when leave a local area
vector<unordered_map<int, ID>> symtab(1);

int token;           //type of the next token
int layer = 0;       //scope of id, 0 means global, the larger, the deeper
int lineno = 1;      //line number of program
int decl_type;       //type of the whole declaration
int Hash;            //mark the cuurent id
bool is_decl = true; //when lexical parse an id, judge legal or not
//if in decl mode, new id can be add into symtab, else illegal

int token_in_val = 0;   //literal int value
double token_d_val = 0; //literal double value
//notice we onlt deal with dec int and double

char token_str_val[64]; //store literal constant of string
char *src;              //buffer of file input

char *reserve[9] = {"char", "int", "double", "else", "enum",
                    "if", "return", "sizeof", "while"};

char *sys[8] = {"open", "read", "close", "printf",
                "malloc", "memset", "memcmp", "exit"};

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

void init_symtab() //put build-in function into symtab
{
    int j = OPEN;
    for (int i = 0; i < 8; i++)
    {
        int hash = hash_str(temp[i]);
        symtab[0][hash].Token = Id;     //store function address
        symtab[0][hash].Class = Sys;    //system function
        symtab[0][hash].Type = INT;     //variable data type or function return type
        symtab[0][hash].In_value = j++; //build-in function type
        strcpy(symtab[0][hash].Name, temp[i]);
    }

    //not sure how to handle "void" and "main"
}

int main(int argc, char **argv)
{
    argc--;
    argv++;
    open_src();
    init_symtab();
    program();
}