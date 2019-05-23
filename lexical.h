#ifndef _LEXICAL_H_
#define _LEXICAL_H_
//headfile for lexical

typedef enum{
    OPERATOR, RESERVED_WORD,
    //system type
    SYS_CON, SYS_FUN, SYS_PROC, SYS_TYPE,
    //identify an id
    ID, 
    //type for an id
    TYPE_INT, TYPE_REAL, TYPE_CHAR, TYPE_BOOL, TYPE_STR,
    //literal constant
    CONST_INT, CONST_REAL, CONST_CHAR, CONST_BOOL, CONST_STR
} Token_type;

typedef enum{
    //operators
    LP = 18, RP, LB, RB, DOT, COMMA, COLON, MUL, DIV, DIV_R, UNEQUAL, NOT,
    PLUS, MINUS, GE, GT, LE, LT, EQUAL, ASSIGN, MOD, DOTDOT, SEMI,
    // reserve words
    AND, ARRAY, BEGIN, CASE, CONST, DO, DOWNTO, ELSE,
    END, FOR, FUNCT, GOTO, IF, IN, OF, OR, PACKED, 
    PROC, PROGRAM, RECORD, REPEAT, SET, THEN, TO, 
    TYPE, UNTIL, VAR, WHILE, WITH,
    // SYS_CON
    FALSE, MAXINT, TRUE,
    // SYS_FUNCT
    ABS, CHR, ODD, ORD, PRED, SQR, SQRT, SUCC,
    // SYS_PROC
    WRITE, WRITELN, READ,
    // SYS_TYPE for declaration
    BOOLEAN, CHAR, INTEGER, REAL, STR
} Token_name; 

typedef struct token_attri{
    int token_type; // operator or reserved_word or...
    int token_name; 
    int data_type; // real, int, bool or...
    union{
        char token_string[32];  //max-length of an id or string 
        int int_num;    //value for int-type
        char ch;    //value for char-type
        double real_num;    //value for real_type
    }token_val;
} *Token; // attribute of a token 

#endif // !_LEXI_H_