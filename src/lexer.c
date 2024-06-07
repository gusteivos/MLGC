#include "lexer.h"
#include "util.h"


token_mapping_t lexer_token_keyword_mappings[] =
{

    { TOKEN_TYPE_KEYWORD_VARIABLE, "var" , "variable" },
    { TOKEN_TYPE_KEYWORD_FUNCTION, "func", "function" },
    { TOKEN_TYPE_EOS, NULL, NULL }

};


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

    lexer->source_location.column = 0;

    lexer_current_char(lexer);

    lexer_update_source_location(lexer);

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

        if (lexer_parse_char_sequence (lexer, token))
        {

            return token;

        }

        if (lexer_parse_digit_sequence(lexer, token))
        {

            return token;

        }

        if (lexer->source_char == '#')
        {

            lexer_skip_line(lexer);

            continue;

        }

        if (lexer->source_char == '$')
        {

            fprint_location(stderr, &lexer->source_location);

            war("\tLine for preprocessor not processed, skipping the line.\n");

            lexer_skip_line(lexer);

            continue;

        }

        switch (lexer->source_char)
        {

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
            token->type = TOKEN_TYPE_TILDE;
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

        default:
            {

                fprint_location(stderr, &lexer->source_location);

                err("\tWhen lexing invalid char \'%c\'.\n", lexer->source_char);

            }
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

        lexer->source_location.column  = 0;

        break;

    case '\t':

        lexer->source_location.column += LEXER_TAB_WIDTH;

        break;

    default:

        lexer->source_location.column += 1;

        break;

    }

}

void lexer_current_char(lexer_t *lexer)
{

    lexer->source_char = (lexer->source_location.index < lexer->source_length) ?
                         lexer->source[lexer->source_location.index] : '\0';

}

void lexer_next_char(lexer_t *lexer)
{

    if (lexer->source_location.index < lexer->source_length)
    {

        lexer->source_location.index++;

        lexer_current_char(lexer);

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

    lexer->source_char = (peek_index < lexer->source_length) ?
                         lexer->source[peek_index] : '\0';

}


void lexer_skip_line(lexer_t *lexer)
{

    size_t line = lexer->source_location.line;

    while
    (
        lexer->source_char &&
        line == lexer->source_location.line
    )
    {

        lexer_next_char(lexer);

    }

}

void lexer_skip_chars_considered_whitespace(lexer_t *lexer)
{

    while
    (
        lexer->source_char &&
        strchr(LEXER_CHARS_CONSIDERED_WHITESPACE, (int)lexer->source_char)
    )
    {

        lexer_next_char(lexer);

    }

}


token_t *lexer_lex2(lexer_t *lexer, token_t *token)
{

    char c = lexer->source_char;

    lexer_next_char(lexer);

    switch (c)
    {

    case '+':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_PLUS_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_PLUS;
            break;

        }
        break;
    case '-':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_MINUS_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_MINUS;
            break;

        }
        break;
    case '*':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_ASTERISK_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_ASTERISK;
            break;

        }
        break;
    case '/':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_SLASH_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_SLASH;
            break;

        }
        break;
    case '%':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_PERCENT_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_PERCENT;
            break;

        }
        break;
    case '^':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_CARET_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_CARET;
            break;

        }
        break;
    case '!':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_EXCLAMATION_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_EXCLAMATION;
            break;

        }
        break;
    case '=':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_EQUAL_EQUAL;
            lexer_next_char(lexer);
            break;

        default:
            token->type = TOKEN_TYPE_EQUAL;
            break;

        }
        break;

    default:
        abort_if_null(NULL);

    }

    return token;

}

