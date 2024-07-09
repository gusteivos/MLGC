#include "utf8.h"

size_t utf8_sized(uint8_t c)
{

    if ((c & 0x80) == 0x00) /* c & 10000000 = 0xxxxxxx */
    {

        return 1; /* cp = c & 0xFF */

    }

    if ((c & 0xE0) == 0xC0) /* c & 11100000 = 110xxxxx */
    {

        return 2; /* cp = c & 0x1F */

    }

    if ((c & 0xF0) == 0xE0) /* c & 11110000 = 1110xxxx */
    {

        return 3; /* cp = c & 0x0F */

    }

    if ((c & 0xF8) == 0xF0) /* c & 11111000 = 11110xxx */
    {

        return 4; /* cp = c & 0x07 */

    }

    return 0;

}

size_t utf8_decode(uint32_t *cp, char *str, size_t str_len)
{

    if (cp == NULL || str == NULL)
    {

        return 0;

    }

    uint8_t c = (uint8_t)str[0];

    size_t codepoint_len = utf8_sized(c);

    if (codepoint_len > str_len)
    {

        return 0;

    }

    switch (codepoint_len)
    {

    case 1:
        *cp = (c & 0xFF);
        break;

    case 2:
        *cp = (c & 0x1F);
        break;

    case 3:
        *cp = (c & 0x0F);
        break;

    case 4:
        *cp = (c & 0x07);
        break;

    default:
        return 0;

    }

    for (size_t q = 1; q < codepoint_len; ++q)
    {

        c = (uint8_t)str[q];

        if ((c & 0xC0) != 0x80)
        {

            *cp = UTF8_INVALID;

            break;

        }

        *cp = (*cp << 6) | (c & 0x3F);

    }

    /* TODO: Verify: Overlong encoding, invalid code point, or UTF-16 surrogate half. */

    return codepoint_len;

}

size_t utf8_sizee(uint32_t cp)
{

    if (cp < 0x000080)
    {

        return 1;

    }

    if (cp < 0x000800)
    {

        return 2;

    }

    if (cp < 0x010000)
    {

        return 3;

    }

    if (cp < 0x110000)
    {

        return 4;

    }

    return 0;

}

size_t utf8_encode(char *str, size_t str_len, uint32_t cp)
{

    if (str == NULL || cp == UTF8_INVALID)
    {

        return 0;

    }

    size_t codepoint_len = utf8_sizee(cp);

    if (codepoint_len > str_len)
    {

        return 0;

    }

    uint8_t first_byte_mask;

    switch (codepoint_len)
    {

    case 1:
        first_byte_mask = 0x00;
        break;

    case 2:
        first_byte_mask = 0xC0;
        break;

    case 3:
        first_byte_mask = 0xE0;
        break;

    case 4:
        first_byte_mask = 0xF0;
        break;

    default:
        return 0;

    }

    for (size_t q = codepoint_len - 1; q > 0; q--)
    {

        str[q] = (cp & 0x3F) | 0x80;

        cp = cp >> 6;

    }

	str[0] = cp | first_byte_mask;

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

            break;

        }

        if (decoded_cp == cp)
        {

            return (char *)&str[q];

        }

        q += len;

    }

    return NULL;

}
