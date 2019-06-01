//main function
#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <algorithm>
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
int token_in_val = 0;
double token_d_val = 0;//notice we onlt deal with dec int and double
char token_str_val[64];//store literal constant of string
char * src;//buffer of file input


void init_symbol() //put the reserve words into symbol table
{
    
    int i = Char;//key words
    
}

int main(int argc, char** argv)
{
    argc--;argv++;
    
    init_symbol();
    
}