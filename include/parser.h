
#ifndef _MLG_PARSER_H_
#define _MLG_PARSER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <ctype.h>
    #include "location.h"
    #include "token.h"
    #include "ast.h"
    #include "utf8.h"


    typedef struct parser_s
    {

        token_t **tokens;

        size_t tokens_count;

        size_t token_index;

        token_t *token;
        
    } parser_t;

    parser_t *create_parser(token_t **tokens, size_t tokens_count);

    bool init_parser(parser_t *parser, token_t **tokens, size_t tokens_count);

    bool destroy_parser(parser_t *parser);

    ast_node_t *parser_parse(parser_t *parser);
    
    ast_node_t *parser_parse_statements(parser_t *parser);

    ast_node_t *parser_parse_statement(parser_t *parser);

    ast_node_t *parser_parse_variable(parser_t *parser);

    ast_node_t *parser_parse_function(parser_t *parser);

    type_kind_t parser_token_to_type_kind(parser_t *parser);

    bool parser_parse_type_kind(parser_t *parser, type_kind_t *type_kind, char **type_name);

    ast_node_t *parser_parse_expression(parser_t *parser);
    
    ast_node_t *parser_parse_factor(parser_t *parser);

    ast_node_t *parser_parse_number(parser_t *parser);

    void parser_current_token_index(parser_t *parser, size_t index);

    void parser_current_token(parser_t *parser);

    void parser_next_token(parser_t *parser);

    token_t *parser_eat_token(parser_t *parser, token_type_t expected_type);

#endif
