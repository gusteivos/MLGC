#include "main.h"


int main(int argc, char *argv[])
{

    char *source_file_path = "./mlg.mlg";

    char *source_file_path_extension = get_file_path_extension(source_file_path);

    if (!source_file_path)
    {

        return EXIT_FAILURE;

    }

    if (strcmp((const char *)source_file_path_extension, ".mlg"))
    {

        fprintf(stderr, "Invalid file extension. Expected .mlg\n");

    }

    size_t source_file_content_size = 0;
    
    char *source_file_content = load_text_file(source_file_path, &source_file_content_size);
    
    if (!source_file_content)
    {
    
        fprintf(stderr, "Failed to load source file: %s\n", source_file_path);
    
        return EXIT_FAILURE;
    
    }

    printf("%s", source_file_content);

    lexer_t *lexer = create_lexer(source_file_content, source_file_content_size);

    token_t *token = NULL;

    do
    {

        token = lexer_lex(lexer);

        print_token(token, true);

    } 
    while (token != NULL && token->type != TOKEN_TYPE_EOS);

    return EXIT_SUCCESS;

}
