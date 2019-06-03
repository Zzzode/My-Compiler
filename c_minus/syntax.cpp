#include <iostream>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
using namespace std;

extern int token;
extern int lineno;
extern int layer;
extern int decl_type; 
extern int Hash;
extern bool is_decl;
extern int token_in_val;
extern double token_d_val;
extern vector<unordered_map<int, ID>> symtab;

void match(int tk)
{
    if (token != tk)
    {
        cout << tk << " unmatch in line " << lineno << endl;
        exit(1);
    }
}

//program : {glo_decl}+
void program()
{
    next();
    while (token > 0)
    {
        glo_decl();
        next();
    }
}

//glo_decl : enum_decl | var_decl | func_decl
//enum_decl : 'enum' id '{' id '='num {',' id '='num} '}'
//var_decl : type {'*'} id { ',' {'*'} id} ';'
//func_decl : type {'*'} id '(' para_decl ')' '{' body_decl '}'
void glo_decl()
{
    int type, i;

    if (token == Enum)//enum part
    {
        next();
        is_decl = true;
        enum_decl();
        is_decl = false;
        match('{');
        return;
    }

    //function or variable part
    if (token == Int)
    {
        decl_type = INT;
        match(Int);next();
    }
    else if (token == Double)
    {
        decl_type = DOUBLE;
        match(Double);next();
    }
    else if (token == Char)
    {
        decl_type = CHAR;
        match(Char);next();
    }

    while (token == Mul) //use offset to mark pointer
    {
        match(Mul);next();
        decl_type = decl_type + PTR;
    }

    if (token != Id)
    {
        cout << "function or variable declaration error! lack identifier in line "
             << lineno << endl;
        exit(1);
    }
    is_decl = true;
    match(Id); next(); //function name or variable name

    if (token == '(') //function
    {
        symtab[0][Hash].Class = Func;//'(' didn't change hash of id
        match('(');next();
        para_decl();
        match(')');next();
        match('{');next();
        body_decl();
        match('}');
    }
    else //variable, global declaration
    {
        var_decl();
        while (token != ';')
        {
            match(',');next();
            match(Id);next();
            var_decl();
        }
        match(';');
    }
    is_decl = false;
}

//enum_decl : 'enum' {id} '{' id '='num {',' id '='num} '}'
void enum_decl()
{
    decl_type = INT;//default type for enum is INT
    if (token == Id) //ignore the enum type name
        next();
    match('{');next();
    while (token != '}')
    {
        match(Id);next();
        match(Assign);next();
        match(Con_Int);next();
        symtab[0][Hash].In_value = token_in_val;
        symtab[0][Hash].Class = Var;
        match(',');next();
    }
}

void var_decl()
{
    symtab[0][Hash].Class = Var;
    if (token == Assign) //assign is not necessary
    {
        match(Assign);next();
        if (decl_type == DOUBLE)
        {
            match(Con_Double);
            symtab[0][Hash].D_value = token_d_val;
        }
        else if (decl_type == INT)
        {
            match(Con_Int);
            symtab[0][Hash].In_value = token_in_val;
        }
        else if (decl_type == CHAR)
        {
            match(Con_Char);
            symtab[0][Hash].In_value = token_in_val;
        }
        else
        {
            cout << "ptr assign is not allowed in decaration in line "
                 << lineno << endl;
            exit(1);
        }
    }
}
