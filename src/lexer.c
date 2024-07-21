#include "lexer.h"
#include "util.h"
#include "utf8.h"
#include "hash_table.h"


static hash_table_t *keyword_table = NULL;

static bool keyword_table_insert(char *keyword, token_type_t type)
{

    hash_table_insert(keyword_table, (uint8_t *)keyword, strlen(keyword), (void *)type);

    return true;

}

static bool keyword_table_lookup(char *key, size_t key_lenght, token_type_t *type)
{

    void *value = NULL;

    if (type && hash_table_lookup(keyword_table, (uint8_t *)key, key_lenght, &value))
    {

        *type = (token_type_t)value;

    }

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

static bool lexer_lex_2compound(lexer_t *lexer, uint32_t ac, token_type_t att, token_type_t btt, token_type_t *type)
{

    if (lexer->source_char == ac)
    {

        lexer_next_char(lexer);

        *type = att;

    }
    else
    {

        *type = btt;

    }

    return true;

}

static bool lexer_lex_type2(lexer_t *lexer, token_type_t *type)
{

    uint32_t c = lexer->source_char;

    lexer_next_char(lexer);

    switch (c)
    {

    case '+':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_PLUS_EQUAL, TOKEN_TYPE_PLUS, type);

    case '-':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_MINUS_EQUAL, TOKEN_TYPE_MINUS, type);

    case '*':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_ASTERISK_EQUAL, TOKEN_TYPE_ASTERISK, type);

    case '/':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_SLASH_EQUAL, TOKEN_TYPE_SLASH, type);

    case '%':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_PERCENT_EQUAL, TOKEN_TYPE_PERCENT, type);

    case '^':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_CARET_EQUAL, TOKEN_TYPE_CARET, type);

    case '!':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_EXCLAMATION_EQUAL, TOKEN_TYPE_EXCLAMATION, type);

    case '=':
        return lexer_lex_2compound(lexer, '=', TOKEN_TYPE_EQUAL_EQUAL, TOKEN_TYPE_EQUAL, type);

    default:
        {

            err("\tTODO: .\n");

        }
        return false;

    }

    return true;

}

static bool lexer_lex_3compound(lexer_t *lexer, uint32_t ac, token_type_t att, uint32_t bc, uint32_t cc, token_type_t ctt, token_type_t btt, token_type_t dtt, token_type_t *type)
{

    if (lexer->source_char == ac)
    {

        *type = att;

        lexer_next_char(lexer);

    }
    else if (lexer->source_char == bc)
    {

        lexer_next_char(lexer);

        if (lexer->source_char == cc)
        {

            *type = ctt;

            lexer_next_char(lexer);

        }
        else
        {

            *type = btt;

        }

    }
    else
    {

        *type = dtt;

    }

    return true;

}

static bool lexer_lex_type3(lexer_t *lexer, token_type_t *type)
{

    uint32_t c = lexer->source_char;

    lexer_next_char(lexer);

    switch (c)
    {

    case '<':
        return lexer_lex_3compound(lexer, '=', TOKEN_TYPE_LESS_THAN_EQUAL, '<', '=', TOKEN_TYPE_LEFT_SHIFT_EQUAL, TOKEN_TYPE_LEFT_SHIFT, TOKEN_TYPE_LESS_THAN, type);

    case '>':
        return lexer_lex_3compound(lexer, '=', TOKEN_TYPE_GREATER_THAN_EQUAL, '>', '=', TOKEN_TYPE_RIGHT_SHIFT_EQUAL, TOKEN_TYPE_RIGHT_SHIFT, TOKEN_TYPE_GREATER_THAN, type);

    case '&':
        return lexer_lex_3compound(lexer, '=', TOKEN_TYPE_AMPERSAND_EQUAL, '&', '=', TOKEN_TYPE_LOGICAL_AND_EQUAL, TOKEN_TYPE_LOGICAL_AND, TOKEN_TYPE_AMPERSAND, type);

    case '|':
        return lexer_lex_3compound(lexer, '=', TOKEN_TYPE_PIPE_EQUAL, '|', '=', TOKEN_TYPE_LOGICAL_OR_EQUAL, TOKEN_TYPE_LOGICAL_OR, TOKEN_TYPE_PIPE, type);

    default:
        {

            err("\tTODO: .\n");

        }
        return false;

    }

    return true;

}

