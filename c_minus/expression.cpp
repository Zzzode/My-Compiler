#include <iostream>
#include <unordered_map>
#include <vector>
#include "c_minus.h"
#include "vmachine.h"
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

/**
 * @Author: zxhcho, Zzzode
 * @Date: 2019-06-14 19:52:48
 * @Desc: expressions
 */
void exp(int level)
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

            *++text = IMM;
            *++text = token_in_val;
        }
        else if (token == Con_Double)
        {
            match(Con_Double);
            exp_type = Con_Double;

            *++text = IMM;
            *++text = token_d_val; // TODO: 需要转换为IEEE浮点数存储
        }
        else if (token == Con_Str)
        {
            match(Con_Str);
            exp_type = Con_Str;
            //
            *++text = IMM;
            *++text = ; // TODO: 需要把字符串转换成二进制
        }
        else if (token == Con_Char)
        {
            match(Con_Char);
            exp_type = Con_Char;
            //
            *++text = IMM;
            *++text = token_str_val[0]; // TODO: 是否已经转换成二进制？
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

            *++text = IMM;
            *++text = (exp_type == CHAR) ? sizeof(char) : sizeof(int); // TODO: 如果是double？

            exp_type = INT;
        }
        else if (token == Id)
        { //function call or variable or enum constant(can't be assign again)
            match(Id);
            if (token == '(') //function call
            {   //add func_paras
                match('(');
                // TODO: Fix all the func paras between "(" ")" √
                int temp = 0; // number of arguments
                while (token != ')')
                {
                    exp(Assign);
                    *++text = PUSH;
                    temp++;

                    if (token == ',')
                    {
                        match(',');
                    }
                }
                match(')');
            }
            else if (symtab[0][Hash].Type == Con_Int) //enum
            { // TODO: Fix it
                *++text = IMM;
                *++text = symtab[0][Hash].In_value;
                exp_type = INT;
            }
            else
            {
                // variable
                // TODO: 这里似乎没有分局部变量和全局变量？
                if (symtab[0][Hash].Class == Var)
                {
                    if(symtab[0][Hash].Type != DOUBLE){
                        *++text = LEA;
                        *++text = index_of_bp - symtab[0][Hash].In_value;
                    }
                    else if(symtab[0][Hash].Type == CHAR)
                    { // TODO: 这边有待商榷
                        *++text = LEA;
                        *++text = index_of_bp - symtab[0][Hash].In_value;
                    }
                }
                else
                {
                    printf("%d: undefined variable\n", line);
                    exit(-1);
                }

                // default behaviour is to load the value of the
                // address which is stored in `ax`
                exp_type = symtab[0][Hash].Type;
                *++text = (exp_type == CHAR) ? LC : LI;
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

                exp(Inc);
            }
            else
            {
                exp(Assign);
                match(')');
            }
        }
        else if (token == Mul) //reference to address
        {
            match(Mul);
            exp(Inc);
            if (exp_type >= PTR)
                exp_type -= PTR;
            else
            {
                cout << "wrong reference to address in line "
                     << lineno << endl;
                exit(1);
            }

            *++text = (exp_type == CHAR) ? LC : LI;
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
