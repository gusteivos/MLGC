
#ifndef _MLGC_UTF8_H_
#define _MLGC_UTF8_H_

    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>


    #define UTF8_INVALID UINT32_MAX


    /**
     * Determines the number of bytes required to represent a UTF-8 character.
     *
     * @param c The first byte of the UTF-8 character.
     * @return The number of bytes needed to fully encode the character, or 0 if `c` is an invalid UTF-8 start byte.
     */
    size_t utf8_sized(uint8_t c);

    /**
     * Decodes a UTF-8 sequence to obtain the corresponding Unicode code point.
     *
     * @param cp Pointer to store the decoded Unicode code point.
     * @param str UTF-8 string to decode.
     * @param str_len Length of the UTF-8 string.
     * @return The number of bytes consumed to decode the character, or 0 if invalid.
     */
    size_t utf8_decode(uint32_t *cp, char *str, size_t str_len);

    /**
     * Determines the size of the UTF-8 sequence needed to encode a Unicode code point.
     *
     * @param cp Unicode code point.
     * @return The number of bytes needed to encode the code point, or 0 if invalid.
     */
    size_t utf8_sizee(uint32_t cp);

    /**
     * Encodes a Unicode code point into a UTF-8 sequence.
     *
     * @param str Buffer to store the encoded UTF-8 sequence.
     * @param str_len Size of the `str` buffer.
     * @param cp Unicode code point to encode.
     * @return The number of bytes written to the `str` buffer, or 0 if the buffer is too small or the code point is invalid.
     */
    size_t utf8_encode(char *str, size_t str_len, uint32_t cp);

    /**
     * Locates the first occurrence of a Unicode code point in a UTF-8 string.
     *
     * @param str UTF-8 string to search.
     * @param cp Unicode code point to find.
     * @return A pointer to the first occurrence of `cp` in the `str` string, or NULL if not found.
     */
    char *utf8_strchr(const char* str, uint32_t cp);

#endif /* _MLGC_UTF8_H_ */
