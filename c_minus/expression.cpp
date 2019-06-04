#include <iostream>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
using namespace std;

extern int token;
extern int lineno;
extern int decl_type;
extern int Hash;
extern int layer;
extern int exp_type;
extern bool is_decl;
extern int token_in_val;
extern double token_d_val;
extern vector<unordered_map<int, ID>> symtab;

void exp()
{
    //unary operator
    {
        if (!token)
        {
            cout << "unexpected token EOF of expression in line "
                 << lineno << endl;
            exit(-1);
        }

        //literal constant, int,char,double, str
        if (token == Con_Int)
        {
            match(Con_Int);
            exp_type = Con_Int;
            //
        }
        else if (token == Con_Double)
        {
            match(Con_Double);
            exp_type = Con_Double;
            //
        }
        else if (token == Con_Str)
        {
            match(Con_Str);
            exp_type = Con_Str;
            //
        }
        else if (token == Con_Char)
        {
            match(Con_Char);
            exp_type = Con_Char;
            //
        }
        else if (token == Sizeof) //sizeof(typename) or sizeof(variable)
        {
            match(Sizeof);
            match('(');
            if (token == Int)
            {
                match(Int);
                exp_type = INT;
                while (token == Mul)
                {
                    match(Mul);
                    exp_type = exp_type + PTR;
                }
            }
            else if (token == Char)
            {
                match(Char);
                exp_type = CHAR;
                while (token == Mul)
                {
                    match(Mul);
                    exp_type = exp_type + PTR;
                }
            }
            else if (token == Double)
            {
                match(Double);
                exp_type = DOUBLE;
                while (token == Mul)
                {
                    match(Mul);
                    exp_type = exp_type + PTR;
                }
            }
            else if (token == Id)
            {
                match(Id);
                exp_type = symtab[layer][Hash].Type;
            }

            match(')');
        }
        else if (token == Id) //function call or variable or enum constant(can't be assign again)
        {
            match(Id);
            if (token == '(') //function call
            {
                match('(');
                //add func_paras
                match(')');
            }
            else if (symtab[0][Hash].Type == Con_Int) //enum
            {
            }
            else //variable
            {
            }
        }

        else if (token == '(') //type casting or parenthese
        {
            match('(');
            if (token == Int || token == Double || token == Char)
            {
                exp_type = token;
                next();
                while (token == Mul)
                {
                    match(Mul);
                    exp_type = exp_type + PTR;
                }
                match(')');
                //
            }
            else
            {
                exp();
                match(')');
            }
        }
        else if (token == Mul) //reference to address
        {
            match(Mul);
            exp();
            if (exp_type >= PTR)
                exp_type -= PTR;
            else
            {
                cout << "wrong reference to address in line "
                     << lineno << endl;
                exit(1);
            }
        }
        else if (token == And) //&, get address of a variable
        {
            match(And);
            exp();
            exp_type += PTR;
        }
        else if (token == Not) //! or not
        {
            match(Not);
            exp();
        }
        else if (token == '~') //bit not
        {
            match('~');
            exp();
        }
        else if (token == Add) //+, legal but meaningless
        {
            match(Add);
            exp();
        }
        else if (token == Sub) //-, negative number
        {
            match(Sub);
            if (token == Con_Int || token == Con_Double) //negative literal constant
            {
            }
            else //negative variable
            {
                exp();
            }
        }
        else if (token == Inc || token == Dec)
        {
            match(token);
            exp();
            //
        }
        else
        {
            cout << "wrong prefix or expression! in line "
                 << lineno << endl;
        }
    }
}