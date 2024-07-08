#include "parser.h"
#include "util.h"


parser_t *create_parser(token_t **tokens, size_t tokens_count)
{

    parser_t *parser = (parser_t *)a_alloc(sizeof(parser_t));

    if (!init_parser(parser, tokens, tokens_count))
    {

        free(parser);

        return NULL;

    }

    return parser;

}

bool init_parser(parser_t *parser, token_t **tokens, size_t tokens_count)
{

    if (!tokens || !tokens_count)
    {

        return false;

    }

    parser->tokens = tokens;

    parser->tokens_count = tokens_count;

    parser->token_index = 0;

    parser_current_token(parser);

    return true;

}

bool destroy_parser(parser_t *parser)
{

    if (!parser)
    {

        return false;

    }

    if (parser->tokens)
    {

        for (size_t q = 0; q < parser->tokens_count; q++)
        {

            destroy_token(parser->tokens[q]);

        }

        free(parser->tokens);

    }

    free(parser);

}

ast_node_t *parser_parse(parser_t *parser)
{

    abort_if_null(parser);

    ast_node_t *root_node = parser_parse_statements(parser);

    if (!root_node)
    {

        err("\nTODO: ");

    }

    if      (!parser->token)
    {

        war("\nTODO: ");

    }
    else if (parser->token->type != TOKEN_TYPE_EOS)
    {

        err("\nTODO: ");

    }

    return root_node;

}

ast_node_t *parser_parse_statements(parser_t *parser)
{

    abort_if_null(parser);

    ast_node_t *statements_node = create_ast_node(AST_NODE_TYPE_STATEMENTS);

    while (parser->token && parser->token->type != TOKEN_TYPE_EOS)
    {

        ast_node_t *node = parser_parse_statement(parser);

        if (!node)
        {

            break;

        }

        ast_node_statements_push(statements_node, node);

        if (node->type == AST_NODE_TYPE_INVALID)
        {

            node->invalid_root_node = statements_node;

            statements_node = node;

            break;

        }

    }

    return statements_node;

}

ast_node_t *parser_parse_statement(parser_t *parser)
{

    abort_if_null(parser);

    if (!parser->token)
    {

        return NULL;

    }

    switch (parser->token->type)
    {

    case TOKEN_TYPE_KEYWORD_VARIABLE:
        return parser_parse_variable(parser);

    case TOKEN_TYPE_KEYWORD_FUNCTION:
        return parser_parse_function(parser);

    case TOKEN_TYPE_RIGHT_PAREN:
    case TOKEN_TYPE_RIGHT_BRACKET:
    case TOKEN_TYPE_RIGHT_BRACE:
    case TOKEN_TYPE_EOS:
        return NULL;

    default:

        print_token(parser->token, true);

        err("\tWhen parsing, invalid token type %d.\n", parser->token->type);

        break;

    }

    return create_ast_node(AST_NODE_TYPE_INVALID);

}

ast_node_t *parser_parse_variable(parser_t *parser)
{

    abort_if_null(parser);

    parser_eat_token(parser, TOKEN_TYPE_KEYWORD_VARIABLE);

    token_t *variable_name = parser_eat_token(parser, TOKEN_TYPE_IDENTIFIER);

    ast_node_t *variable_node = create_ast_node(AST_NODE_TYPE_VARIABLE_DEC);

    variable_node->variable_name = variable_name->value;

    parser_eat_token(parser, TOKEN_TYPE_COLON);

    if (!parser_parse_type_kind(parser, &variable_node->variable_type_kind, &variable_node->variable_type_name))
    {

        err("\tTODO: ");

    }

    if (!parser->token)
    {

        err("\tTODO: ");
    
    }

    switch (parser->token->type)
    {

    case TOKEN_TYPE_SEMICOLON:
        
        parser_next_token(parser);
        
        break;
    
    case TOKEN_TYPE_EQUAL:

        parser_next_token(parser);

        variable_node->type = AST_NODE_TYPE_VARIABLE_DEF;

        variable_node->variable_expr = parser_parse_expression(parser);

        /*TODO: */

        parser_eat_token(parser, TOKEN_TYPE_SEMICOLON);

        break;

    default:
    
        err("\tTODO: ");
    
        break;
    
    }

    return variable_node;

}

ast_node_t *parser_parse_function(parser_t *parser)
{

    abort_if_null(parser);

    parser_eat_token(parser, TOKEN_TYPE_KEYWORD_FUNCTION);

    token_t *function_name = parser_eat_token(parser, TOKEN_TYPE_IDENTIFIER);

    ast_node_t *function_node = create_ast_node(AST_NODE_TYPE_FUNCTION_DEC);

    function_node->function_name = function_name->value;

    parser_eat_token(parser, TOKEN_TYPE_LEFT_PAREN);

    function_node->function_parameters = parser_parse_statements(parser);

    /*TODO: .*/

    parser_eat_token(parser, TOKEN_TYPE_RIGHT_PAREN);

    parser_eat_token(parser, TOKEN_TYPE_COLON);

    if (!parser_parse_type_kind(parser, &function_node->function_return_type_kind, &function_node->function_return_type_name))
    {

        err("\tTODO: ");

    }

    if (!parser->token)
    {

        err("\tTODO: ");
    
    }

    switch (parser->token->type)
    {

    case TOKEN_TYPE_SEMICOLON:
        
        parser_next_token(parser);
        
        break;
    
    case TOKEN_TYPE_LEFT_BRACE:
        
        parser_next_token(parser);

        function_node->type = AST_NODE_TYPE_FUNCTION_DEF;

        function_node->function_body = parser_parse_statements(parser);
        
        parser_eat_token(parser, TOKEN_TYPE_RIGHT_BRACE);

        break;

    default:
    
        err("\tTODO: ");
    
        break;
    
    }

    return function_node;

}

