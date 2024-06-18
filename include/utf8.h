
#ifndef _MLGC_UTF8_H_
#define _MLGC_UTF8_H_

    #include <stdint.h>
    #include <stdlib.h>
   

    #define UTF8_INVALID UINT32_MAX


    /**
     * Decodes a UTF-8 sequence from a string.
     *
     * @param cp Pointer to a uint32_t to store the decoded codepoint.
     * @param str Pointer to the input string containing the UTF-8 sequence.
     * @param str_len Length of the input string.
     * @return The number of bytes used for the decoded codepoint, 0 if the sequence is invalid or if decoding is invalid returns UTF8_INVALID.
     */
    size_t utf8_decode(uint32_t *cp, char *str, size_t str_len);

    /**
     * Encodes a codepoint into a UTF-8 sequence.
     *
     * @param str Pointer to the output string buffer.
     * @param str_len Length of the output string buffer.
     * @param cp The Unicode codepoint to encode.
     * @return The number of bytes written to the output string, or 0 if the codepoint is invalid.
     */
    size_t utf8_encode(char *str, size_t str_len, uint32_t cp);

    /**
     * Searches for a specific UTF-8 encoded codepoint in a string.
     *
     * @param str Pointer to the input string.
     * @param cp The Unicode codepoint to search for.
     * @return Pointer to the first occurrence of the codepoint in the string, or NULL if not found.
     */
    char *utf8_strchr(const char* str, uint32_t cp);

#endif /* _MLGC_UTF8_H_ */
