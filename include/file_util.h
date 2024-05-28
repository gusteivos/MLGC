
#ifndef _MLG_FILE_UTIL_H_
#define _MLG_FILE_UTIL_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>


    /**
     * Extracts the base name from a file path.
     *
     * @param path The file path.
     * @return The base name of the file path.
     */
    char *get_path_base_name(char *path);

    /**
     * Extracts the file extension from a file path, including the period '.'.
     *
     * @param file_path The file path.
     * @return The file extension of the file path, including the period '.'.
     */
    char *get_file_path_extension(char *file_path);

    /**
     * Opens a file for reading and returns its size.
     *
     * @param file_path The path to the file.
     * @param binary_mode Whether to open the file in binary mode (true) or text mode (false).
     * @param size A pointer to a size_t variable to store the size of the file.
     * @return A pointer to the opened file, or NULL if an error occurred.
     */
    FILE *open_file_read(const char *file_path, bool binary_mode, size_t *size);

    /**
     * Loads a binary file into memory.
     *
     * @param file_path The path to the file.
     * @param size A pointer to a size_t variable to store the size of the loaded file.
     * @return A pointer to the loaded file content in memory, or NULL if an error occurred.
     *         The caller is responsible for freeing the allocated memory.
     */
    char *load_file(const char *file_path, size_t *size);

    /**
     * Loads a text file into memory, ensuring the content is null-terminated.
     *
     * @param file_path The path to the file.
     * @param size A pointer to a size_t variable to store the size of the loaded file (excluding the null terminator).
     *             If NULL, the size is not stored.
     * @return A pointer to the loaded file content in memory, or NULL if an error occurred.
     *         The caller is responsible for freeing the allocated memory.
     */
    char *load_text_file(const char *file_path, size_t *size);

#endif
