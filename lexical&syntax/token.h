#include "lexical.h"
//definitions of syntax tree nodes

typedef enum{
    VAR_DEC, FUC_DEC
} declaration_kind; // variable, enum and function

typedef enum{
    COM_STMT, EXP_STMT, SEL_STMT, ITE_STMT, RET_STMT
}statement_kind; // compond, expression, select, iterate, return statement

typedef enum{
    ASS_EXP, SIM_EXP
}expression_kind; //assign and simmple expression

typedef enum{
    EXP, L_VALUE, CALL, LITERAL_CONST
}factor_kind;



//***************************************
//program definitions 
struct program{
    struct declaration * sub;
} ; // root node, contain declarations

struct declaration{
    declaration_kind dec_kind;
    union{
        struct var_declaration * var_dec;
        struct funct_declaration * funct_dec;
    }sub;
    struct declaration * next;
}; //declaration node, contains a sub node and  a sibling node



//***********************************
//node definitions for statements
struct statement{
        statement_kind stmt_kind;
        union{
            struct expression_statement* exp_stmt;
            struct compond_statement* com_stmt;
            struct select_statement* sel_stmt;
            struct iteration_statement* ite_stmt;
            struct return_statement* ret_stmt;
        }sub;
};

struct args_list{
    struct expression * exp;
    struct arg_list * next;
};

struct call{
    Token func_name;
    struct args_list args;
};//call of a function

struct factor_expression{
    factor_kind fac_kind;
    union{
        struct expression *exp;
        struct l_value * l_value;
        int int_const;
        double double_const;
        struct call * call;
        }value;
};

struct term_expression{
    struct term_expression * term;
    int mulop;//    *, /
    struct factor_expression factor;
};

struct add_expression{
    struct add_expression * add_exp;
    struct term_expression term;
    int addop;//+ , -
};

struct l_value{
    int type;
    Token id;
    struct expression * exp; // in case of array
};

struct assign_expression{
    struct l_value l_value;//left value
    struct expression* exp;
};

struct sim_expression{
    int node_type;// 1 means no relop, 2 means two add_exp and a relop 
    struct add_expression add_exp[2];
    int relop; // < , >  and so on 
};

struct expression{
    expression_kind exp_kind;
    union{
        struct assign_expression *ass_exp;
        struct simple_expression *sim_exp;
    }sub;
};

struct expression_statement{
    struct expression  exp;
};

struct local_declaration_list{
    struct var_declaration *var_dec;
    struct local_declaration_list* next;
};

struct statement_list{
    struct statement * stmt;
    struct statement_list * next;
};

struct compond_statement{
    struct local_declaration_list  local_dec;
    struct statement_list stmt_list;
};

struct select_statement{
    struct expression exp;
    struct statement true_stmt;
    struct statement false_stmt;
};

struct iteration_statement{
    struct expression exp;
    struct statement stmt;
};

struct return_statement{
    struct expression exp;
};



//************************************
//node definitions for declarations
struct id_list{
    Token id;
    struct id_list * next;
};

struct var_declaration{
    int type;// sys_type or ptr or array
    struct id_list;
    int array_size; // in case of arr
};

struct para{
    int type_specifier;
    Token id;
};//pass by value or ptr

struct para_list{
    struct para para;
    struct para_list* next_para;
};

struct funct_declaration{
    int type;
    Token func_name;
    struct para_list paras;
    struct compond_statement com_stmt;
};
