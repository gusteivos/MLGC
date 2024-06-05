
#ifndef _MLG_TOKEN_H_
#define _MLG_TOKEN_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "location.h"


    typedef enum token_type_s
    {

        TOKEN_TYPE_INVALID = 0,

        TOKEN_TYPE_IDENTIFIER,

        TOKEN_TYPE_LITERAL_NUMBER,

        TOKEN_TYPE_KEYWORD_VARIABLE,
        TOKEN_TYPE_KEYWORD_FUNCTION,

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
     * Converts a token_type_t value to its string representation.
     *
     * @param type The type of the token.
     * @return A string representing the token type.
     */
    char *token_type_to_string(token_type_t type);

    typedef struct token_s
    {

        token_type_t type;

        char *value;

        location_t location;

    } token_t;

    /**
     * Creates and initializes a new token with the provided values.
     *
     * @param type The type of the token.
     * @param value The value of the token (will be duplicated).
     * @param location The location where the token was found.
     * @return A pointer to the newly created token, or NULL if allocation fails.
     */
    token_t *create_token(token_type_t type, char *value, location_t location);

    /**
     * Initializes an existing token with the provided values.
     *
     * @param token The token to be initialized.
     * @param type The type of the token.
     * @param value The value of the token (will be duplicated).
     * @param location The location where the token was found.
     * @return true if initialization is successful, false otherwise.
     */
    bool init_token(token_t *token, token_type_t type, char *value, location_t location);

    /**
     * Prints the token information.
     *
     * @param token The token to be printed.
     * @param token_type_name If true, prints the token type name; otherwise prints the numeric value of the type.
     * @return true if the token is valid and printing is successful, false otherwise.
     */
    bool print_token(token_t *token, bool token_type_name);

    /**
     * Frees the memory allocated for the token's value and the token itself.
     *
     * @param token The token to be destroyed.
     * @return true if the token is valid and destruction is successful, false otherwise.
     */
    bool destroy_token(token_t *token);

    typedef struct token_mapping_s
    {

        token_type_t type;

        char *value0;

        char *value1;

    } token_mapping_t;

    /**
     * Maps a token type to a token mapping value.
     *
     * @param token The token to be mapped.
     * @param token_mapping The token mapping to be used.
     * @param mapped Pointer to a boolean indicating whether the mapping was successful.
     * @param use_value If true, the token's value will be updated.
     * @return true if the operation is successful, false otherwise.
     */
    bool token_type_token_mapping_value(token_t *token, token_mapping_t *token_mapping, bool *mapped, bool use_value);

    /**
     * Maps a token value to a token type based on a token mapping.
     *
     * @param token_mapping The token mapping to be used.
     * @param token The token to be mapped.
     * @param mapped Pointer to a boolean indicating whether the mapping was successful.
     * @param use_value If true, the token's value will be updated.
     * @return true if the operation is successful, false otherwise.
     */
    bool token_mapping_value_token_type(token_mapping_t *token_mapping, token_t *token, bool *mapped, bool use_value);

#endif /* _MLG_TOKEN_H_ */
