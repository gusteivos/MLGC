#ifndef _MLG_UTIL_H_
#define _MLG_UTIL_H_

    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>


    /**
     * The FNV-1a offset basis constant.
     */
    #define FNV_OFFSET_BASIS 2166136261u

    /**
     * The FNV-1a prime constant.
     */
    #define FNV_PRIME 16777619u


    /**
     * Computes the djb2 hash of a string.
     *
     * @param str The input string to hash.
     * @return The computed hash value.
     */
    uint32_t djb2_string(const char *str);

    /**
     * Computes the FNV-1a hash of a string.
     *
     * @param str The input string to hash.
     * @return The computed hash value.
     */
    uint32_t fnv1a_string(const char *str);

    /**
     * Checks if the given pointer is null.
     * If it is null, calls abort().
     *
     * @param p The pointer to check.
     */
    void abort_if_null(void *p);

    /**
     * Allocates memory of the specified size.
     * If memory allocation fails, it calls abort().
     *
     * @param s The size of memory to allocate.
     * @return A pointer to the allocated memory.
     */
    void *a_alloc(size_t s);

    /**
     * Allocates memory of the specified size and clears it to zero.
     * If memory allocation fails, it calls abort().
     *
     * @param n The number of elements.
     * @param s The size of each element.
     * @return A pointer to the allocated memory.
     */
    void *a_clear_alloc(size_t n, size_t s);

    /**
     * Reallocates memory pointed to by p to the specified size.
     * If memory reallocation fails, it calls abort().
     *
     * @param p A pointer to the memory to reallocate.
     * @param s The new size of memory to allocate.
     * @return A pointer to the reallocated memory.
     */
    void *a_realloc(void *p, size_t s);

    /**
     * Duplicates a null-terminated string.
     * If memory allocation fails, it calls abort().
     *
     * @param s The string to duplicate.
     * @return A pointer to the duplicated string.
     */
    char *a_duplicate_string(const char *s);

#endif /* _MLG_UTIL_H_ */
