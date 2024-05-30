#include "lexer.h"
#include "util.h"


lexer_t *create_lexer(char *source, size_t source_length)
{

    lexer_t *lexer = (lexer_t *)a_alloc(sizeof(lexer_t));

    if (!init_lexer(lexer, source, source_length))
    {

        free(lexer);

        return NULL;

    }

    return lexer;

}

bool init_lexer(lexer_t *lexer, char *source, size_t source_length)
{

    if (!lexer || !source)
    {

        return false;

    }

    if (source_length == 0)
    {

        source_length = strlen(source);

    }

    lexer->source = source;

    lexer->source_length = source_length;

    lexer->source_location.index = 0;

    lexer->source_location.line = 1;

    lexer->source_location.column = 1;

    lexer->source_char = source[lexer->source_location.index];

    return true;
}

token_t *lexer_lex(lexer_t *lexer)
{

    if (!lexer)
    {

        return NULL;

    }

    token_t *token = create_token(TOKEN_TYPE_INVALID, NULL, (location_t){ 0, 0, 0 });

    do
    {

        lexer_skip_chars_considered_whitespace(lexer);

        token->location = lexer->source_location;

        switch (lexer->source_char)
        {

        default:
            {

                return token;

            }
            break;

        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
        case '!':
        case '=':
            return lexer_lex2(lexer, token);
        case '~':
            /* TODO: */
            break;

        case '<':
        case '>':
        case '&':
        case '|':
            return lexer_lex3(lexer, token);

    	case '.':
            token->type = TOKEN_TYPE_DOT;
            break;
    	case ',':
            token->type = TOKEN_TYPE_COMMA;
            break;
        case ':':
            token->type = TOKEN_TYPE_COLON;
            break;
        case ';':
            token->type = TOKEN_TYPE_SEMICOLON;
            break;

        case '[':
            token->type = TOKEN_TYPE_LEFT_BRACKET;
            break;
        case ']':
            token->type = TOKEN_TYPE_RIGHT_BRACKET;
            break;
        case '{':
            token->type = TOKEN_TYPE_LEFT_BRACE;
            break;
        case '}':
            token->type = TOKEN_TYPE_RIGHT_BRACE;
            break;
        case '(':
            token->type = TOKEN_TYPE_LEFT_PAREN;
            break;
        case ')':
            token->type = TOKEN_TYPE_RIGHT_PAREN;
            break;

        case '\0':
            token->type = TOKEN_TYPE_EOS;
            break;

        }

        lexer_next_char(lexer);

    } while (token->type == TOKEN_TYPE_INVALID);

    return token;

}

bool reset_lexer(lexer_t *lexer, char *source, size_t source_length)
{

    if (!lexer)
    {

        return false;

    }

    if (!source)
    {

        source = lexer->source;

        source_length = lexer->source_length;

    }

    return init_lexer(lexer, source, source_length);

}

bool destroy_lexer(lexer_t *lexer)
{

    if (!lexer)
    {

        return false;

    }

    free(lexer);

    return true;

}


void lexer_update_source_location(lexer_t *lexer)
{

    switch (lexer->source_char)
    {

    case '\n':

        lexer->source_location.line++;

        lexer->source_location.column  = 1;

        break;

    case '\t':

        lexer->source_location.column += LEXER_TAB_WIDTH;

        break;

    default:

        lexer->source_location.column += 1;

        break;

    }

}

void lexer_next_char(lexer_t *lexer)
{

    if (lexer->source_location.index < lexer->source_length)
    {

        lexer->source_location.index++;

        if (lexer->source_location.index >= lexer->source_length)
        {

            lexer->source_char = '\0';

        }
        else
        {

            lexer->source_char = lexer->source[lexer->source_location.index];

        }

        lexer_update_source_location(lexer);

    }
    else
    {

        lexer->source_char = '\0';

    }

}

void lexer_peek_char(lexer_t *lexer, size_t offset)
{

    size_t peek_index = lexer->source_location.index + offset;

    if (peek_index >= lexer->source_length)
    {

        lexer->source_char = '\0';

    }
    else
    {

        lexer->source_char = lexer->source[peek_index];

    }

}

void lexer_skip_chars_considered_whitespace(lexer_t *lexer)
{

    while (lexer->source_char && strchr(LEXER_CHARS_CONSIDERED_WHITESPACE, (int)lexer->source_char))
    {

        lexer_next_char(lexer);

    }

}

token_t *lexer_lex2(lexer_t *lexer, token_t *token)
{

    /* TODO: */

    return token;

}

token_t *lexer_lex3(lexer_t *lexer, token_t *token)
{

    /* TODO: */

    return token;

}