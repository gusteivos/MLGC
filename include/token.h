
#ifndef _MLG_TOKEN_H_
#define _MLG_TOKEN_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>


    typedef enum token_type_s
    {

        TOKEN_TYPE_INVALID = 0,

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

        size_t line;

        size_t column;

    } token_t;

    /**
     * Creates and initializes a new token with the provided values.
     *
     * @param type The type of the token.
     * @param value The value of the token (will be duplicated).
     * @param line The line where the token was found.
     * @param column The column where the token was found.
     * @return A pointer to the newly created token, or NULL if allocation fails.
     */
    token_t *create_token(token_type_t type, char *value, size_t line, size_t column);

    /**
     * Initializes an existing token with the provided values.
     *
     * @param token The token to be initialized.
     * @param type The type of the token.
     * @param value The value of the token (will be duplicated).
     * @param line The line where the token was found.
     * @param column The column where the token was found.
     * @return true if initialization is successful, false otherwise.
     */
    bool init_token(token_t *token, token_type_t type, char *value, size_t line, size_t column);

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

#endif /* _MLG_TOKEN_H_ */
