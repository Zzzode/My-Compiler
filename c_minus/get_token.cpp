#include <iostream>
#include <string.h>
#include <cctype>
#include <unordered_map>
#include "c_minus.h"
using namespace std;
//functions of lexical part

extern char token;
extern char * src;
extern int lineno;
extern int layer;
extern int decl_type;
extern int token_in_val;
extern double token_d_val;
extern char token_str_val[64];
extern unordered_map<int,ID> symtab; 
int Hash;//mark the cuurent token

void next()
{
    while((token = *(src++)))
    {
        if(token == '\n')
            lineno++;

        else if(token == '#')//skip the macro, not supported
        {
            while (*src != 0 && *src != '\n') 
            src++;
        }

         // char literal constant ,we only support the escape of \n, \t, \r
        else if (token == '\'')
        {
            token = Con_Char;
            token_in_val = *(src++);//store char as int type
            if(token_in_val == '\\')//escape charactor
            {
                if(*src == 'n')
                    token_in_val = '\n';
                else if(*src == 't')
                    token_in_val = '\t';
                else if(*src == 'r')
                    token_in_val = '\r';
                else
                {
                    cout << "wrong escape charactor! in line " << lineno << endl;
                    exit(1);
                }
                ++src;
            }
            if(*src != '\'')
            {
                cout << "lack \' of literal charactor in line " << lineno << endl;
                exit(1);
            }
            return;
        }

        else if(token == '"')//string literal constant
        {
            int cnt = 0;
            while( (token = *(src++)) != '"' && cnt < 63)//max string length is 63
            {
                if(token == '\\')
                {
                    if(*src == 'n')
                        token_str_val[cnt++] = '\n';
                    else if(*src == 't')
                        token_str_val[cnt++] = '\t';
                    else if(*src == 'r')
                        token_str_val[cnt++] = '\r';
                    ++src;
                }
                else
                    token_str_val[cnt++] = token;
            }
            token_str_val[cnt] = '\0';
            if(cnt == 63 && token != '"')
            {
                cout << "string length out of range 64, in line " << lineno << endl;
                exit(1);
            }
            token = Con_Str;
            return;
        }

        else if (token == '/')//comment or divide
        {
            if(*src == '/')//skip comments
            while(*src != '\n' && *src != 0)
                ++src;
            else
            {
                token = Div;
                return ;
            }
        }

        else if (token == '=')// = or ==
        {
            if(*src == '=')//skip comments
            {
                token = Eq;
                ++src;
            }
            else
                token = Assign;
            return ;
        }

        else if (token == '+')// + or ++
        {
            if(*src == '+')
            {
                token = Inc;
                ++src;
            }
            else
                token = Add;
            return ;
        }

        else if(token == '-')// - or --
        {
            if(*src == '-')
            {
                token = Dec;
                ++src;
            }
            else
                token = Sub;
            return;
        }

        else if(token == '!') // !=
        {
            if (*src == '=') 
            {
                token = Ne;
                ++src;
            }
            return;
        }

        else if(token == '<')  // <  or <= or <<
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

        else if(token == '>')  // >  or >= or >>
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
        {
            // get Hash of the id, and store the string ptr in last
            Hash = token;
            int index = 0;
            char temp[64];
            temp[index++] = token;
            while (isalpha(token) || isdigit(token) || (token == '_'))
            {
                Hash = Hash * 147 + *(src++);
                temp[index++] = token;
                if(index > 63)
                {
                    cout << "identifier out of range(64) in line " 
                        << lineno << endl;
                    exit(1);
                }
            }
            temp[index] = '\0';
            if(symtab.find(Hash) == symtab.end())//no duplicate
            {
                strcpy(symtab[Hash].Name, temp);
                symtab[Hash].Token = token = Id; //mark the token as an id
                symtab[Hash].info.push_back(id_info(decl_type, layer, 0, 0));
                //default value attribute is 0
            }
            else//duplicate id name
            {
                if(symtab[Hash].info.back().Layer >= layer)
                {
                    cout << "duplicate definition of identifier in line " 
                        << lineno << endl;
                    exit(1);
                }
                else
                    symtab[Hash].info.push_back(id_info(decl_type, layer, 0, 0));
            }
            return;
        }

        else if (isdigit(token))//in case of  positive int or double
        {
            token_in_val = token-'0';
            while(isdigit(token = *(src++)))
                token_in_val = token_in_val*10 + token-'0';
            token = Con_Int;
            if(token == '.')
            {
                double dot = 0.1;
                token_d_val = token_in_val;
                while(isdigit(token = *(src++)))
                {
                    token_d_val += dot*(token-'0');
                    dot /= 10;
                }    
                token = Con_Double;
            }
            return;
        }

        else if(token == '[' || token == ']' || token == '?' || token == '~' || token == '(' || token == ')'
                    || token == ';' || token == '{' || token == '}' || token == ':')
            return;

        else if(!isspace(token))//illegal charactor
        {
            cout << "illegal charactor in line "<< lineno << endl;
            exit(1);
        }
    }
}