
#ifndef _MLG_LEXER_H_
#define _MLG_LEXER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <ctype.h>
    #include "location.h"
    #include "token.h"


    #ifndef LEXER_PARSE_ALPHA_SEQUENCE_OTHERS
    #define LEXER_PARSE_ALPHA_SEQUENCE_OTHERS "_1234567890"
    #endif

    #ifndef LEXER_PARSE_DIGIT_SEQUENCE_OTHERS
    #define LEXER_PARSE_DIGIT_SEQUENCE_OTHERS "bho."
    #endif

    #ifndef LEXER_CHARS_CONSIDERED_WHITESPACE
    #define LEXER_CHARS_CONSIDERED_WHITESPACE " \t\n"
    #endif

    #ifndef LEXER_TAB_WIDTH
    #define LEXER_TAB_WIDTH 4
    #endif


    typedef struct lexer_s
    {

        char *source;

        size_t source_length;

        location_t source_location;

        char source_char;

    } lexer_t;


    /**
     * Creates and initializes a new lexer with the provided source and source length.
     * If the source length is not provided, it uses strlen to determine the length.
     *
     * @param source The source string to be lexed.
     * @param source_length The length of the source string. If 0, strlen will be used.
     * @return A pointer to the newly created lexer, or NULL if allocation fails.
     */
    lexer_t *create_lexer(char *source, size_t source_length);

    /**
     * Initializes an existing lexer with the provided source and source length.
     * If the source length is not provided, it uses strlen to determine the length.
     *
     * @param lexer The lexer to be initialized.
     * @param source The source string to be lexed.
     * @param source_length The length of the source string. If 0, strlen will be used.
     * @return true if initialization is successful, false otherwise.
     */
    bool init_lexer(lexer_t *lexer, char *source, size_t source_length);

    /**
     * Lexes the next token from the source.
     *
     * @param lexer The lexer instance.
     * @return A pointer to the next token, or NULL if an error occurs or end of source is reached.
     */
    token_t *lexer_lex(lexer_t *lexer);

    /**
     * Resets the lexer with a new source and source length.
     * If the provided source is NULL, it uses the existing source and source length of the lexer.
     * If the provided source length is 0, it uses strlen to determine the length of the new source string.
     *
     * @param lexer The lexer to be reset.
     * @param source The new source string to be lexed. If NULL, the existing source is used.
     * @param source_length The length of the new source string. If 0, strlen will be used.
     *                      If source is NULL, the existing source length is used.
     * @return true if the lexer is successfully reset, false otherwise.
     */
    bool reset_lexer(lexer_t *lexer, char *source, size_t source_length);

    /**
     * Frees the memory allocated for the lexer.
     *
     * @param lexer The lexer to be destroyed.
     * @return true if the lexer is successfully destroyed, false otherwise.
     */
    bool destroy_lexer(lexer_t *lexer);


    void lexer_update_source_location(lexer_t *lexer);

    void lexer_next_char(lexer_t *lexer);

    void lexer_peek_char(lexer_t *lexer, size_t offset);

    void lexer_skip_chars_considered_whitespace(lexer_t *lexer);

    token_t *lexer_lex2(lexer_t *lexer, token_t *token);

    token_t *lexer_lex3(lexer_t *lexer, token_t *token);

#endif /* _MLG_LEXER_H_ */
