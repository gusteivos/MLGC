
#ifndef _MLG_BUFFER_H_
#define _MLG_BUFFER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>


    typedef struct buffer_s
    {

        uint8_t *data;

        size_t capacity;

        size_t count;

    } buffer_t;

    /**
     * @brief Creates a new buffer with the specified capacity.
     *
     * @param capacity The initial capacity of the buffer.
     * @return A pointer to the newly created buffer, or NULL on failure.
     */
    buffer_t *create_buffer(size_t capacity);

    /**
     * @brief Initializes the buffer with the specified capacity.
     *
     * @param buffer The pointer to the buffer to be initialized.
     * @param capacity The initial capacity of the buffer.
     * @return true if the initialization is successful, false otherwise.
     */
    bool init_buffer(buffer_t *buffer, size_t capacity);

    /**
     * @brief Resets the buffer, setting the count to 0.
     *
     * @param buffer The pointer to the buffer to be reset.
     * @return true if the reset is successful, false otherwise.
     */
    bool reset_buffer(buffer_t *buffer);

    /**
     * @brief Adjusts the buffer capacity to the new specified value.
     *
     * @param buffer The pointer to the buffer to be adjusted.
     * @param new_capacity The new capacity of the buffer.
     * @return true if the adjustment is successful, false otherwise.
     */
    bool grow_buffer(buffer_t *buffer, size_t new_capacity);

    bool buffer_add_data(buffer_t *buffer, void *data, size_t data_size);

    bool buffer_add_uint8(buffer_t *buffer, uint8_t b);

    bool buffer_add_char(buffer_t *buffer, char c);

    bool buffer_add_uint32(buffer_t *buffer, uint32_t u);

    bool buffer_add_string(buffer_t *buffer, char *str);

    bool buffer_get_data(buffer_t *buffer, void *data, size_t data_size);

    bool buffer_get_uint8(buffer_t *buffer, uint8_t *b);

    bool buffer_get_char(buffer_t *buffer, char *c);

    bool buffer_get_uint32(buffer_t *buffer, uint32_t *u);

    bool buffer_get_string(buffer_t *buffer, char **str, size_t str_len);

    bool buffer_to_string(buffer_t *buffer, char **str);

    /**
     * @brief Destroys the buffer, freeing its memory.
     *
     * @param buffer The pointer to the buffer to be destroyed.
     * @return true if the destruction is successful, false otherwise.
     */
    bool destroy_buffer(buffer_t *buffer);

#endif /* _MLG_BUFFER_H_ */
