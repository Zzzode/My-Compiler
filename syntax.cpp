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
extern long long Hash;
extern bool is_decl;
extern int token_val;
extern double token_d_val;
extern vector<unordered_map<long long, ID>> symtab;
// extern unordered_map<int, int *> func_inst;
// extern unordered_map<int, int *> func_var;
extern char *reserve[12];

// match this character then go next
void match(int tk)
{
    if (token == tk)
    {
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
        symtab[0][Hash].In_value = (long long)(text + 1);
        symtab.push_back(unordered_map<long long, ID>{}); //enter scope of the function
        match('(');
        if(token != ')')
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
        if (token == Assign) //assign is optional
        {
            match(Assign);
            if (decl_type == DOUBLE)
            {
                match(Con_Double);
                symtab[0][Hash].In_value = (long long)data;
                data = data + sizeof(int);
            }
            else if (decl_type == INT)
            {
                match(Con_Int);
                symtab[0][Hash].In_value = (long long)data;
                data = data + sizeof(int);
            }
            else if (decl_type == CHAR)
            {
                match(Con_Char);
                symtab[0][Hash].In_value = (long long)data;
                data = data + sizeof(int);
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
            int temp = Hash; //store the last id, otherwise the id in [] may overload
            match('[');
            symtab[0][Hash].Type += PTR;
            int len = 0;
            if (token == Con_Int)
            {
                match(Con_Int);
                len = token_val;
            }
            else if (token == Id)
            {
                if (symtab[0][temp].Type != Int || symtab[0][temp].In_value == 0)
                {
                    cout << "array size erro! in line " << lineno << endl;
                    exit(1);
                }
                len = symtab[0][temp].In_value;
                match(Id);
            }
            symtab[0][temp].len = len;           //mark array size
            // symtab[0][temp].addr = new int[len]; // malloc address for array
            //TODO consider how to delete the allocated memory of the array when leave scope
            match(']');
        }
        while (token != ';')
        {
            match(',');
            match(Id);
            symtab[0][Hash].Class = Var;
            if (token == Assign) //assign is optional
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
                    symtab[0][Hash].In_value = token_val;
                }
                else if (decl_type == CHAR)
                {
                    match(Con_Char);
                    symtab[0][Hash].In_value = token_val;
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
                int temp = Hash; //store the last id, otherwise the id in [] may overload
                match('[');
                symtab[0][Hash].Type += PTR;
                int len = 0;
                if (token == Con_Int)
                {
                    match(Con_Int);
                    len = token_val;
                }
                else if (token == Id)
                {
                    if (symtab[0][temp].Type != Int || symtab[0][temp].In_value == 0)
                    {
                        cout << "array size erro! in line " << lineno << endl;
                        exit(1);
                    }
                    len = symtab[0][temp].In_value;
                    match(Id);
                }
                symtab[0][temp].len = len;           //mark array size
                // symtab[0][temp].addr = new int[len]; // malloc address for array
                //TODO consider how to delete the allocated memory of the array when leave scope
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
    symtab[0][Hash].In_value = token_val;
    symtab[0][Hash].Class = Var;
    symtab[0][Hash].Type = Con_Int;
    while (token != '}')
    {
        match(',');
        match(Id);
        match(Assign);
        match(Con_Int);
        symtab[0][Hash].In_value = token_val;
        symtab[0][Hash].Class = Var;
    }
    match('}');
}

//var_decl : type {'*'} id {'=' num}{ ',' id {'=' num}} ';'
//TODO: add variable to function stack
void var_decl()
{
    /*decl_type = token;
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
            symtab.back()[Hash].In_value = token_val;
        }
        else if (decl_type == Char)
        {
            match(Con_Char);
            symtab.back()[Hash].In_value = token_val;
        }
        else if (decl_type > PTR)
        {
            cout << "ptr is defaultly assign as NULL in line "
                 << lineno << endl;
            exit(1);
        }
    }
    else if (token == '[') // 数组形式[]
    {
        int temp = Hash; //store the last id
        match('[');
        symtab.back()[Hash].Type += PTR;
        if (token == Con_Int)
        {
            match(Con_Int);
            symtab.back()[Hash].len = token_val; //mark array size
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
    }*/
    while (token != ';')
    {
        if(token == Int)
            exp_type = INT;
        else if(token == Char)
            exp_type = CHAR;
        else if(token == Double)
            exp_type = DOUBLE;

        match(token);
        match(Id);

        symtab.back()[Hash].Class = Var;
        symtab.back()[Hash].Type = exp_type;
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
                symtab.back()[Hash].In_value = token_val;
            }
            else if (decl_type == Char)
            {
                match(Con_Char);
                symtab.back()[Hash].In_value = token_val;
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
                symtab.back()[Hash].len = token_val; //mark array size
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
        if(token == ',')
            match(',');
    }
    is_decl = false;
    match(';');
}

//func_para : type {'*'} id {',' type {'*'} id}
//TODO add parameters to function variable stack
//TODO add pass like int a[], add pass by reference
void func_para()
{
    int type;
    int params;
    params = 0;
    while(token != ')') {
        if (token == Int) {
            type = INT;
            match(Int);
        } else if (token == Char) {
            type = CHAR;
            match(Char);
        } else if (token == Double) {
            type = DOUBLE;
            match(Double);
        } else {
            cout << "unknown type in line " << lineno << endl;
            exit(1);
        }
        decl_type = type;
        while (token == Mul) {
            match(Mul);
            decl_type += PTR;
        }
        if (token == Lan) //TODO 引用传递，暂时跳过
        {
        }
        match(Id);
        if (token == '[') //TODO 数组，暂时跳过
        {
            match('[');
            //
            match(']');
            //
        }
        // store the local variable
        symtab.back()[Hash].Type = type;
        symtab.back()[Hash].Class  = Var;
        symtab.back()[Hash].In_value  = params++;   // index of current parameter
        if(token == ',')
            match(',');
    }
/*    while (token != ')')
    {
        match(',');
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
        if (token == Lan) //TODO &, pass by reference
        {
        }
        match(Id);
        if (token == '[') //TODO pass array
        {
            match('[');
            //
            match(']');
            //
        }
    }*/
    index_of_bp = params+1;
}

//func_body : {var_decl} {stmt}
//TODO add function body to function instruction
void func_body()
{
    int pos_local; // position of local variables on the stack.
    int type;
    pos_local = index_of_bp;

    if (token == '{') //enter a nest scope
    {
        symtab.push_back(unordered_map<long long, ID>{});
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
            symtab.push_back(unordered_map<long long, ID>{});
            match('{');
            func_body();
            symtab.pop_back(); //leave scope
            match('}');
        }
    }
    // save the stack size for local variables
    *++text = ENT;
    *++text = pos_local - index_of_bp;

    //statements
    is_decl = false;
    while (token != '}')
        stmt();

    *++text = LEV;
}

//stmt : empty_stmt | no_empty_stmt
//no_empty_stmt : if_stmt | while_stmt | '{' stmt '}' | 'return' exp ';' | exp ';'
void stmt()
{
    int *a, *b;
    if (token == If) //if stmt
    {
        match(If);
        match('(');
        exp(Assign); //control sentence
        match(')');

        *++text = JZ;
        b = ++text;

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
        a = text + 1;
        match('(');
        exp(Assign);
        match(')');
        *++text = JZ;
        b = ++text;
        stmt();
        *++text = JMP;
        *++text = (long long)a;
        *b = (long long)(text + 1);
    }
    else if (token == '{') //nest scope
    {
        symtab.push_back(unordered_map<long long, ID>{});
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

        *++text = LEV;
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
