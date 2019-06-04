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
extern char **reserve;

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
//func_decl : type {'*'} id '(' func_para ')' '{' body_decl '}'
void glo_decl()
{
    if (token == Enum) //enum part
    {
        next();
        is_decl = true;
        enum_decl();
        is_decl = false;
        match('}');
        return;
    }

    //function or variable part
    if (token == Int)
    {
        decl_type = INT;
        match(Int);
        next();
    }
    else if (token == Double)
    {
        decl_type = DOUBLE;
        match(Double);
        next();
    }
    else if (token == Char)
    {
        decl_type = CHAR;
        match(Char);
        next();
    }

    while (token == Mul) //use offset to mark pointer
    {
        match(Mul);
        next();
        decl_type = decl_type + PTR;
    }

    if (token != Id)
    {
        cout << "function or variable declaration error! lack identifier in line "
             << lineno << endl;
        exit(1);
    }
    is_decl = true;
    match(Id);
    next(); //function name or variable name

    if (token == '(') //function
    {
        symtab[0][Hash].Class = Func;               //'(' didn't change hash of id
        symtab.push_back(unordered_map<int, ID>{}); //enter scope of the function
        match('(');
        next();
        func_para();
        match(')');
        next();
        match('{');
        next();
        func_body();
        match('}');
        symtab.pop_back(); //leave scope
    }
    else //global variable declaration
    {
        symtab[0][Hash].Class = Var;
        if (token == Assign) //assign is not necessary
        {
            next();
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
            else //ptr type
            {
                cout << "ptr assign is not allowed in decaration part in line "
                     << lineno << endl;
                exit(1);
            }
        }
        while (token != ';')
        {
            match(',');
            next();
            match(Id);
            next();
            symtab[0][Hash].Class = Var;
            if (token == Assign) //assign is not necessary
            {
                match(Assign);
                next();
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
                else //ptr type
                {
                    cout << "ptr assign is not allowed in decaration part in line "
                         << lineno << endl;
                    exit(1);
                }
            }
        }
        match(';');
    }
    is_decl = false;
}

//enum_decl : 'enum' {id} '{' id '='num {',' id '='num} '}'
void enum_decl()
{
    decl_type = INT; //default type for enum is INT
    if (token == Id) //ignore the enum type name
        next();
    match('{');
    next();
    while (token != '}')
    {
        match(Id);
        next();
        match(Assign);
        next();
        match(Con_Int);
        next();
        symtab[0][Hash].In_value = token_in_val;
        symtab[0][Hash].Class = Var;
        match(',');
        next();
    }
}

//func_para : type {'*'} id {',' type {'*'} id}
void func_para()
{
    if (token == Int)
    {
        match(INT);
        next();
    }
    else if (token == Char)
    {
        match(Char);
        next();
    }
    else if (token == Double)
    {
        match(Double);
        next();
    }
    else
    {
        cout << "unknowm type in line " << lineno << endl;
        exit(1);
    }
    decl_type = token;

    while (token == Mul)
    {
        decl_type += PTR;
        next();
    }
    match(Id);
    next();
    while (token != ')')
    {
        match(',');
        next();
        if (token == Int)
        {
            match(INT);
            next();
        }
        else if (token == Char)
        {
            match(Char);
            next();
        }
        else if (token == Double)
        {
            match(Double);
            next();
        }
        else
        {
            cout << "unknowm type in line " << lineno << endl;
            exit(1);
        }
        decl_type = token;
        while (token == Mul)
        {
            decl_type += PTR;
            next();
        }
        match(Id);
        next();
    }
}

void func_body()
{
    if (token == '{') //enter a scope
    {
        symtab.push_back(unordered_map<int, ID>{});
        next();
        func_body();
        match('}');
        symtab.pop_back(); //leave scope
    }

    //local declarations
    while (token == Int || token == Char || token == Double)
    {
        decl_type = token;
        next();
        while (token != ';')
        {
            while (token == Mul)
            {
                decl_type += PTR;
                next();
            }
            match(Id);
            next();
            symtab.back()[Hash].Class = Var;
            if (token == Assign) //assign while declaration
            {
                next();
                if (decl_type == Double)
                {
                    match(Con_Double);
                    next();
                    symtab.back()[Hash].D_value = token_d_val;
                }
                else if (decl_type == Int)
                {
                    match(Con_Int);
                    next();
                    symtab.back()[Hash].In_value = token_in_val;
                }
                else if (decl_type == Char)
                {
                    match(Con_Char);
                    next();
                    symtab.back()[Hash].In_value = token_in_val;
                }
                else if (decl_type > PTR)
                {
                    cout << "ptr is defaultly assign as NULL in line "
                         << lineno << endl;
                }
            }
        }
        next();
    }
}