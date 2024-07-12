#include "buffer.h"
#include "util.h"


buffer_t *create_buffer(size_t capacity)
{

    buffer_t *buffer = a_alloc(sizeof(buffer_t));

    if (!init_buffer(buffer, capacity))
    {

        destroy_buffer(buffer);

        return NULL;

    }

    return buffer;

}

bool init_buffer(buffer_t *buffer, size_t capacity)
{

    if (!buffer)
    {

        return false;

    }


    buffer->count = 0;

    buffer->data = NULL;

    return grow_buffer(buffer, capacity);

}

bool reset_buffer(buffer_t *buffer)
{

    if (!buffer)
    {

        return false;

    }

    buffer->count = 0;

    return true;

}

bool grow_buffer(buffer_t *buffer, size_t new_capacity)
{

    if (!buffer)
    {

        return false;

    }

    buffer->capacity = new_capacity;

    buffer->data = a_realloc(buffer->data, buffer->capacity * sizeof(uint8_t));

    if (buffer->count >= buffer->capacity)
    {

        buffer->count = buffer->capacity;

    }

    return true;

}

bool buffer_add_data(buffer_t *buffer, void *data, size_t data_size)
{

    if (!buffer || !data || data_size == 0)
    {

        return false;

    }

    if (buffer->count + data_size > buffer->capacity)
    {

        grow_buffer(buffer, buffer->capacity + data_size);

    }

    memcpy(&buffer->data[buffer->count], data, data_size);

    buffer->count += data_size;

    return true;

}

bool buffer_add_uint8(buffer_t *buffer, uint8_t b)
{

    return buffer_add_data(buffer, &b, sizeof(uint8_t));

}

bool buffer_add_char(buffer_t *buffer, char c)
{

    return buffer_add_data(buffer, &c, sizeof(char));

}

bool buffer_add_uint32(buffer_t *buffer, uint32_t u)
{

    return buffer_add_data(buffer, &u, sizeof(uint32_t));

}

bool buffer_add_string(buffer_t *buffer, char *str)
{

    if (!buffer || !str)
    {

        return false;

    }

    size_t str_len = strlen((const char *)str);

    if (str == 0)
    {

        return true;

    }

    return buffer_add_data(buffer, str, str_len);

}

bool buffer_get_data(buffer_t *buffer, void *data, size_t data_size)
{

    if (!buffer || !data || data_size == 0)
    {

        return false;

    }

    if (data_size > buffer->count)
    {

        return false;

    }

    memcpy(data, &buffer->data[buffer->count - data_size], data_size);

    buffer->count -= data_size;

    return true;

}

bool buffer_get_uint8(buffer_t *buffer, uint8_t *b)
{

    return buffer_get_data(buffer, b, sizeof(uint8_t));

}

bool buffer_get_char(buffer_t *buffer, char *c)
{

    return buffer_get_data(buffer, c, sizeof(char));

}

bool buffer_get_uint32(buffer_t *buffer, uint32_t *u)
{

    return buffer_get_data(buffer, u, sizeof(uint32_t));

}

bool buffer_get_string(buffer_t *buffer, char **str, size_t str_len)
{

    if (!buffer || str_len == 0)
    {

        return false;

    }
    
    *str = a_alloc(str_len + 1 * sizeof(char));

    if (!buffer_get_data(buffer, (void *)*str, str_len))
    {

        free(*str);

        *str = NULL;

        return false;

    }

    (*str)[str_len] = '\0';

    return true;

}

bool buffer_to_string(buffer_t *buffer, char **str)
{

    if (!buffer || !str)
    {

        return false;

    }

    char *str_buffer = (char *)a_alloc((buffer->count + 1) * sizeof(char));
  
    if (!buffer_get_data(buffer, str_buffer, buffer->count))
    {

        free(str_buffer);

        return false;

    } 

    size_t str_len = strlen(str_buffer);

    *str = (char *)a_alloc(str_len + 1);

    strcpy(*str, (const char *)str_buffer);

    free(str_buffer);

    return true;

}

bool destroy_buffer(buffer_t *buffer)
{

    if (!buffer)
    {

        return false;

    }

    if (buffer->data)
    {

        free(buffer->data);

    }

    free(buffer);

    return true;

}
