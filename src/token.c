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

    case TOKEN_TYPE_KEYWORD_VOID: return "VOID KEYWORD";

    case TOKEN_TYPE_KEYWORD_INT8: return "INT8 KEYWORD";
    case TOKEN_TYPE_KEYWORD_INT16: return "INT16 KEYWORD";
    case TOKEN_TYPE_KEYWORD_INT32: return "INT32 KEYWORD";
    case TOKEN_TYPE_KEYWORD_INT64: return "INT64 KEYWORD";

    case TOKEN_TYPE_KEYWORD_FLOAT32: return "FLOAT32 KEYWORD";
    case TOKEN_TYPE_KEYWORD_FLOAT64: return "FLOAT64 KEYWORD";

    case TOKEN_TYPE_KEYWORD_SIGNED: return "SIGNED KEYWORD";
    case TOKEN_TYPE_KEYWORD_UNSIGNED: return "UNSIGNED KEYWORD";

    case TOKEN_TYPE_KEYWORD_SIZEOF: return "SIZEOF KEYWORD";
    case TOKEN_TYPE_KEYWORD_KINDOF: return "KINDOF KEYWORD";

    case TOKEN_TYPE_PLUS: return "PLUS";               /* + */
    case TOKEN_TYPE_MINUS: return "MINUS";             /* - */
    case TOKEN_TYPE_ASTERISK: return "ASTERISK";       /* * */
    case TOKEN_TYPE_SLASH: return "SLASH";             /* / */
    case TOKEN_TYPE_PERCENT: return "PERCENT";         /* % */
    case TOKEN_TYPE_CARET: return "CARET";             /* ^ */
    case TOKEN_TYPE_EXCLAMATION: return "EXCLAMATION"; /* ! */
    case TOKEN_TYPE_EQUAL: return "EQUAL";             /* = */
    case TOKEN_TYPE_TILDE: return "TILDE";             /* ~ */

    case TOKEN_TYPE_PLUS_EQUAL: return "PLUS EQUAL";
    case TOKEN_TYPE_MINUS_EQUAL: return "MINUS EQUAL";
    case TOKEN_TYPE_ASTERISK_EQUAL: return "ASTERISK EQUAL";
    case TOKEN_TYPE_SLASH_EQUAL: return "SLASH EQUAL";
    case TOKEN_TYPE_PERCENT_EQUAL: return "PERCENT EQUAL";
    case TOKEN_TYPE_CARET_EQUAL: return "CARET EQUAL";
    case TOKEN_TYPE_EXCLAMATION_EQUAL: return "EXCLAMATION EQUAL";
    case TOKEN_TYPE_EQUAL_EQUAL: return "EQUAL EQUAL";

    case TOKEN_TYPE_LESS_THAN: return "LESS THAN";       /* < */
    case TOKEN_TYPE_GREATER_THAN: return "GREATER THAN"; /* > */
    case TOKEN_TYPE_AMPERSAND: return "AMPERSAND";       /* & */
    case TOKEN_TYPE_PIPE: return "PIPE";                 /* | */

    case TOKEN_TYPE_LEFT_SHIFT: return "LEFT SHIFT";
    case TOKEN_TYPE_RIGHT_SHIFT: return "RIGHT SHIFT";
    case TOKEN_TYPE_LOGICAL_AND: return "LOGICAL AND";
    case TOKEN_TYPE_LOGICAL_OR: return "LOGICAL OR";

    case TOKEN_TYPE_LESS_THAN_EQUAL: return "LESS THAN EQUAL";
    case TOKEN_TYPE_GREATER_THAN_EQUAL: return "GREATER THAN EQUAL";
    case TOKEN_TYPE_AMPERSAND_EQUAL: return "AMPERSAND EQUAL";
    case TOKEN_TYPE_PIPE_EQUAL: return "PIPE EQUAL";

    case TOKEN_TYPE_LEFT_SHIFT_EQUAL: return "LEFT SHIFT EQUAL";
    case TOKEN_TYPE_RIGHT_SHIFT_EQUAL: return "RIGHT SHIFT EQUAL";
    case TOKEN_TYPE_LOGICAL_AND_EQUAL: return "LOGICAL AND EQUAL";
    case TOKEN_TYPE_LOGICAL_OR_EQUAL: return "LOGICAL OR EQUAL";

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

char *token_type_to_symbol(token_type_t type)
{

    switch (type)
    {

    /*TODO: .*/

    default: return token_type_to_string(type);

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

    abort_if_null(token);

    token->type = type;

    token->value = value;

    token->location = location;

    return true;

}

bool fprint_token(FILE *s, token_t *token)
{

    if (!s)
    {

        return true;

    }

    abort_if_null(token);

    int q = fprintf
    (
        s,
        "Token:\n\ttype: %d\n\ttype symbol: %s\n\tvalue: '%s'\n",
        token->type, token_type_to_symbol(token->type), token->value
    );

    bool w = fprint_location(s, &token->location);

    return !(q < 0) && w;

}

bool destroy_token(token_t *token)
{

    abort_if_null(token);

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
