#include "util.h"


uint32_t djb2_32(const void *data, size_t data_size)
{

    if (data == NULL || data_size == 0)
    {

        return DJB2_MAGIC_NUMBER;

    }

    const uint8_t *bytes = (const uint8_t *)data;

    uint32_t hash = DJB2_MAGIC_NUMBER;

    for (size_t i = 0; i < data_size; ++i)
    {

        hash = ((hash << 5) + hash) + bytes[i]; /* hash * 33 + byte; */

    }

    return hash;

}

uint32_t djb2_32_string(const char *str)
{

    if (str == NULL)
    {

        return DJB2_MAGIC_NUMBER;

    }

    uint32_t hash = DJB2_MAGIC_NUMBER;

    int c;

    while ((c = *str++))
    {

        hash = ((hash << 5) + hash) + c; /* hash * 33 + c; */

    }

    return hash;

}

uint32_t fnv1a_32(const void *data, size_t data_size)
{

    if (data == NULL || data_size == 0)
    {

        return FNV_32_OFFSET_BASIS;

    }

    const uint8_t *bytes = (const uint8_t *)data;

    uint32_t hash = FNV_32_OFFSET_BASIS;

    for (size_t i = 0; i < data_size; ++i)
    {

        hash = (hash ^ bytes[i]) * FNV_32_PRIME; /* hash ^ byte; hash * FNV_32_PRIME; */

    }

    return hash;

}

uint32_t fnv1a_32_string(const char *str)
{

    if (str == NULL)
    {

        return FNV_32_OFFSET_BASIS;

    }

    uint32_t hash = FNV_32_OFFSET_BASIS;

    int c;

    while ((c = *str++))
    {

        hash = (hash ^ c) * FNV_32_PRIME; /* hash ^ c; hash * FNV_32_PRIME; */

    }

    return hash;

}

void abort_if_null_function_line(const char *func, int line, void *p)
{

    if (p != NULL)
    {

        return;

    }

    fflush(stderr);

    fprintf(stderr, "Aborting NULL pointer detected in function %s, on line %d.\n", func, line);

    abort();

}

void *a_alloc(size_t s)
{

    void *ptr = malloc(s);

    if (ptr == NULL && s != 0)
    {

        abort_if_null(ptr);

    }

    return ptr;

}

void *a_clear_alloc(size_t n, size_t s)
{

    void *ptr = calloc(n, s);

    if (ptr == NULL && s != 0)
    {

        abort_if_null(ptr);

    }

    return ptr;

}

void *a_realloc(void *p, size_t s)
{

    if (p == NULL)
    {

        return a_alloc(s);

    }

    void *ptr = realloc(p, s);

    if (ptr == NULL && s != 0)
    {

        abort_if_null(ptr);

    }

    return ptr;

}

char *a_duplicate_string(const char *s)
{

    if (s == NULL)
    {
        
        return NULL;
 
    }

    size_t len = strlen(s);

    char *dup = (char *)a_alloc(len + 1);

    strcpy(dup, s);

    return dup;

}

void err_function_line(const char *func, int line, const char *fmt, ...)
{

	va_list ap;

    fprintf(stderr, "\033[31mError on func %s, on line %d:\033[0m\n", func, line);

    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);

    va_end(ap);

    putc('\n', stderr);

	exit(EXIT_FAILURE);

}

void war_function_line(const char *func, int line, const char *fmt, ...)
{

	va_list ap;

    fprintf(stdout, "\033[33mWarning on func %s, on line %d:\033[0m\n", func, line);

    va_start(ap, fmt);

    vfprintf(stdout, fmt, ap);

    va_end(ap);

    putc('\n', stdout);

}
