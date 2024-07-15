#include "lexer.h"
#include "util.h"


token_mapping_t lexer_token_keyword_mappings[] =
{

    { TOKEN_TYPE_KEYWORD_VARIABLE, "var" , "variable" },
    { TOKEN_TYPE_KEYWORD_FUNCTION, "func", "function" },

    { TOKEN_TYPE_KEYWORD_VOID , "void" , NULL },

    { TOKEN_TYPE_KEYWORD_INT8 , "byte" , "int8"  },
    { TOKEN_TYPE_KEYWORD_INT16, "short", "int16" },
    { TOKEN_TYPE_KEYWORD_INT32, "int"  , "int32" },
    { TOKEN_TYPE_KEYWORD_INT64, "long" , "int64" },

    { TOKEN_TYPE_KEYWORD_FLOAT32, "single", "float32" },
    { TOKEN_TYPE_KEYWORD_FLOAT64, "double", "float64" },

    { TOKEN_TYPE_KEYWORD_SIGNED  , "signed"  , NULL },
    { TOKEN_TYPE_KEYWORD_UNSIGNED, "unsigned", NULL },

    { TOKEN_TYPE_KEYWORD_SIZEOF, "sizeof", NULL },
    { TOKEN_TYPE_KEYWORD_KINDOF, "kindof", NULL },

    { TOKEN_TYPE_EOS, NULL, NULL }

};


lexer_t *create_lexer(const char *source_filename, char *source, size_t source_length)
{

    lexer_t *lexer = (lexer_t *)a_alloc(sizeof(lexer_t));

    init_lexer(lexer, source_filename, source, source_length);
    
    return lexer;

}

bool init_lexer(lexer_t *lexer, const char *source_filename, char *source, size_t source_length)
{

    abort_if_null(lexer);

    abort_if_null((char *)source_filename);

    abort_if_null(source);

    if (source_length == 0)
    {

        source_length = strlen(source);

    }

    lexer->source_filename = source_filename;

    lexer->source = source;

    lexer->source_length = source_length;

    lexer->source_location.index = 0;

    lexer->source_location.line = 1;

    lexer->source_location.column = 0;

    lexer_current_char(lexer);

    lexer_update_source_location(lexer);

    init_buffer(&lexer->buffer, LEXER_BUFFER_INITIAL_CAPACITY);

    lexer->use_buffer = false;

    lexer->tab_width = LEXER_TAB_WIDTH;

    lexer->chars_considered_whitespace = LEXER_CHARS_CONSIDERED_WHITESPACE;

    return true;

}

token_t *lexer_lex(lexer_t *lexer)
{

    abort_if_null(lexer);

    bool exit = false;

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

        if (lexer->source_char == '@')
        {

            if (lexer_skip_block_comment(lexer))
            {

                continue;

            }

            if (lexer->source_char == '\0')
            {

                err("\tTODO: .\n");

            }

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

        case '#':
            lexer_skip_line_comment(lexer);
            continue;

        case '$':
            {

                fprint_location(stderr, &lexer->source_location);

                war("\tLine for preprocessor not processed, skipping the line.\n");

                lexer_skip_line(lexer);

            }
            continue;

        case '\0':
            token->type = TOKEN_TYPE_EOS;
            break;

        default:
            {

                fprint_location(stderr, &lexer->source_location);

                char str[] = { '\0', '\0', '\0', '\0', '\0' };

                size_t utf8_encode_size = utf8_encode(str, sizeof(str), lexer->source_char);

                err("\tWhen lexing invalid char \'%s\'.\n", utf8_encode_size ? str : "invalid and not encodable char");

                exit = true;

            }
            continue;

        }

        lexer_next_char(lexer);

    } while (token->type == TOKEN_TYPE_INVALID && !exit);

    return token;

}

bool lexer_lex_all(lexer_t *lexer, token_t ***tokens, size_t *tokens_count)
{

    abort_if_null(lexer);

    abort_if_null(tokens_count);

    token_t *token = NULL;

    do
    {

        token = lexer_lex(lexer);

        (*tokens_count)++;

        *tokens = (token_t **)a_realloc(*tokens, *tokens_count * sizeof(token_t *));

        (*tokens)[*tokens_count - 1] = token;

        if (token->type == TOKEN_TYPE_INVALID)
        {

            return false;

        }

    } while (token && token->type != TOKEN_TYPE_EOS);

    return true;

}

bool destroy_lexer(lexer_t *lexer)
{

    abort_if_null(lexer);

    if (lexer->buffer.data)
    {

        free(lexer->buffer.data);

    }

    free(lexer);

    return true;

}


