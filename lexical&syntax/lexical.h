#ifndef _LEXICAL_H_
#define _LEXICAL_H_
//headfile for lexical

//system constant
#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define FALSE  0
#define TRUE 1

enum Token_type{
    //system type
    SYS_FUN = 2, SYS_TYPE,
    //identify an id
    ID, 
    //type for an id
    TYPE_INT, TYPE_DOUBLE, TYPE_CHAR, TYPE_BOOL,
    PTR_INT, TPR_DOUBLE, PTR_CHAR, PTR_BOOL,
    ARR_INT, ARR_DOUBLE, ARR_CHAR, ARR_BOOL,
    //literal constant
    CONST_INT, CONST_DOUBLE, CONST_CHAR
};

enum Token_name{
    //operators and delimers
    LP = 20, RP, LSB, RSB, LCB, RCB, COMMA, MUL, DIV, NE, NOT, AND, OR,
    PLUS, MINUS, GE, GT, LE, LT, EQUAL, ASSIGN, MOD, SEMI, ADDR,
    // reserve words
    ELSE, IF, RETURN,  WHILE, CONST,
    // sys_function
    ABS, SQRT, MAX, MIN,
    // SYS_TYPE for declaration
    BOOL, INT, DOUBLE,  CHAR, VOID, ENUM
} ; 

typedef struct token_attri{
    int token_type; 
    int token_index;  //index in enum definition
    int data_type;//  if token type is an id , record its type
    char token_string[32];  //record token text
    union{
        bool bool_num;
        int int_num;    //value for int-type
        char ch;    //value for char-type
        double double_num;    //value for real_type
    }token_val;
} *Token; // attribute of a token 

#endif // !_LEXI_H_