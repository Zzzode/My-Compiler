#include <iostream>
#include <string.h>
#include <cctype>
#include <vector>
#include <unordered_map>
#include "c_minus.h"
#include "vmachine.h"
using namespace std;
//functions of lexical part

extern int token;
extern int lineno;
extern int decl_type;
extern long long Hash;
extern int layer;
extern bool is_decl;
extern int token_val;
extern double token_d_val;
extern char token_str_val[64];
extern vector<unordered_map<long long, ID>> symtab;
extern char *reserve[12];
extern char *src, *oldsrc;

//匹配下一个token
void next()
{
    char *last_pos;

    while ((token = *(src++)))
    {
        if (token == '\n') {
            if (assembly) {
                // print compile info
                printf("%d: %.*s", lineno, src - oldsrc, oldsrc);
                oldsrc = src;

                while (old_text < text) {
                    printf("%8.4s", &"LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,"
                                     "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                                     "OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT"[*++old_text * 5]);

                    if (*old_text <= ADJ)
                        printf(" %d\n", *++old_text);
                    else
                        printf("\n");
                }
            }
            lineno++;
        }
        else if (token == '[' || token == ']' || token == '?' || token == '~' || token == '(' || token == ')' || token == ';' || token == '{' || token == '}' || token == ':' || token == ','/*|| token == EOF*/)
            return;
        else if (isspace(token)) // 如果碰到空格，跳过，其实也可以什么都不做
            continue;
        else if (token == '#')
        {   //skip the macro, not supported
            while (*src != 0 && *src != '\n')
                src++;
        }
        else if (token == '"' || token == '\'')
        {   // char literal constant
            // we only support the escape of \n, \t, \r, '\\'
            int cnt = 0;
            last_pos = data;
            while(*src != 0 && *src != token) {
                cnt++;
                token_val = *(src++);  //store char as int type
                if (token_val == '\\') //escape charactor
                {
                    if (*src == 'n')
                        token_val = '\n';
                    else if (*src == 't')
                        token_val = '\t';
                    else if (*src == 'r')
                        token_val = '\r';
                    else if (*src == '\\')
                        token_val = '\\';
                    else {
                        cout << "wrong escape charactor! in line " << lineno << endl;
                        exit(1);
                    }
                    src++;
                }
                if (token == '"') {
                    *data++ = token_val;
                    // token = Con_Str;
                }
                if(cnt > 64)
                    cout << "string overflow in line " << lineno << endl;
            }
            ++src;
            if (token == '"') {
                token_val = (long long)last_pos;
                token = Con_Str;
            } else {
                token = Con_Char;
            }
//            if (*src != '\'')
//            {
//                cout << "lack \' of literal charactor in line " << lineno << endl;
//                exit(1);
//            }
            return;
        }
/*        else if (token == '"')
        {   //string literal constant
            int cnt = 0; // max string length
            last_pos = data;
            while ((token = *(src++)) != '"' && cnt < 63) //max string length is 63
            //TODO add EOF juegement, add security control
            {
                if (token == '\\')
                {
                    if (*src == 'n')
                        token_str_val[cnt++] = '\n';
                    else if (*src == 't')
                        token_str_val[cnt++] = '\t';
                    else if (*src == 'r')
                        token_str_val[cnt++] = '\r';
                    else if (*src == '\\')
                        token_str_val[cnt++] = '\\';
                    ++src;
                }
                else
                    token_str_val[cnt++] = token;
            }
            token_str_val[cnt] = '\0';
            if (cnt == 63 && token != '"')
            {
                cout << "string layergth out of range 64, in line " << lineno << endl;
                exit(1);
            }
            token = Con_Str;
            return;
        }*/
        else if (token == '/') //comment or divide
        {
            if (*src == '/') //skip comments
                while (*src != '\n' && *src != 0)
                    ++src;
            else
            {
                token = Div;
                return;
            }
        }

        else if (token == '=') // = or ==
        {
            if (*src == '=') //skip comments
            {
                token = Eq;
                ++src;
            }
            else
                token = Assign;
            return;
        }

        else if (token == '+') // + or ++
        {
            if (*src == '+')
            {
                token = Inc;
                ++src;
            }
            else
                token = Add;
            return;
        }

        else if (token == '-') // - or --
        {
            if (*src == '-')
            {
                token = Dec;
                ++src;
            }
            else
                token = Sub;
            return;
        }

        else if (token == '!') // .   != or !
        {
            if (*src == '=')
            {
                token = Ne;
                ++src;
            }
            else
                token = Not;
            return;
        }

        else if (token == '<') // <  or <= or <<
        {
            if (*src == '=')
            {
                token = Le;
                ++src;
            }
            else if (*src == '<')
            {
                token = Shl;
                ++src;
            }
            else
                token = Lt;
            return;
        }

        else if (token == '>') // >  or >= or >>
        {
            if (*src == '=')
            {
                token = Ge;
                ++src;
            }
            else if (*src == '>')
            {
                token = Shr;
                ++src;
            }
            else
                token = Gt;
            return;
        }

        else if (token == '|') // |  or ||
        {
            if (*src == '|')
            {
                token = Lor;
                ++src;
            }
            else
                token = Or;
            return;
        }

        else if (token == '&') //& or &&
        {
            if (*src == '&')
            {
                token = Lan;
                ++src;
            }
            else
                token = And;
            return;
        }

        else if (token == '^')
        {
            token = Xor;
            return;
        }

        else if (token == '%')
        {
            token = Mod;
            return;
        }

        else if (token == '*')
        {
            token = Mul;
            return;
        }

        else if (isalpha(token) || (token == '_')) // in case of an id
        {   // get Hash of the id, and store the id name in temp
            Hash = token;
            int index = 0;
            char temp[64];
            temp[index++] = token; 
            token = *src++;
            while (isalpha(token) || isdigit(token) || (token == '_'))
            {
                Hash = Hash * 147 + token;
                temp[index++] = token;
                token = *src++;
                if (index > 63)
                {
                    cout << "identifier out of range(64-bits) in line "
                         << lineno << endl;
                    exit(1);
                }
            }
            temp[index] = '\0';
            src--;//go back
            token = Id;

            //return reserve words
            int j = Char;
            for (int i = 0; i < 9; i++, j++)
            {
                if (strcmp(temp, reserve[i]) == 0)
                {
                    token = j;
                    return;
                }
            }

            if (is_decl) //in declaration sentence
            {
                if (symtab.back().find(Hash) != symtab.back().end()) //already in symtab
                {
                    cout << "identifier definition duplicate in line "
                         << lineno << endl;
                    exit(1);
                }
                else
                {
                    symtab.back()[Hash].Type = decl_type; //inherit type
                    symtab.back()[Hash].In_value = symtab.back()[Hash].D_value = 0;
                    //default value is 0,not sure it's a function or variable, dealing in syntax part
                }
            }
            else//call of a known identifier
            {
                layer = symtab.size();
                while (layer--)//find the variable in which layer
                {
                    if (symtab[layer].find(Hash) != symtab[layer].end())
                        break;
                }
                if (layer == -1)
                {
                    cout << "unknown identifier in line "
                         << lineno << endl;
                    exit(1);
                }
            }
            return;
        }

        else if (isdigit(token)) //in case of  positive int or double
        {
            token_val = token - '0';
            while (isdigit(token = *(src++)))
                token_val = token_val * 10 + token - '0';
            token = Con_Int;
            src--;
            if (*src == '.')
            {
                src++;
                double dot = 0.1;
                token_d_val = token_val;
                while (isdigit(token = *(src++)))
                {
                    token_d_val += dot * (token - '0');
                    dot /= 10;
                }
                token = Con_Double;
                src--;
            }
            return;
        }

        /*else if (token == '[' || token == ']' || token == '?' || token == '~' || token == '(' || token == ')' || token == ';' || token == '{' || token == '}' || token == ':')
            return;
*/
        else //illegal charactor
        {
            cout << "illegal charactor in line " << lineno << endl;
            exit(1);
        }
    }
}
