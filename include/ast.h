
#ifndef _MLG_AST_H_
#define _MLG_AST_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "type.h"


    typedef enum ast_operator_type_e
    {

        AST_OPERATOR_TYPE_INVALID = 0,
        
        AST_OPERATOR_TYPE_PLUS  ,
        AST_OPERATOR_TYPE_MINUS ,
        AST_OPERATOR_TYPE_TIMES ,
        AST_OPERATOR_TYPE_MODULO,
        AST_OPERATOR_TYPE_DIVIDE

    } ast_operator_type_t;

    typedef enum ast_node_type_e
    {

        AST_NODE_TYPE_INVALID = 0,  

        AST_NODE_TYPE_STATEMENTS,

        AST_NODE_TYPE_VARIABLE_DEC,
        AST_NODE_TYPE_VARIABLE_DEF,

        AST_NODE_TYPE_FUNCTION_DEC,
        AST_NODE_TYPE_FUNCTION_DEF,

        AST_NODE_TYPE_BI_EXPR,
        AST_NODE_TYPE_U_EXPR,

        AST_NODE_TYPE_LITERAL,
 
    } ast_node_type_t;
    
    typedef struct ast_node_s
    {

        ast_node_type_t type;

        union
        {

            struct
            {
            
                struct ast_node_s **statements;

                size_t statements_count;

            };
            
            struct
            {

                char *variable_name;

                type_kind_t variable_type_kind;

                char *variable_type_name;

                struct ast_node_s *variable_expr;

            };

            struct
            {

                char *function_name;

                struct ast_node_s *function_parameters;

                type_kind_t function_return_type_kind;

                char *function_return_type_name;

                struct ast_node_s *function_body;

            };

            struct
            {

                ast_operator_type_t u_expr_operator;

                struct ast_node_s *u_expr_factor_node;

            };

            struct
            {

                ast_operator_type_t bi_expr_operator;

                struct ast_node_s *bi_expr_right_factor_node;

                struct ast_node_s *bi_expr_left_factor_node;

            };

            struct
            {

                type_kind_t literal_type_kind;

                char *literal_value;
                
            };
            
            struct
            {

                struct ast_node_s *invalid_root_node;

            };

        };

    } ast_node_t;

    ast_node_t *create_ast_node(ast_node_type_t type);

    bool ast_node_statements_push(ast_node_t *statements_node, ast_node_t *node);

    bool ast_node_statements_pop(ast_node_t *statements_node, ast_node_t **node);

    bool destroy_ast_node(ast_node_t *node);

#endif
