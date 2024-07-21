
#ifndef _MLG_TOKEN_H_
#define _MLG_TOKEN_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "location.h"

    typedef enum token_type_e
    {

        TOKEN_TYPE_INVALID = 0,

        TOKEN_TYPE_IDENTIFIER,

        TOKEN_TYPE_LITERAL_NUMBER,

        TOKEN_TYPE_KEYWORD_VARIABLE,
        TOKEN_TYPE_KEYWORD_FUNCTION,

        TOKEN_TYPE_KEYWORD_VOID,

        TOKEN_TYPE_KEYWORD_INT8 ,
        TOKEN_TYPE_KEYWORD_INT16,
        TOKEN_TYPE_KEYWORD_INT32,
        TOKEN_TYPE_KEYWORD_INT64,

        TOKEN_TYPE_KEYWORD_FLOAT32,
        TOKEN_TYPE_KEYWORD_FLOAT64,

        TOKEN_TYPE_KEYWORD_SIGNED  ,
        TOKEN_TYPE_KEYWORD_UNSIGNED,

        TOKEN_TYPE_KEYWORD_SIZEOF,
        TOKEN_TYPE_KEYWORD_KINDOF,

        TOKEN_TYPE_PLUS,        /* + */
        TOKEN_TYPE_MINUS,       /* - */
        TOKEN_TYPE_ASTERISK,    /* * */
        TOKEN_TYPE_SLASH,       /* / */
        TOKEN_TYPE_PERCENT,     /* % */
        TOKEN_TYPE_CARET,       /* ^ */
        TOKEN_TYPE_EXCLAMATION, /* ! */
        TOKEN_TYPE_EQUAL,       /* = */
        TOKEN_TYPE_TILDE,       /* ~ */

        TOKEN_TYPE_PLUS_EQUAL,
        TOKEN_TYPE_MINUS_EQUAL,
        TOKEN_TYPE_ASTERISK_EQUAL,
        TOKEN_TYPE_SLASH_EQUAL,
        TOKEN_TYPE_PERCENT_EQUAL,
        TOKEN_TYPE_CARET_EQUAL,
        TOKEN_TYPE_EXCLAMATION_EQUAL,
        TOKEN_TYPE_EQUAL_EQUAL,

        TOKEN_TYPE_LESS_THAN,    /* < */
        TOKEN_TYPE_GREATER_THAN, /* > */
        TOKEN_TYPE_AMPERSAND,    /* & */
        TOKEN_TYPE_PIPE,         /* | */

        TOKEN_TYPE_LEFT_SHIFT,
        TOKEN_TYPE_RIGHT_SHIFT,
        TOKEN_TYPE_LOGICAL_AND,
        TOKEN_TYPE_LOGICAL_OR,

        TOKEN_TYPE_LESS_THAN_EQUAL,
        TOKEN_TYPE_GREATER_THAN_EQUAL,
        TOKEN_TYPE_AMPERSAND_EQUAL,
        TOKEN_TYPE_PIPE_EQUAL,

        TOKEN_TYPE_LEFT_SHIFT_EQUAL,
        TOKEN_TYPE_RIGHT_SHIFT_EQUAL,
        TOKEN_TYPE_LOGICAL_AND_EQUAL,
        TOKEN_TYPE_LOGICAL_OR_EQUAL,

        TOKEN_TYPE_DOT,
        TOKEN_TYPE_COMMA,
        TOKEN_TYPE_COLON,
        TOKEN_TYPE_SEMICOLON,

        TOKEN_TYPE_LEFT_BRACKET,  /* [ */
        TOKEN_TYPE_RIGHT_BRACKET, /* ] */
        TOKEN_TYPE_LEFT_BRACE,    /* { */
        TOKEN_TYPE_RIGHT_BRACE,   /* } */
        TOKEN_TYPE_LEFT_PAREN,    /* ( */
        TOKEN_TYPE_RIGHT_PAREN,   /* ) */

        TOKEN_TYPE_EOS = EOF

    } token_type_t;

    /**
     * @brief Converts a token type to its corresponding string representation.
     *
     * This function takes a token type (of type token_type_t) and returns
     * a string that describes the token type. If the token type is not
     * recognized, it returns "UNKNOWN".
     *
     * @param type The token type to convert.
     * @return A string representing the token type.
     */
    char *token_type_to_string(token_type_t type);

    /**
     * @brief Converts a token type to its corresponding symbol.
     *
     * This function takes a token type (of type token_type_t) and returns
     * a string that represents the symbol for the token type. If the token
     * type has no direct symbol representation, it falls back to returning
     * the string representation using the token_type_to_string function.
     *
     * @param type The token type to convert.
     * @return A string representing the token type symbol.
     */
    char *token_type_to_symbol(token_type_t type);

    typedef struct token_s
    {

        token_type_t type;

        char *value;

        location_t location;

    } token_t;

    /**
     * @brief Creates a new token with the specified type, value, and location.
     *
     * This function allocates memory for a new token, initializes it with the provided
     * type, value, and location, and returns a pointer to the newly created token.
     *
     * @param type The type of the token.
     * @param value The value associated with the token.
     * @param location The location information of the token.
     * @return A pointer to the newly created token, or NULL if memory allocation fails.
     */
    token_t *create_token(token_type_t type, char *value, location_t location);

    /**
     * @brief Initializes a token with the specified type, value, and location.
     *
     * This function initializes the fields of the provided token structure
     * with the given type, value, and location.
     *
     * @param token A pointer to the token structure to initialize.
     * @param type The type of the token.
     * @param value The value associated with the token.
     * @param location The location information of the token.
     * @return true if initialization was successful, false otherwise.
     */
    bool init_token(token_t *token, token_type_t type, char *value, location_t location);

    bool fprint_token(FILE *s, token_t *token);

    /**
     * @brief Destroys (frees) a token.
     *
     * This function frees the memory allocated for a token structure. Note that
     * this function does not free the memory pointed to by token->value.
     *
     * @param token A pointer to the token structure to destroy.
     * @return true if the token was successfully destroyed, false otherwise.
     */
    bool destroy_token(token_t *token);

#endif /* _MLG_TOKEN_H_ */