bool lexer_parse_sequence(lexer_t *lexer, int (*char_rule)(int), char *others, size_t max_length)
{

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

    return lexer->use_buffer = str_len > 0;

}

bool lexer_parse_char_sequence(lexer_t *lexer)
{

    return lexer_parse_sequence(lexer, isalpha, LEXER_PARSE_CHAR_SEQUENCE_OTHERS, __SIZE_MAX__);

}

bool lexer_parse_digit_sequence(lexer_t *lexer)
{

    return lexer_parse_sequence(lexer, isdigit, LEXER_PARSE_DIGIT_SEQUENCE_OTHERS, __SIZE_MAX__);

}

bool start_lexing()
{

    if (keyword_table)
    {

        return false;

    }

    keyword_table = create_hash_table(24, (hash_table_hash_function_t)fnv1a_32);

    keyword_table_insert("var", TOKEN_TYPE_KEYWORD_VARIABLE);
    keyword_table_insert("func", TOKEN_TYPE_KEYWORD_FUNCTION);

    keyword_table_insert("void", TOKEN_TYPE_KEYWORD_VOID);

    keyword_table_insert("byte", TOKEN_TYPE_KEYWORD_INT8);
    keyword_table_insert("int8", TOKEN_TYPE_KEYWORD_INT8);

    keyword_table_insert("short", TOKEN_TYPE_KEYWORD_INT16);
    keyword_table_insert("int16", TOKEN_TYPE_KEYWORD_INT16);

    keyword_table_insert("int", TOKEN_TYPE_KEYWORD_INT32);
    keyword_table_insert("int32", TOKEN_TYPE_KEYWORD_INT32);

    keyword_table_insert("long", TOKEN_TYPE_KEYWORD_INT64);
    keyword_table_insert("int64", TOKEN_TYPE_KEYWORD_INT64);

    keyword_table_insert("single", TOKEN_TYPE_KEYWORD_FLOAT32);
    keyword_table_insert("float32", TOKEN_TYPE_KEYWORD_FLOAT32);

    keyword_table_insert("double", TOKEN_TYPE_KEYWORD_FLOAT64);
    keyword_table_insert("float64", TOKEN_TYPE_KEYWORD_FLOAT64);

    keyword_table_insert("signed", TOKEN_TYPE_KEYWORD_SIGNED);
    keyword_table_insert("unsigned", TOKEN_TYPE_KEYWORD_UNSIGNED);

    keyword_table_insert("sizeof", TOKEN_TYPE_KEYWORD_SIZEOF);
    keyword_table_insert("kindof", TOKEN_TYPE_KEYWORD_KINDOF);

#ifndef NDEBUG

    fprint_hash_table(stdout, keyword_table);

#endif

    return true;

}

bool finish_lexing()
{

    abort_if_null(keyword_table);

    destroy_hash_table(keyword_table, NULL);

    return true;

}

lexer_t *create_lexer(const char *source_filename, char *source, size_t source_length)
{

    lexer_t *lexer = (lexer_t *)a_alloc(sizeof(lexer_t));

    init_lexer(lexer, source_filename, source, source_length);

    return lexer;

}

