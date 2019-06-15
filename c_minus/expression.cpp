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
 * @Desc: exps
 */
void exp(int level)
{
    int temp;
    int *addr;
    //unary operator
    {
        if (!token)
        {
            cout << "unexpected token EOF of exp in line "
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
                temp = 0; // number of arguments
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
            exp(Inc);
            if (*text == LC || *text == LI)
            {
                text--;
            }
            else
            {
                cout << "bad address of " << line << endl;
                exit(-1);
            }
            exp_type += PTR;
        }
        else if (token == Not) //! or not
        {
            match(Not);
            exp(Inc);

            *++text = PUSH;
            *++text = IMM;
            *++text = 0;
            *++text = EQ;

            exp_type = INT;
        }
        else if (token == '~') //bit not
        {
            match('~');
            exp(Inc);

            *++text = PUSH;
            *++text = IMM;
            *++text = -1;
            *++text = XOR;

            exp_type = INT;
        }
        else if (token == Add) //+, legal but meaningless
        {
            match(Add);
            exp(Inc);

            exp_type = INT;
        }
        else if (token == Sub) //-, negative number
        {
            match(Sub);
            if (token == Con_Int) //negative literal constant
            {
                *++text = IMM;
                *++text = -token_in_val;
                match(Con_Int);
            }
            else if (token == Con_Double){
                *++text = IMM;
                *++text = -token_d_val;
                match(Con_Double);
            }
            else //negative variable
            {
                *++text = IMM;
                *++text = -1;
                *++text = PUSH;
                exp(Inc);
                *++text = MUL;
            }
            exp_type = INT;
        }
        else if (token == Inc || token == Dec)
        {
            temp = token;
            match(token);
            exp(Inc);
            //
            if (*text == LC)
            {
                *text = PUSH; // to duplicate the address
                *++text = LC;
            }
            else if (*text == LI)
            {
                *text = PUSH;
                *++text = LI;
            }
            else
            {
                printf("%d: bad lvalue of pre-increment\n", line);
                exit(-1);
            }
            *++text = PUSH;
            *++text = IMM;
            *++text = (exp_type > PTR) ? sizeof(int) : sizeof(char);
            *++text = (temp == Inc) ? ADD : SUB;
            *++text = (exp_type == CHAR) ? SC : SI;
        }
        else
        {
            cout << "wrong prefix or expression! in line "
                 << lineno << endl;
        }
    }

    // binary operator and postfix operators.
    {
        while (token >= level)
        {
            // handle according to current operator's precedence
            temp = exp_type;
            if (token == Assign)
            {
                // var = expr;
                match(Assign);
                if (*text == LC || *text == LI)
                {
                    *text = PUSH; // save the lvalue's pointer
                }
                else
                {
                    cout << "bad lvalue in assignment" << line;
                    exit(-1);
                }
                exp(Assign);

                exp_type = temp;
                *++text = (exp_type == CHAR) ? SC : SI;
            }
            // else if (token == Cond)
            // {
            //     // expr ? a : b;
            //     match(Cond);
            //     *++text = JZ;
            //     addr = ++text;
            //     exp(Assign);
            //     if (token == ':')
            //     {
            //         match(':');
            //     }
            //     else
            //     {
            //         cout << "missing colon in conditional " << line << endl;
            //         exit(-1);
            //     }
            //     *addr = (int)(text + 3);
            //     *++text = JMP;
            //     addr = ++text;
            //     exp(Cond);
            //     *addr = (int)(text + 1);
            // }
            else if (token == Lor)
            {
                // logic or
                match(Lor);
                *++text = JNZ;
                addr = ++text;
                exp(Lan);
                *addr = (int)(text + 1);
                exp_type = INT;
            }
            else if (token == Lan)
            {
                // logic and
                match(Lan);
                *++text = JZ;
                addr = ++text;
                exp(Or);
                *addr = (int)(text + 1);
                exp_type = INT;
            }
            else if (token == Or)
            {
                // bitwise or
                match(Or);
                *++text = PUSH;
                exp(Xor);
                *++text = OR;
                exp_type = INT;
            }
            else if (token == Xor)
            {
                // bitwise xor
                match(Xor);
                *++text = PUSH;
                exp(And);
                *++text = XOR;
                exp_type = INT;
            }
            else if (token == And)
            {
                // bitwise and
                match(And);
                *++text = PUSH;
                exp(Eq);
                *++text = AND;
                exp_type = INT;
            }
            else if (token == Eq)
            {
                // equal ==
                match(Eq);
                *++text = PUSH;
                exp(Ne);
                *++text = EQ;
                exp_type = INT;
            }
            else if (token == Ne)
            {
                // not equal !=
                match(Ne);
                *++text = PUSH;
                exp(Lt);
                *++text = NE;
                exp_type = INT;
            }
            else if (token == Lt)
            {
                // less than
                match(Lt);
                *++text = PUSH;
                exp(Shl);
                *++text = LT;
                exp_type = INT;
            }
            else if (token == Gt)
            {
                // greater than
                match(Gt);
                *++text = PUSH;
                exp(Shl);
                *++text = GT;
                exp_type = INT;
            }
            else if (token == Le)
            {
                // less than or equal to
                match(Le);
                *++text = PUSH;
                exp(Shl);
                *++text = LE;
                exp_type = INT;
            }
            else if (token == Ge)
            {
                // greater than or equal to
                match(Ge);
                *++text = PUSH;
                exp(Shl);
                *++text = GE;
                exp_type = INT;
            }
            else if (token == Shl)
            {
                // shift left
                match(Shl);
                *++text = PUSH;
                exp(Add);
                *++text = SHL;
                exp_type = INT;
            }
            else if (token == Shr)
            {
                // shift right
                match(Shr);
                *++text = PUSH;
                exp(Add);
                *++text = SHR;
                exp_type = INT;
            }
            else if (token == Add)
            {
                // add
                match(Add);
                *++text = PUSH;
                exp(Mul);

                exp_type = temp;
                if (exp_type > PTR)
                {
                    // pointer type, and not `char *`
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                }
                *++text = ADD;
            }
            else if (token == Sub)
            {
                // sub
                match(Sub);
                *++text = PUSH;
                exp(Mul);
                if (temp > PTR && temp == exp_type)
                {
                    // pointer subtraction
                    *++text = SUB;
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = DIV;
                    exp_type = INT;
                }
                else if (temp > PTR)
                {
                    // pointer movement
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                    *++text = SUB;
                    exp_type = temp;
                }
                else
                {
                    // numeral subtraction
                    *++text = SUB;
                    exp_type = temp;
                }
            }
            else if (token == Mul)
            {
                // multiply
                match(Mul);
                *++text = PUSH;
                exp(Inc);
                *++text = MUL;
                exp_type = temp;
            }
            else if (token == Div)
            {
                // divide
                match(Div);
                *++text = PUSH;
                exp(Inc);
                *++text = DIV;
                exp_type = temp;
            }
            else if (token == Mod)
            {
                // Modulo
                match(Mod);
                *++text = PUSH;
                exp(Inc);
                *++text = MOD;
                exp_type = temp;
            }
            else if (token == Inc || token == Dec)
            {
                // postfix inc(++) and dec(--)
                // we will increase the value to the variable and decrease it
                // on `ax` to get its original value.
                if (*text == LI)
                {
                    *text = PUSH;
                    *++text = LI;
                }
                else if (*text == LC)
                {
                    *text = PUSH;
                    *++text = LC;
                }
                else
                {
                    cout << "bad value in increment" << line << endl;
                    exit(-1);
                }

                *++text = PUSH;
                *++text = IMM;
                *++text = (exp_type > PTR) ? sizeof(int) : sizeof(char);
                *++text = (token == Inc) ? ADD : SUB;
                *++text = (exp_type == CHAR) ? SC : SI;
                *++text = PUSH;
                *++text = IMM;
                *++text = (exp_type > PTR) ? sizeof(int) : sizeof(char);
                *++text = (token == Inc) ? SUB : ADD;
                match(token);
            }
            else if (token == '[')
            {
                // array access var[xx]
                match('[');
                *++text = PUSH;
                exp(Assign);
                match(']');

                if (temp > PTR)
                {
                    // pointer, `not char *`
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                }
                else if (temp < PTR)
                {
                    printf("%d: pointer type expected\n", line);
                    exit(-1);
                }
                exp_type = temp - PTR;
                *++text = ADD;
                *++text = (exp_type == CHAR) ? LC : LI;
            }
            else
            {
                printf("%d: compiler error, token = %d\n", line, token);
                exit(-1);
            }
        }
    }
}
