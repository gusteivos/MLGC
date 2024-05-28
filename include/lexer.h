
#ifndef _MLG_LEXER_H_
#define _MLG_LEXER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <ctype.h>
    #include "token.h"


    #ifndef LEXER_PARSE_ALPHA_SEQUENCE_OTHERS
    #define LEXER_PARSE_ALPHA_SEQUENCE_OTHERS "_1234567890"
    #endif

    #ifndef LEXER_PARSE_DIGIT_SEQUENCE_OTHERS
    #define LEXER_PARSE_DIGIT_SEQUENCE_OTHERS "bho."
    #endif

    #ifndef LEXER_CONSIDERED_SPACE_CHAR
    #define LEXER_CONSIDERED_SPACE_CHAR " \t\n"
    #endif


    typedef struct lexer_s
    {

        char *source;

        size_t source_length;

        size_t source_index;

        char source_char;

        size_t line;

        size_t column;

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
     *
     * @param lexer The lexer to be reset.
     * @param source The new source string to be lexed.
     * @param source_length The length of the new source string.
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

#endif /* _MLG_LEXER_H_ */
