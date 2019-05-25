#ifndef _LEXICAL_H_
#define _LEXICAL_H_
//headfile for lexical

enum Token_type{
    OPERATOR, RESERVE,
    //system type
    SYS_CON, SYS_FUN, SYS_PROC, SYS_TYPE,
    //identify an id
    ID, 
    //type for an id
    TYPE_INT, TYPE_REAL, TYPE_CHAR, TYPE_BOOL, TYPE_STR,
    //literal constant
    CONST_INT, CONST_REAL, CONST_CHAR, CONST_BOOL, CONST_STR
};

enum Token_name{
    //operators and delimers
    LP = 17, RP, LSB, RSB, LCB, RCB, COMMA, MUL, DIV, NE, NOT, AND, OR,
    PLUS, MINUS, GE, GT, LE, LT, EQUAL, ASSIGN, MOD, SEMI,
    // reserve words
    ELSE, IF, RETURN, VOID, WHILE, CONST, VAR,
    // sys_constant
    FALSE, MAXINT, TRUE,
    // sys_function
    ABS, SQRT, MAX, MIN,
    // SYS_TYPE for declaration
    BOOL, INT, REAL, STR, CHAR
} ; 

typedef struct token_attri{
    int token_type; // operator or reserved_word or...
    int token_index; 
    int data_type; // real, int, bool or...
    char token_string[32];  //max-length of an id or string 
    union{
        int int_num;    //value for int-type
        char ch;    //value for char-type
        double real_num;    //value for real_type
    }token_val;
} *Token; // attribute of a token 

#endif // !_LEXI_H_