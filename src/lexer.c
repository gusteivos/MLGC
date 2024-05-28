#include "lexer.h"
#include "util.h"


static void lexer_update_source_location(lexer_t *lexer)
{

    abort_if_null((void *)lexer);

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

static void lexer_next_char(lexer_t *lexer)
{

    abort_if_null((void *)lexer);

    if (lexer->source_location.index < lexer->source_length)
    {

        lexer->source_char = lexer->source[lexer->source_location.index++];

        lexer_update_source_location(lexer);

    }

}

static void lexer_peek_char(lexer_t *lexer, size_t offset)
{

    abort_if_null((void *)lexer);

    size_t peek_index = lexer->source_location.index + offset;

    if (peek_index >= lexer->source_length)
    {

        lexer->source_char = '\0';

        return;
    }

    lexer->source_char = lexer->source[peek_index];

}


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

    abort_if_null((void *)lexer);

    token_t *token = create_token(TOKEN_TYPE_INVALID, NULL, (location_t){ 0, 0, 0 });

    do
    {

        token->location = lexer->source_location;

        switch (lexer->source_char)
        {

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
