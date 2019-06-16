#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
#include "vmachine.h"
using namespace std;

extern int token;
extern int lineno;
extern int decl_type;
extern int Hash;
extern bool is_decl;
extern int token_in_val;
extern double token_d_val;
extern vector<unordered_map<int, ID>> symtab;
extern char *reserve[12];

// match this character then go next
void match(int tk)
{
    if (token == tk) {
        next();
    }
    else
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
        match(Enum);
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

    while (token == Mul) //use offset to mark pointer
    {
        match(Mul);
        decl_type = decl_type + PTR;
    }

    if (token != Id)
    {
        cout << "function or variable declaration error! lack identifier in line "
             << lineno << endl;
        exit(1);
    }
    is_decl = true;
    match(Id); //function name or variable name

    if (token == '(') //function
    {
        symtab[0][Hash].Class = Func;               //'(' didn't change hash of id
        symtab.push_back(unordered_map<int, ID>{}); //enter scope of the function
        match('(');
        func_para();
        match(')');
        match('{');
        func_body();
        match('}');
        symtab.pop_back(); //leave scope
    }
    else //global variable declaration
    {
        symtab[0][Hash].Class = Var;
        if (token == Assign) //assign is not necessary
        {
            match(Assign);
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
        else if (token == '[')
        {
            int temp = Hash; //store the last id
            match('[');
            symtab[0][Hash].Type += PTR;
            if (token == Con_Int)
            {
                match(Con_Int);
                symtab[0][Hash].len = token_in_val; //mark array size
            }
            else if (token == Id)
            {
                if (symtab[0][Hash].Type != Int || symtab[0][Hash].In_value == 0)
                {
                    cout << "array size erro! in line " << lineno << endl;
                    exit(1);
                }
                match(Id);
                symtab[0][temp].len = symtab[0][Hash].In_value;
            }
            match(']');
        }
        while (token != ';')
        {
            match(',');
            match(Id);
            symtab[0][Hash].Class = Var;
            if (token == Assign) //assign is not necessary
            {
                match(Assign);
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
            else if (token == '[')
            {
                int temp = Hash;//store the last id
                match('[');
                symtab[0][Hash].Type += PTR;
                if(token == Con_Int)
                {
                    match(Con_Int);
                    symtab[0][Hash].len = token_in_val; //mark array size
                }
                else if(token == Id)
                {
                    if (symtab[0][Hash].Type != Int || symtab[0][Hash].In_value == 0)
                    {
                        cout << "array size erro! in line " << lineno << endl;
                        exit(1);
                    }
                    match(Id);
                    symtab[0][temp].len = symtab[0][Hash].In_value;
                }
                match(']');
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
        match(Id);
    match('{');
    match(Id);
    match(Assign);
    match(Con_Int);
    symtab[0][Hash].In_value = token_in_val;
    symtab[0][Hash].Class = Var;
    symtab[0][Hash].Type = Con_Int;
    while (token != '}')
    {
        match(',');
        match(Id);
        match(Assign);
        match(Con_Int);
        symtab[0][Hash].In_value = token_in_val;
        symtab[0][Hash].Class = Var;
    }
    match('}');
}

//var_decl : type {'*'} id {'=' num}{ ',' id {'=' num}} ';'
//TODO: add variable to stack
void var_decl()
{
    decl_type = token;
    match(token);
    while (token == Mul)
    {
        match(Mul);
        decl_type += PTR;
    }
    symtab.back()[Hash].Class = Var;
    if (token == Assign) //assign while declaration
    {
        match(Assign);
        if (decl_type == Double)
        {
            match(Con_Double);
            symtab.back()[Hash].D_value = token_d_val;
        }
        else if (decl_type == Int)
        {
            match(Con_Int);
            symtab.back()[Hash].In_value = token_in_val;
        }
        else if (decl_type == Char)
        {
            match(Con_Char);
            symtab.back()[Hash].In_value = token_in_val;
        }
        else if (decl_type > PTR)
        {
            cout << "ptr is defaultly assign as NULL in line "
                 << lineno << endl;
            exit(1);
        }
    }
    else if (token == '[')
    {
        int temp = Hash; //store the last id
        match('[');
        symtab.back()[Hash].Type += PTR;
        if (token == Con_Int)
        {
            match(Con_Int);
            symtab.back()[Hash].len = token_in_val; //mark array size
        }
        else if (token == Id)
        {
            if (symtab.back()[Hash].Type != Int || symtab.back()[Hash].In_value == 0)
            {
                cout << "array size erro! in line " << lineno << endl;
                exit(1);
            }
            match(Id);
            symtab.back()[temp].len = symtab.back()[Hash].In_value;
        }
        match(']');
    }
    while (token != ';')
    {
        match(',');
        match(Id);
        symtab.back()[Hash].Class = Var;
        if (token == Assign) //assign while declaration
        {
            match(Assign);
            if (decl_type == Double)
            {
                match(Con_Double);
                symtab.back()[Hash].D_value = token_d_val;
            }
            else if (decl_type == Int)
            {
                match(Con_Int);
                symtab.back()[Hash].In_value = token_in_val;
            }
            else if (decl_type == Char)
            {
                match(Con_Char);
                symtab.back()[Hash].In_value = token_in_val;
            }
            else if (decl_type > PTR)
            {
                cout << "ptr is defaultly assign as NULL in line "
                     << lineno << endl;
                exit(1);
            }
        }
        else if (token == '[')
        {
            int temp = Hash; //store the last id
            match('[');
            symtab.back()[Hash].Type += PTR;
            if (token == Con_Int)
            {
                match(Con_Int);
                symtab.back()[Hash].len = token_in_val; //mark array size
            }
            else if (token == Id)
            {
                if (symtab.back()[Hash].Type != Int || symtab.back()[Hash].In_value == 0)
                {
                    cout << "array size erro! in line " << lineno << endl;
                    exit(1);
                }
                match(Id);
                symtab.back()[temp].len = symtab.back()[Hash].In_value;
            }
            match(']');
        }
    }
    match(';');
}

//func_para : type {'*'} id {',' type {'*'} id}
//TODO add parameters to function stack
//TODO add pass like int a[]
void func_para()
{
    if (token == Int)
        match(Int);
    else if (token == Char)
        match(Char);
    else if (token == Double)
        match(Double);
    else
    {
        cout << "unknowm type in line " << lineno << endl;
        exit(1);
    }
    decl_type = token;

    while (token == Mul)
    {
        match(Mul);
        decl_type += PTR;
    }
    match(Id);
    while (token != ')')
    {
        match(',');
        if (token == Int)
            match(INT);
        else if (token == Char)
            match(Char);
        else if (token == Double)
            match(Double);
        else
        {
            cout << "unknowm type in line " << lineno << endl;
            exit(1);
        }
        decl_type = token;

        while (token == Mul)
        {
            match(Mul);
            decl_type += PTR;
        }
        match(Id);
    }
}

//func_body : {var_decl} {stmt}
//TODO add function body to code area
void func_body()
{
    if (token == '{') //enter a nest scope
    {
        symtab.push_back(unordered_map<int, ID>{});
        match('{');
        func_body();
        symtab.pop_back(); //leave scope
        match('}');
    }

    //local declarations
    while (token == Int || token == Char || token == Double)
    {
        var_decl();
        if (token == '{') //enter a nest scope
        {
            symtab.push_back(unordered_map<int, ID>{});
            match('{');
            func_body();
            symtab.pop_back(); //leave scope
            match('}');
        }
    }

    //statements
    is_decl = false;
    while (token != '}')
        stmt();
}

//stmt : empty_stmt | no_empty_stmt
//no_empty_stmt : if_stmt | while_stmt | '{' stmt '}' | 'return' exp ';' | exp ';'
void stmt()
{
    if (token == If) //if stmt
    {
        match(If);
        match('(');
        exp(Assign); //control sentence
        match(')');
        stmt(); //in case of true
        if (token == Else)
        {
            match(Else);
            stmt(); //in case of false
        }
    }
    else if (token == While) //while loop
    {
        match(While);
        match('(');
        exp(Assign);
        match(')');
        stmt();
    }
    else if (token == '{') //nest scope
    {
        symtab.push_back(unordered_map<int, ID>{});
        match('{');
        while (token != '}')
            stmt();
        symtab.pop_back(); //leave scope
        match('}');
    }
    else if (token == Return) //return stmt
    {
        match(Return);
        if (token != ';')
            exp(Assign);
        match(';');
    }
    else if (token == ';') //empty stmt
    {
        match(';');
    }
    else if (token == Int || token == Double || token == Char) //decl_stmt
    {
        is_decl = true;
        var_decl();
        is_decl = false;
    }
    else //expression
    {
        exp(Assign);
        match(';');
    }
}
