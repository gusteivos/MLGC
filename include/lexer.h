
#ifndef _MLG_LEXER_H_
#define _MLG_LEXER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <ctype.h>

    #include "location.h"
    #include "token.h"
    #include "buffer.h"


    #ifndef LEXER_BUFFER_INITIAL_CAPACITY
    #define LEXER_BUFFER_INITIAL_CAPACITY 8 /* ideally the size in bytes of the largest keyword. */
    #endif

    #ifndef LEXER_PARSE_CHAR_SEQUENCE_OTHERS
    #define LEXER_PARSE_CHAR_SEQUENCE_OTHERS "_1234567890"
    #endif

    #ifndef LEXER_PARSE_DIGIT_SEQUENCE_OTHERS
    #define LEXER_PARSE_DIGIT_SEQUENCE_OTHERS "bhoBHO.abcdefABCDEF"
    #endif

    #ifndef LEXER_CHARS_CONSIDERED_WHITESPACE
    #define LEXER_CHARS_CONSIDERED_WHITESPACE " \t\n"
    #endif

    #ifndef LEXER_TAB_WIDTH
    #define LEXER_TAB_WIDTH 4
    #endif


    bool start_lexing();

    bool finish_lexing();

    typedef struct lexer_s
    {

        const char *source_filename;

        FILE *source_file; /* for the future. */

        char *source;

        size_t source_length;

        location_t source_location;

        uint32_t source_char;

        size_t source_char_size;

        buffer_t buffer;

        bool use_buffer;

        /*  */

        size_t tab_width;

        char *chars_considered_whitespace;

    } lexer_t;

    lexer_t *create_lexer(const char *source_filename, char *source, size_t source_length);

    bool init_lexer(lexer_t *lexer, const char *source_filename, char *source, size_t source_length);

    bool fprint_lexer(FILE *s, lexer_t *lexer);

    bool lexer_lex_type(lexer_t *lexer, token_type_t *type, location_t *location);

    /**
     * Lexes the next token from the source.
     *
     * @param lexer The lexer instance.
     * @return A pointer to the next token, or NULL if an error occurs or end of source is reached.
     */
    token_t *lexer_lex(lexer_t *lexer);

    /**
     * Lexes all tokens from the source and stores them in the provided array.
     * The function dynamically reallocates memory for the tokens array.
     *
     * @param lexer The lexer instance.
     * @param tokens A pointer to an array of tokens, dynamically allocated and filled by this function.
     * @param tokens_count A pointer to a size_t variable where the number of tokens will be stored.
     * @return true if lexing is successful, false otherwise.
     */
    bool lexer_lex_all(lexer_t *lexer, token_t ***tokens, size_t *tokens_count);

    /**
     * Frees the memory allocated for the lexer.
     *
     * @param lexer The lexer to be destroyed.
     * @return true if the lexer is successfully destroyed, false otherwise.
     */
    bool destroy_lexer(lexer_t *lexer);

#endif /* _MLG_LEXER_H_ */
