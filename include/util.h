
#ifndef _MLG_UTIL_H_
#define _MLG_UTIL_H_

    #include <stdint.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>


    #ifndef DJB2_MAGIC_NUMBER
    #define DJB2_MAGIC_NUMBER 5381
    #endif

    #ifndef FNV_32_OFFSET_BASIS
    #define FNV_32_OFFSET_BASIS 2166136261
    #endif

    #ifndef FNV_32_PRIME
    #define FNV_32_PRIME 16777619
    #endif


    uint32_t djb2_32(const void *data, size_t data_size);

    uint32_t djb2_32_string(const char *str);

    uint32_t fnv1a_32(const void *data, size_t data_size);

    uint32_t fnv1a_32_string(const char *str);

    /**
     * Computes the FNV-1a hash of a string.
     *
     * @param str The input string to hash.
     * @return The computed hash value.
     */
    uint32_t fnv1a_string(const char *str);

    /**
     * @brief Checks if a pointer is null and aborts the program if it is.
     *
     * This function prints an error message containing the function name and line number
     * where the check was performed, before aborting the program.
     *
     * @param function The name of the function where the check was performed.
     * @param line The line number where the check was performed.
     * @param p The pointer to be checked.
     */
    void abort_if_null_function_line(const char *func, int line, void *p);

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

    /**
     * @brief Displays a formatted error message and terminates the program.
     *
     * This function displays a formatted error message on the standard error output (stderr),
     * indicating the function and line where the error occurred. It then terminates the program
     * with an exit code indicating failure.
     *
     * @param func The name of the function where the error occurred.
     * @param line The line number where the error occurred.
     * @param fmt  The format string for the error message.
     * @param ...  Additional arguments for the format string.
     */
    void err_function_line(const char *func, int line, const char *fmt, ...);

    /**
     * @brief Displays a formatted warning message.
     *
     * This function displays a formatted warning message on the standard output (stdout),
     * indicating the function and line where the warning occurred.
     *
     * @param func The name of the function where the warning occurred.
     * @param line The line number where the warning occurred.
     * @param fmt  The format string for the warning message.
     * @param ...  Additional arguments for the format string.
     */
    void war_function_line(const char *func, int line, const char *fmt, ...);

    #define abort_if_null(p) abort_if_null_function_line(__FUNCTION__, __LINE__, p)

    #define err(fmt, ...) err_function_line(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

    #define war(fmt, ...) war_function_line(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* _MLG_UTIL_H_ */