bool init_lexer(lexer_t *lexer, const char *source_filename, char *source, size_t source_length)
{

    abort_if_null(lexer);

    abort_if_null((void *)source_filename);

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

bool fprint_lexer(FILE *s, lexer_t *lexer)
{

    if (!s)
    {

        return true;

    }

    abort_if_null(lexer);

    int q = fprintf(s, "Lexer:\n\tSource filename: %s\n\tSource file: %p\n", lexer->source_filename, (void *)lexer->source_file);

    if (q < 0)
    {

        return false;

    }

    bool w = fprint_location(s, &lexer->source_location);

    if (!w)
    {

        return false;

    }

    q = fprintf(s, "\tCurrent source char: %u\n\tCurrent source char size: %zu\n", lexer->source_char, lexer->source_char_size);

    if (q < 0)
    {

        return false;

    }

    /* TODO: print buffer.*/

    q = fprintf(s, "Use buffer: %s\n", lexer->use_buffer ? "true" : "false");

    return !(q < 0);

}

bool lexer_lex_type(lexer_t *lexer, token_type_t *type, location_t *location)
{

    abort_if_null(lexer);

    abort_if_null(type);

    *type = TOKEN_TYPE_INVALID;

retry: /*TODO: remove it later.*/

    lexer_skip_chars_considered_whitespace(lexer);

    if (location)
    {

        *location = lexer->source_location;

    }

    if (lexer->source_char == '@')
    {

        if (lexer_skip_block_comment(lexer))
        {

            goto retry;

        }

        if (lexer->source_char == '\0')
        {

            fprint_location(stderr, location);

            err("\tWhen lexing block comment not finished.\n");

            return false;

        }

    }

    if (lexer_parse_char_sequence(lexer))
    {

        *type = TOKEN_TYPE_IDENTIFIER;

        return true;

    }

    if (lexer_parse_digit_sequence(lexer))
    {

        *type = TOKEN_TYPE_LITERAL_NUMBER;

        return true;

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
        return lexer_lex_type2(lexer, type);
    case '~':
        *type = TOKEN_TYPE_TILDE;
        break;

    case '<':
    case '>':
    case '&':
    case '|':
        return lexer_lex_type3(lexer, type);

    case '.':
        *type = TOKEN_TYPE_DOT;
        break;
    case ',':
        *type = TOKEN_TYPE_COMMA;
        break;
    case ':':
        *type = TOKEN_TYPE_COLON;
        break;
    case ';':
        *type = TOKEN_TYPE_SEMICOLON;
        break;

    case '[':
        *type = TOKEN_TYPE_LEFT_BRACKET;
        break;
    case ']':
        *type = TOKEN_TYPE_RIGHT_BRACKET;
        break;
    case '{':
        *type = TOKEN_TYPE_LEFT_BRACE;
        break;
    case '}':
        *type = TOKEN_TYPE_RIGHT_BRACE;
        break;
    case '(':
        *type = TOKEN_TYPE_LEFT_PAREN;
        break;
    case ')':
        *type = TOKEN_TYPE_RIGHT_PAREN;
        break;

    case '#':
        lexer_skip_line_comment(lexer);
        goto retry;

    case '$':
        {

            fprint_location(stdout, &lexer->source_location);

            war("\tWhen lexing line for preprocessor not processed, skipping the line.\n");

            lexer_skip_line(lexer);

        }
        goto retry;

    case '\0':
        *type = TOKEN_TYPE_EOS;
        break;

    default:
        {

            fprint_location(stderr, &lexer->source_location);

            size_t utf8_size = utf8_sizee(lexer->source_char);

            char *utf8_char = (char *)a_alloc((utf8_size + 1) * sizeof(char));

            utf8_encode(utf8_char, utf8_size, lexer->source_char);

            utf8_char[utf8_size] = '\0';

            err("\tWhen lexing invalid char \'%s\'.\n", utf8_size ? utf8_char : "invalid and or not encodable char");

            free(utf8_char);

        }
        return false;

    }

    lexer_next_char(lexer);

    return true;

}

token_t *lexer_lex(lexer_t *lexer)
{

    token_type_t type;

    location_t location;

    lexer_lex_type(lexer, &type, &location);

    char *value = NULL;

    size_t value_lenght = 0;

    if (lexer->use_buffer)
    {

        buffer_to_string(&lexer->buffer, &value, &value_lenght);

        lexer->use_buffer = false;

    }

    if (type == TOKEN_TYPE_IDENTIFIER)
    {

        if (value)
        {

            if (keyword_table_lookup(value, value_lenght, &type))
            {

                free(value);

                value = NULL;

            }

        }
        else
        {

            type = TOKEN_TYPE_INVALID;

        }

    }

    token_t *token = (token_t *)a_alloc(sizeof(token_t));

    token->type = type;

    token->value = value;

    token->location = location;

    return token;

}

bool lexer_lex_all(lexer_t *lexer, token_t ***tokens, size_t *tokens_count)
{

    abort_if_null(lexer);

    abort_if_null(tokens);

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
