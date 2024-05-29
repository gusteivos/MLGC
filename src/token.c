#include "token.h"
#include "util.h"


char *token_type_to_string(token_type_t type)
{

    switch (type)
    {

    case TOKEN_TYPE_INVALID: return "INVALID";

    case TOKEN_TYPE_LEFT_BRACKET: return "LEFT BRACKET";
    case TOKEN_TYPE_RIGHT_BRACKET: return "RIGHT BRACKET";
    case TOKEN_TYPE_LEFT_BRACE: return "LEFT BRACE";
    case TOKEN_TYPE_RIGHT_BRACE: return "RIGHT BRACE";
    case TOKEN_TYPE_LEFT_PAREN: return "LEFT PAREN";
    case TOKEN_TYPE_RIGHT_PAREN: return "RIGHT PAREN";

    case TOKEN_TYPE_EOS: return "END OF SOURCE";

    default: return "UNKNOWN";

    }

    return NULL;

}

token_t *create_token(token_type_t type, char *value, location_t location)
{

    token_t *token = (token_t *)a_alloc(sizeof(token_t));

    init_token(token, type, value, location);

    return token;

}

bool init_token(token_t *token, token_type_t type, char *value, location_t location)
{

    if (!token)
    {

        return false;

    }

    token->type = type;

    token->value = value ? a_duplicate_string(value) : value;

    token->location = location;

    return true;

}

bool print_token(token_t *token, bool token_type_name)
{

    if (!token)
    {

        fprintf(stderr, "Error token is NULL.\n");

        return false;

    }

    fprintf(stdout, "Token:\n");
    fprintf(stdout, "\ttype: %d\n", token->type);

    if (token_type_name)
    {

        fprintf(stdout, "\ttype to string: %s\n", token_type_to_string(token->type));

    }

    fprintf(stdout, "\tvalue: '%s'\n", token->value);

    fprint_location(stdout, &token->location);

    return true;

}

bool destroy_token(token_t *token)
{

    if (!token)
    {

        return false;

    }

    if (token->value)
    {

        free(token->value);

    }

    free(token);

    return true;

}
