#include "file_util.h"
#include "util.h"


char *get_path_base_name(char *path)
{

    if (!path)
    {

        return NULL;

    }

    char *last_slash = strrchr(path, '/');

    if (!last_slash)
    {

        last_slash = strrchr(path, '\\');

    }

    if (!last_slash)
    {

        return a_duplicate_string(path);

    }

    return a_duplicate_string(last_slash + 1);

}

char *get_file_path_extension(char *file_path)
{

    if (!file_path)
    {

        return NULL;

    }

    char *last_dot = strrchr(file_path, '.');

    if (!last_dot)
    {

        return NULL;

    }

    return a_duplicate_string(last_dot);

}

FILE *open_file_read(const char *file_path, bool binary_mode, size_t *size)
{

    const char *mode = binary_mode ? "rb" : "r";

    FILE *file_stream = fopen(file_path, mode);

    if (!file_stream)
    {

        fprintf(stderr, "Failed to open file: %s\n", file_path);

        return NULL;

    }

    if (size != NULL)
    {

        if (fseek(file_stream, 0, SEEK_END) != 0)
        {

            fprintf(stderr, "Failed to seek end of file: %s\n", file_path);

            fclose(file_stream);

            return NULL;

        }

        long file_size = ftell(file_stream);

        if (file_size < 0)
        {

            fprintf(stderr, "Failed to determine file size: %s\n", file_path);

            fclose(file_stream);

            return NULL;

        }

        if (fseek(file_stream, 0, SEEK_SET) != 0)
        {

            fprintf(stderr, "Failed to seek start of file: %s\n", file_path);

            fclose(file_stream);

            return NULL;

        }

        *size = (size_t)file_size;

    }

    return file_stream;

}

char *load_file(const char *file_path, size_t *size)
{

    size_t _size = 0;

    FILE *file = open_file_read(file_path, true, &_size);

    if (!file)
    {

        return NULL;

    }

    char *buffer = (char *)a_alloc(_size);

    if (fread(buffer, sizeof(char), _size, file) != _size)
    {

        fprintf(stderr, "Failed to read file: %s\n", file_path);

        free(buffer);

        fclose(file);

        return NULL;

    }

    fclose(file);

    if (size != NULL)
    {

        *size = _size;

    }

    return buffer;
}

char *load_text_file(const char *file_path, size_t *size)
{

    size_t _size = 0;

    FILE *file = open_file_read(file_path, false, &_size);

    if (!file)
    {

        return NULL;

    }

    char *buffer = (char *)a_alloc(_size + 1);

    if (fread(buffer, sizeof(char), _size, file) != _size)
    {

        fprintf(stderr, "Failed to read file: %s\n", file_path);

        free(buffer);

        fclose(file);

        return NULL;

    }

    buffer[_size] = '\0';

    fclose(file);

    if (size != NULL)
    {

        *size = _size;

    }

    return buffer;

}
