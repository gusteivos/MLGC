#include "util.h"


uint32_t djb2_string(const char *str)
{
    
    uint32_t hash = 5381;
    
    int c;

    while ((c = *str++))
    {
    
        hash = ((hash << 5) + hash) + c;
    
    }

    return hash;

}

uint32_t fnv1a_string(const char *str)
{

    uint32_t hash = FNV_OFFSET_BASIS;

    while (*str)
    {
    
        hash ^= (uint8_t)(*str++);
    
        hash *= FNV_PRIME;
    
    }
    
    return hash;

}

void abort_if_null(void *p)
{

    if (p == NULL)
    {
    
        abort();

    }

}

void *a_alloc(size_t s)
{

    void *ptr = malloc(s);

    abort_if_null(ptr);
    
    return ptr;

}

void *a_clear_alloc(size_t n, size_t s)
{

    void *ptr = calloc(n, s);

    abort_if_null(ptr);

    return ptr;

}

void *a_realloc(void *p, size_t s)
{

    void *ptr = realloc(p, s);

    abort_if_null(ptr);

    return ptr;

}

char *a_duplicate_string(const char *s)
{

    size_t len = strlen(s);

    char *dup = (char *)a_alloc(len + 1);

    abort_if_null((void *)dup);

    strcpy(dup, s);

    return dup;

}
