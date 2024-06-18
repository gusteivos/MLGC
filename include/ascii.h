
#ifndef _MLGC_ASCII_H_
#define _MLGC_ASCII_H_

    #include <stdbool.h>


    /**
     * Checks if the given character is an ASCII character.
     *
     * @param c The character to check.
     * @return true if the character is an ASCII character, false otherwise.
     */
    bool is_ascii_char(char c);

    /**
     * Checks if the given string contains only ASCII characters.
     *
     * @param s The string to check.
     * @return true if the string contains only ASCII characters, false otherwise.
     */
    bool is_ascii_string(const char *s);

#endif /* _MLGC_ASCII_H_ */
