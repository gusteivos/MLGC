#include "main.h"


void shift_argc_argv(int *argc, char **argv[])
{

    assert(!argc && *argc > 0);

    ++*argv;

    --*argc;

}

int main(int argc, char *argv[])
{

    #ifndef NDEBUG

    return debug_main(argc, argv);

    #endif

    char *name = argv[0];

    if (argc < MIN_ARG_C)
    {

        usage(name, true, EXIT_FAILURE);

        return EXIT_FAILURE;

    }

    shift_argc_argv(&argc, &argv);

    char *output_file_name = NULL;

    size_t input_files_count = 0;

    char **input_files       = NULL;

    bool single_file_mode = false;

    while (argc > 0)
    {

        char *value = argv[0];

        if
        (
            !strcmp((const char *)value, "-h"    ) ||
            !strcmp((const char *)value, "--help")
        )
        {

            usage(name, false, EXIT_SUCCESS);

            continue;

        }

        if
        (
            !strcmp((const char *)value, "-s"    )
        )
        {

            if (single_file_mode)
            {

                war(
                    "\tSingle file mode is already enabled.\n"
                   );

            }

            single_file_mode = true;

            shift_argc_argv(&argc, &argv);

            continue;

        }

        if
        (
            !strcmp((const char *)value, "-o"    )
        )
        {

            shift_argc_argv(&argc, &argv);

            if (argc > 0)
            {

                if (output_file_name)
                {

                    war(
                        "\tOutput file name already specified,\n"
                        "\tOverwriting the previous one.\n"
                    );

                    free(output_file_name);

                }

                output_file_name = a_duplicate_string(argv[0]);

            }
            else
            {

                war("\tOutput file name not specified after -o option.\n");

                usage(name, true, EXIT_FAILURE);

            }

            shift_argc_argv(&argc, &argv);

            continue;

        }

        input_files = a_realloc(input_files, (input_files_count + 1) * sizeof(char *));

        input_files[input_files_count] = a_duplicate_string(value);

        input_files_count++;

        shift_argc_argv(&argc, &argv);

    }

    if (single_file_mode && input_files_count > 1)
    {

        err(
            "\tSingle-file mode requires exactly one input file.\n"
            "\tMultiple files provided.\n"
           );

        return EXIT_FAILURE;

    }

    /* TODO: lex parse check emit: compile. */

    return EXIT_SUCCESS;

}

void usage(char *name, bool show_help, int exit_status)
{

    printf
    (
        "\n"
        "\t%sUsage: %s\n",
        (name) ? "" : "Try ",
        (name) ? name : NAME
    );

    printf
    (
        "\t\t-s                    Set single file mode\n"
        "\t\t-o <file>             Specify output file name\n"
    );

if (show_help)
{

    printf
    (
        "\t\t-h, --help            Output this help information\n"
    );

}

    printf
    (
        "\n"
        "\t\tAny other arguments are interpreted as input files.\n"
        "\n"
    );

    exit(exit_status);

}

int debug_main(int argc, char *argv[])
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

    printf("Starting to read the file: %s\n", source_file_path);

    size_t source_file_content_size = 0;

    char *source_file_content = load_text_file(source_file_path, &source_file_content_size);

    if (!source_file_content)
    {

        fprintf(stderr, "Failed to load source file: %s\n", source_file_path);

        return EXIT_FAILURE;

    }

    printf("File content: \"");

    printf("%s", source_file_content);

    printf("\"\n");

    fflush(stdout);

    printf("Starting to lexing the file:\n");

    start_lexing();

    lexer_t *lexer = create_lexer(source_file_path, source_file_content, source_file_content_size);

    token_t **tokens = NULL;

    size_t tokens_count = 0;

    if (!lexer_lex_all(lexer, &tokens, &tokens_count))
    {

        return EXIT_FAILURE;

    }

    printf("Generated tokens:\n");

    for (size_t q = 0; q < tokens_count; q++)
    {

        token_t *token = tokens[q];

        fprint_token(stdout, token);

    }

    finish_lexing();

    printf("Starting to parsing the file:\n");

    parser_t *parser = create_parser(tokens, tokens_count);

    ast_node_t *root_node = parser_parse(parser);

    if (!root_node)
    {

        err("\troot_node is NULL.\n");

    }

    if (root_node->type == TOKEN_TYPE_INVALID)
    {

        err("\troot_node TYPE is TOKEN_TYPE_INVALID.\n");

    }



    return EXIT_SUCCESS;

}
