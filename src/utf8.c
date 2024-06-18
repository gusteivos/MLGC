#include "utf8.h"


size_t utf8_decode(uint32_t *cp, char *str, size_t str_len)
{

    if (str == NULL)
    {

        return 0;

    }

    size_t codepoint_len = SIZE_MAX;

    uint32_t codepoint = 0;

    uint8_t byte = (uint8_t)str[0];

    if      ((byte & 0x80) == 0x00 && str_len > 0) /* byte & 10000000 = 0xxxxxxx */
    {

        codepoint = byte;

        codepoint_len = 1;

    }
    else if ((byte & 0xE0) == 0xC0 && str_len > 1) /* byte & 11100000 = 110xxxxx */
    {

		codepoint = byte & 0x1f;

        codepoint_len = 2;

    }
    else if ((byte & 0xF0) == 0xE0 && str_len > 2) /* byte & 11110000 = 1110xxxx */
    {

        codepoint = byte & 0x0f;

        codepoint_len = 3;

    }
    else if ((byte & 0xF8) == 0xF0 && str_len > 3) /* byte & 11111000 = 11110xxx */
    {

        codepoint = byte & 0x0e;

        codepoint_len = 4;

    }
    else
    {

        return 0;

    }

    if (cp)
    {

        for (size_t q = 1; q < codepoint_len; ++q)
        {

            byte = (uint8_t)str[q];

            if ((byte & 0xC0) != 0x80)
            {

                codepoint = UTF8_INVALID;

                break;

            }

            codepoint = (codepoint << 6) | (byte & 0x3F);

        }

        /* TODO: Verify: Overlong encoding, invalid code point, or UTF-16 surrogate half. */

        *cp = codepoint;

    }

    return codepoint_len;

}

size_t utf8_encode(char *str, size_t str_len, uint32_t cp)
{

    if (str == NULL)
    {

        return 0;

    }

    size_t codepoint_len = SIZE_MAX;

    if      (cp <= 0x00007F && str_len > 0)
    {

        str[0] = cp;

        codepoint_len = 1;

    }
    else if (cp <= 0x0007FF && str_len > 1)
    {

        str[0] = 0xC0 | (cp >> 6);

        str[1] = 0x80 | (cp & 0x3F);

        codepoint_len = 2;

    }
    else if (cp <= 0x00FFFF && str_len > 2)
    {

        str[0] = 0xE0 | (cp >> 12);

        str[1] = 0x80 | ((cp >> 6) & 0x3F);

        str[2] = 0x80 | (cp & 0x3F);

        codepoint_len = 3;

    }
    else if (cp <= 0x10FFFF && str_len > 3)
    {

        str[0] = 0xF0 | (cp >> 18);

        str[1] = 0x80 | ((cp >> 12) & 0x3F);

        str[2] = 0x80 | ((cp >> 6) & 0x3F);

        str[3] = 0x80 | (cp & 0x3F);

        codepoint_len = 4;

    }
    else
    {

        return 0;

    }

    /* TODO: Encode using for. */

    return codepoint_len;

}

char *utf8_strchr(const char *str, uint32_t cp)
{

    if (str == NULL)
    {
    
        return NULL;
    
    }

    size_t q = 0;
    
    while (str[q] != '\0')
    {
    
        uint32_t decoded_cp;
    
        size_t len = utf8_decode(&decoded_cp, (char *)&str[q], 4);

        if (len == 0)
        {
        
            return NULL;
        
        }

        if (decoded_cp == cp)
        {
        
            return (char *)&str[q];
        
        }

        q += len;
    
    }

    return NULL;

}
