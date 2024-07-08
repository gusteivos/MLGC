#include "ast.h"
#include "util.h"


ast_node_t *create_ast_node(ast_node_type_t type)
{

    ast_node_t *node = (ast_node_t *)a_alloc(sizeof(ast_node_t));

    node->type = type;

    switch (node->type)
    {

    case AST_NODE_TYPE_INVALID:

        node->invalid_root_node = NULL;

        break;

    case AST_NODE_TYPE_STATEMENTS:

        node->statements = NULL;

        node->statements_count = 0;

        break;

    case AST_NODE_TYPE_VARIABLE_DEC:
    case AST_NODE_TYPE_VARIABLE_DEF:

        node->variable_name = NULL;

        node->variable_type_kind = TYPE_KIND_UNKNOWN;

        node->variable_type_name = NULL;

        node->variable_expr = NULL;

        break;

    case AST_NODE_TYPE_FUNCTION_DEC:
    case AST_NODE_TYPE_FUNCTION_DEF:

        node->function_name = NULL;

        node->function_parameters = NULL;

        node->function_return_type_kind = TYPE_KIND_UNKNOWN;

        node->function_return_type_name = NULL;

        node->function_body = NULL;

        break;

    }

    return node;

}

bool ast_node_statements_push(ast_node_t *statements_node, ast_node_t *node)
{

    abort_if_null(statements_node);

    statements_node->statements_count++;

    statements_node->statements = (struct ast_node_s **)a_realloc(statements_node->statements, statements_node->statements_count * sizeof(struct ast_node_s *));

    statements_node->statements[statements_node->statements_count - 1] = node;

    return true;

}

bool ast_node_statements_pop(ast_node_t *statements_node, ast_node_t **node)
{

    abort_if_null(statements_node);

    abort_if_null(node);

    if (statements_node->statements_count == 0)
    {

        return false;

    }

    *node = statements_node->statements[0];

    for (size_t q = 1; q < statements_node->statements_count; ++q)
    {

        statements_node->statements[q - 1] = statements_node->statements[q];

    }

    statements_node->statements_count--;

    statements_node->statements = (struct ast_node_s **)a_realloc(statements_node->statements, statements_node->statements_count * sizeof(struct ast_node_s *));

    return true;

}

bool destroy_ast_node(ast_node_t *node)
{

    if (!node)
    {

        return false;

    }

    switch (node->type)
    {

    case AST_NODE_TYPE_STATEMENTS:

        if (node->statements)
        {

            free(node->statements);

        }

        break;

    }

    free(node);

    return true;

}
