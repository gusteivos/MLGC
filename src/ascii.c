#include "ascii.h"


bool is_ascii_char(char c)
{

    return
    (

    /*  c == '\a' || */
    /*  c == '\b' || */
        c == '\t' ||
        c == '\n' ||
    /*  c == '\v' || */
    /*  c == '\f' || */
        c == '\r' ||

        (c >= ' ' && c <= '~') /* '~' or (char)0x7F */

    );

}

bool is_ascii_string(const char *s)
{

    if (!s)
    {

        return false;

    }

    while (*s)
    {

        if (!is_ascii_char(*s))
        {

            return false;

        }

        s++;

    }

    return true;

}
