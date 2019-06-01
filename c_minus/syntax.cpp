#include <iostream>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
using namespace std;

extern int token;
extern int lineno;
extern int layer;
extern int decl_type; //type of a declaration
extern int Hash;//hash of the token
extern unordered_map<int, ID> symtab;

void match(int tk)
{
    if(token == tk)
        next();
    else
    {
        cout << "syntax error in line " << lineno << endl;
        exit(1);
    }
}


//program : {glo_decl}+
void program()
{
    next();
    while(token > 0)
        glo_decl();
}

//glo_decl : enum_decl | var_decl | func_decl
//enum_decl : 'enum' id '{' id '='num {',' id '='num} '}'
//var_decl : type {'*'} id { ',' {'*'} id} ';'
//func_decl : type {'*'} id '(' para_decl ')' '{' body_decl '}'
void glo_decl()
{
    int type, i;

    if (token == Enum)
    {
        match(Enum);
        if (token != '{')
            match(Id);
        else
        {
            match('{');
            enum_decl();
            match('}');
        }
        match(';');
    }

    else if (token == Int)
    {
        decl_type = INT;
        match(Int);
    }
    else if (token == Double)
    {
        decl_type = DOUBLE;
        match(Double);
    }
    else if (token == Char)
    {
        decl_type = CHAR;
        match(Char);
    }

    while(token != ';' && token != '}')//end of var_decl or func_decl
    {
        while(token == Mul)//use offset to mark pointer
        {
            match(Mul);
            decl_type = decl_type + PTR;
        }

        if(token != Id)
        {
            cout << "function or variable declaration error! lack identifier in line " 
            << lineno << endl; 
            exit(1);
        }
        match(Id);

        if(token == '(')
        {
            symtab[Hash].info.back().Class = Func;
            symtab[Hash].info.back().In_value  = int(text+1);
            //fill function memory
            func_decl();
        }
        else
        {
            symtab[Hash].info.back().Class = Var;
            symtab[Hash].info.back().In_value = int(data);
            //fill data memory
            data = data + sizeof(int);
        }

        if(token == ',')
            match(',');
    }
    next();
}

//enum_decl : 'enum' id '{' id '='num {',' id '='num} '}'
void enum_decl()
{
    int i = 0;
    if(token != Id)
    {
        cout << "add a type name for enum in line " 
            << lineno << endl;
        exit(1);
    }
    next();
    match('{')                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    while(token != '}')
    {
        if(token != Id)

    }
}