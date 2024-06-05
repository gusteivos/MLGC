#include "token.h"
#include "util.h"


char *token_type_to_string(token_type_t type)
{

    switch (type)
    {

    case TOKEN_TYPE_INVALID: return "INVALID";

    case TOKEN_TYPE_IDENTIFIER: return "IDENTIFIER";

    case TOKEN_TYPE_LITERAL_NUMBER: return "LITERAL NUMBER";

    case TOKEN_TYPE_KEYWORD_VARIABLE: return "VARIABLE KEYWORD";
    case TOKEN_TYPE_KEYWORD_FUNCTION: return "FUNCTION KEYWORD";

    case TOKEN_TYPE_PLUS: return "PLUS";               /* + */
    case TOKEN_TYPE_MINUS: return "MINUS";             /* - */
    case TOKEN_TYPE_ASTERISK: return "ASTERISK";       /* * */
    case TOKEN_TYPE_SLASH: return "SLASH";             /* / */
    case TOKEN_TYPE_PERCENT: return "PERCENT";         /* % */
    case TOKEN_TYPE_CARET: return "CARET";             /* ^ */
    case TOKEN_TYPE_EXCLAMATION: return "EXCLAMATION"; /* ! */
    case TOKEN_TYPE_EQUAL: return "EQUAL";             /* = */
    case TOKEN_TYPE_TILDE: return "TILDE";             /* ~ */

    case TOKEN_TYPE_LESS_THAN: return "LESS THAN";       /* < */
    case TOKEN_TYPE_GREATER_THAN: return "GREATER THAN"; /* > */
    case TOKEN_TYPE_AMPERSAND: return "AMPERSAND";       /* & */
    case TOKEN_TYPE_PIPE: return "PIPE";                 /* | */

    case TOKEN_TYPE_DOT: return "DOT";
    case TOKEN_TYPE_COMMA: return "COMMA";
    case TOKEN_TYPE_COLON: return "COLON";
    case TOKEN_TYPE_SEMICOLON: return "SEMICOLON";

    case TOKEN_TYPE_LEFT_BRACKET: return "LEFT BRACKET";   /* [ */
    case TOKEN_TYPE_RIGHT_BRACKET: return "RIGHT BRACKET"; /* ] */
    case TOKEN_TYPE_LEFT_BRACE: return "LEFT BRACE";       /* { */
    case TOKEN_TYPE_RIGHT_BRACE: return "RIGHT BRACE";     /* } */
    case TOKEN_TYPE_LEFT_PAREN: return "LEFT PAREN";       /* ( */
    case TOKEN_TYPE_RIGHT_PAREN: return "RIGHT PAREN";     /* ) */

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

bool token_type_token_mapping_value(token_t *token, token_mapping_t *token_mapping, bool *mapped, bool use_value)
{

    if (!token || !token_mapping)
    {

        return false;

    }

    if (token->type == token_mapping->type)
    {

        if (token->value != NULL)
        {

            free(token->value);

            token->value = NULL;

        }

        if (use_value)
        {

            token->value = a_duplicate_string(token_mapping->value0 ? token_mapping->value0 : token_mapping->value1);

        }

        if (mapped != NULL)
        {

            *mapped = true;

        }

        return true;

    }

    if (mapped != NULL)
    {

        *mapped = false;

    }

    return true;
}

bool token_mapping_value_token_type(token_mapping_t *token_mapping, token_t *token, bool *mapped, bool use_value)
{

    if (!token_mapping || !token)
    {

        return false;

    }

    if
    (
        token->value &&
        (token_mapping->value0 && strcmp(token->value, token_mapping->value0) == 0) ||
        (token_mapping->value1 && strcmp(token->value, token_mapping->value1) == 0)
    )
    {

        free(token->value);

        token->value = NULL;

        token->type = token_mapping->type;

        if (use_value)
        {

            token->value = a_duplicate_string(token_mapping->value0 ? token_mapping->value0 : token_mapping->value1);

        }

        if (mapped != NULL)
        {

            *mapped = true;

        }

        return true;

    }

    if (mapped != NULL)
    {

        *mapped = false;

    }

    return true;

}