type_kind_t parser_token_to_type_kind(parser_t *parser)
{

    abort_if_null(parser);

    abort_if_null(parser->token);

    switch (parser->token->type)
    {

    case TOKEN_TYPE_KEYWORD_VOID: return TYPE_KIND_VOID;

    case TOKEN_TYPE_KEYWORD_INT8 : return TYPE_KIND_INT8 ;
    case TOKEN_TYPE_KEYWORD_INT16: return TYPE_KIND_INT16;
    case TOKEN_TYPE_KEYWORD_INT32: return TYPE_KIND_INT32;
    case TOKEN_TYPE_KEYWORD_INT64: return TYPE_KIND_INT64;

    case TOKEN_TYPE_KEYWORD_FLOAT32: return TYPE_KIND_FLOAT32;
    case TOKEN_TYPE_KEYWORD_FLOAT64: return TYPE_KIND_FLOAT64;

    }

    return TYPE_KIND_UNKNOWN;

}

bool parser_parse_type_kind(parser_t *parser, type_kind_t *type_kind, char **type_name)
{

    abort_if_null(parser);

    abort_if_null(parser->token);

    abort_if_null(type_kind);

    abort_if_null(type_name);

    *type_kind = parser_token_to_type_kind(parser);

    if (*type_kind == TYPE_KIND_UNKNOWN)
    {

        if (parser->token->type != TOKEN_TYPE_IDENTIFIER)
        {

            return false;

        }

        *type_name = parser->token->value;

    }

    parser_next_token(parser);

    return true;

}

ast_node_t *parser_parse_expression(parser_t *parser)
{

    abort_if_null(parser);

    return parser_parse_factor(parser);

}

ast_node_t *parser_parse_factor(parser_t *parser)
{

    abort_if_null(parser);

    switch (parser->token->type)
    {

    case TOKEN_TYPE_LITERAL_NUMBER:
        
        return parser_parse_number(parser);
        
    default:

        print_token(parser->token, true);

        err("\tTODO: .\n");

        break;
    
    }

    return create_ast_node(AST_NODE_TYPE_INVALID);

}

ast_node_t *parser_parse_number(parser_t *parser)
{

    abort_if_null(parser);

    token_t *token_number = parser_eat_token(parser, TOKEN_TYPE_LITERAL_NUMBER);

    ast_node_t *node_literal = create_ast_node(AST_NODE_TYPE_LITERAL);

    char *number_value = token_number->value;

    size_t token_number_len = strlen(number_value);

    size_t number_base = 10;

    if (token_number_len > 2 && number_value[0] == '0')
    {

        switch (number_value[1])
        {

        case 'B':
        case 'b':

            number_base = 2 ;

            break;

        case 'H':
        case 'h':

            number_base = 16;

            break;
    
        case 'O':
        case 'o':

            number_base = 8 ;

            break;

        }

    }

    char *number_end = NULL;

    if (utf8_strchr(token_number->value, (uint32_t)'.'))
    {

        float single_number = strtof((const char *)number_value, &number_end);

        if (number_end[0] == '\0')
        {

            node_literal->literal_type_kind = TYPE_KIND_FLOAT32;

            /*TODO: */

            return node_literal;

        }

        double double_number = strtod((const char *)number_value, &number_end);

        if (number_end[0] == '\0')
        {
        
            node_literal->literal_type_kind = TYPE_KIND_FLOAT64;

            /*TODO: */

            return node_literal;

        }

        err("TODO: ");

    }
    else
    {

        switch (number_base)
        {

        case 2 :

            number_value += 2;

            break;

        case 16:

            number_value[1] = 'x';

            break;

        case 8 :

            number_value += 1;

            number_value[0] = '0';

            break;

        }

        unsigned long long integer_number = strtoull((const char *)number_value, &number_end, number_base);

        if (number_end[0] != '\0')
        {

            err("TODO: ");

        }

        /*TODO: */

    }

    return node_literal;

}
void parser_current_token_index(parser_t *parser, size_t index)
{

    abort_if_null(parser);

    if (index >= parser->tokens_count)
    {

        parser->token = NULL;

        return;

    }

    parser->token = parser->tokens[index];

}

void parser_current_token(parser_t *parser)
{

    abort_if_null(parser);

    parser_current_token_index(parser, parser->token_index);

}

void parser_next_token(parser_t *parser)
{

    abort_if_null(parser);

    if (parser->token_index < parser->tokens_count)
    {

        parser->token_index++;

    }

    parser_current_token(parser);

}

token_t *parser_eat_token(parser_t *parser, token_type_t expected_type)
{

    abort_if_null(parser);

    abort_if_null(parser->token);

    if (parser->token->type == expected_type)
    {

        token_t *token = parser->token;

        parser_next_token(parser);

        return token;

    }

    err (
            "\tWhen parsing, unexpected token type: %s, was expecting: %s.\n",
            token_type_to_string(parser->token->type), token_type_to_string(expected_type)
        );

    return NULL;

}