token_t *lexer_lex3(lexer_t *lexer, token_t *token)
{

    char c = lexer->source_char;

    lexer_next_char(lexer);

    switch (c)
    {

    case '<':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_LESS_THAN_EQUAL;
            lexer_next_char(lexer);
            break;

        case '<':
            {

                lexer_next_char(lexer);

                token->type = (lexer->source_char == '=') ? TOKEN_TYPE_LEFT_SHIFT_EQUAL : TOKEN_TYPE_LEFT_SHIFT;

                if (lexer->source_char == '=')
                {

                    lexer_next_char(lexer);

                }

            }
            break;

        default:
            token->type = TOKEN_TYPE_LESS_THAN;
            break;

        }
        break;
    case '>':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_GREATER_THAN_EQUAL;
            lexer_next_char(lexer);
            break;

        case '>':
            {

                lexer_next_char(lexer);

                token->type = (lexer->source_char == '=') ? TOKEN_TYPE_RIGHT_SHIFT_EQUAL : TOKEN_TYPE_RIGHT_SHIFT;

                if (lexer->source_char == '=')
                {

                    lexer_next_char(lexer);

                }

            }
            break;

        default:
            token->type = TOKEN_TYPE_GREATER_THAN;
            break;

        }
        break;
    case '&':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_AMPERSAND_EQUAL;
            lexer_next_char(lexer);
            break;

        case '&':
            {

                lexer_next_char(lexer);

                token->type = (lexer->source_char == '=') ? TOKEN_TYPE_LOGICAL_AND_EQUAL : TOKEN_TYPE_LOGICAL_AND;

                if (lexer->source_char == '=')
                {

                    lexer_next_char(lexer);

                }

            }
            break;

        default:
            token->type = TOKEN_TYPE_AMPERSAND;
            break;

        }
        break;

    case '|':
        switch (lexer->source_char)
        {

        case '=':
            token->type = TOKEN_TYPE_PIPE_EQUAL;
            lexer_next_char(lexer);
            break;

        case '|':
            {

                lexer_next_char(lexer);

                token->type = (lexer->source_char == '=') ? TOKEN_TYPE_LOGICAL_OR_EQUAL : TOKEN_TYPE_LOGICAL_OR;

                if (lexer->source_char == '=')
                {

                    lexer_next_char(lexer);

                }

            }
            break;

        default:
            token->type = TOKEN_TYPE_PIPE;
            break;

        }
        break;

    default:
        abort_if_null(NULL);

    }

    return token;
}


char *lexer_parse_sequence(lexer_t *lexer, int (*char_rule)(int), char *others, size_t max_length)
{

    char *str = NULL;

    size_t str_len = 0;

    while (
        lexer->source_char != '\0' &&
        (char_rule((int)lexer->source_char) ||
        (str_len != 0 && strchr(others, (int)lexer->source_char))) &&
        str_len < max_length
    )
    {

        str = (char *)a_realloc(str, (str_len + 1) * sizeof(char));

        str[str_len++] = lexer->source_char;

        lexer_next_char(lexer);

    }

    if (str != NULL)
    {

        str[str_len] = '\0';

    }

    return str;

}

bool lexer_parse_char_sequence(lexer_t *lexer, token_t *token)
{

    char *token_valeu = lexer_parse_sequence(lexer, isalpha, LEXER_PARSE_CHAR_SEQUENCE_OTHERS, __SIZE_MAX__);

    if (token_valeu == NULL)
    {

        return false;

    }

    token->type = TOKEN_TYPE_IDENTIFIER;

    token->value = token_valeu;

    size_t mapping_index = 0;

    token_mapping_t current_mapping = lexer_token_keyword_mappings[mapping_index];

    while
    (
        (
            current_mapping.value0 != NULL ||
            current_mapping.value1 != NULL
        ) &&
        current_mapping.type != TOKEN_TYPE_EOS
    )
    {

        bool mapped = false;

        if (!token_mapping_value_token_type(&current_mapping, token, &mapped, false))
        {

            abort_if_null(NULL);

        }

        if (mapped)
        {

            break;

        }

        mapping_index++;

        current_mapping = lexer_token_keyword_mappings[mapping_index];

    }

    return true;

}

bool lexer_parse_digit_sequence(lexer_t *lexer, token_t *token)
{

    char *token_valeu = lexer_parse_sequence(lexer, isdigit, LEXER_PARSE_DIGIT_SEQUENCE_OTHERS, __SIZE_MAX__);

    if (token_valeu == NULL)
    {

        return false;

    }

    token->type = TOKEN_TYPE_LITERAL_NUMBER;

    token->value = token_valeu;

    return true;

}