void lexer_update_source_location(lexer_t *lexer)
{

    abort_if_null(lexer);

    switch (lexer->source_char)
    {

    case '\n':

        lexer->source_location.line++;

        lexer->source_location.column  = 0;

        break;

    case '\t':

        lexer->source_location.column += lexer->tab_width;

        break;

    default:

        lexer->source_location.column += 1;

        break;

    }

}

void lexer_current_char_index(lexer_t *lexer, size_t index)
{

    abort_if_null(lexer);

    char *source_index = (char *)&lexer->source[index];

    if (index >= lexer->source_length)
    {

        lexer->source_char_size = 1;

        lexer->source_char = '\0';

        return;

    }

    size_t source_len = (lexer->source_length - lexer->source_location.index) + 1;

    lexer->source_char_size = utf8_decode(&lexer->source_char, source_index, source_len);

    if (lexer->source_char_size == 0)
    {

        fprint_location(stderr, &lexer->source_location);

        err("\tWhen lexing encoding not compatible with utf8.\n");

    }

    if (lexer->source_char_size == UTF8_INVALID)
    {

        fprint_location(stderr, &lexer->source_location);

        err("\tWhen lexing utf8 decoding invalid.\n");

    }

}

void lexer_current_char(lexer_t *lexer)
{

    abort_if_null(lexer);

    lexer_current_char_index(lexer, lexer->source_location.index);

}

void lexer_next_char(lexer_t *lexer)
{

    abort_if_null(lexer);

    if (lexer->use_buffer)
    {

        char str[] = { '\0', '\0', '\0', '\0', '\0' };

        size_t utf8_encode_size = utf8_encode(str, sizeof(str) - 1, lexer->source_char);
        
        buffer_add_string(&lexer->buffer, str);

    }

    if (lexer->source_location.index < lexer->source_length)
    {

        lexer->source_location.index += lexer->source_char_size;

    }

    lexer_current_char(lexer);

    lexer_update_source_location(lexer);

}

void lexer_peek_char(lexer_t *lexer, size_t offset)
{

    abort_if_null(lexer);

    size_t peek_index = lexer->source_location.index + offset;

    lexer_current_char_index(lexer, peek_index);

}


bool lexer_skip_line(lexer_t *lexer)
{

    abort_if_null(lexer);

    size_t line = lexer->source_location.line;

    while
    (
        lexer->source_char &&
        line == lexer->source_location.line
    )
    {

        lexer_next_char(lexer);

    }

    return true;

}

bool lexer_skip_chars_considered_whitespace(lexer_t *lexer)
{

    abort_if_null(lexer);

    while
    (
        lexer->source_char &&
        utf8_strchr((const char *)lexer->chars_considered_whitespace, lexer->source_char)
    )
    {

        lexer_next_char(lexer);

    }

    return true;

}

bool lexer_skip_line_comment(lexer_t *lexer)
{

    return lexer_skip_line(lexer);

}

bool lexer_skip_block_comment(lexer_t *lexer)
{

    abort_if_null(lexer);

    if (lexer->source_char != '@')
    {

        return false;
        
    }

    lexer_peek_char(lexer, 1);

    if (lexer->source_char != '#')
    {

        return false;

    }

    lexer_next_char(lexer);

    uint32_t last_source_char;

    do
    {
    
        last_source_char = lexer->source_char;

        lexer_next_char(lexer);

        if (lexer->source_char == '\0')
        {

            return false;

        }

    } while (last_source_char != '#' || lexer->source_char != '@');

    lexer_next_char(lexer);
    
    return true;

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

    abort_if_null(lexer);

    reset_buffer(&lexer->buffer);

    lexer->use_buffer = true;

    size_t str_len = 0;

    while (
        lexer->source_char != '\0' &&
        (char_rule((int)lexer->source_char) ||
        (str_len != 0 && utf8_strchr((const char *)others, lexer->source_char))) &&
        str_len < max_length
    )
    {

        str_len += lexer->source_char_size;

        lexer_next_char(lexer);

    }

    lexer->use_buffer = false;

    char *str = NULL;

    buffer_to_string(&lexer->buffer, &str);

    return str;

}

bool lexer_parse_char_sequence(lexer_t *lexer, token_t *token)
{

    abort_if_null(lexer);
 
    abort_if_null(token);

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

    abort_if_null(lexer);
 
    abort_if_null(token);

    char *token_valeu = lexer_parse_sequence(lexer, isdigit, LEXER_PARSE_DIGIT_SEQUENCE_OTHERS, __SIZE_MAX__);

    if (token_valeu == NULL)
    {

        return false;

    }

    token->type = TOKEN_TYPE_LITERAL_NUMBER;

    token->value = token_valeu;

    return true;

}
