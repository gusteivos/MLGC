
#ifndef _LOCATION_H_
#define _LOCATION_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct location_s
    {

        size_t index;

        size_t line;

        size_t column;

    } location_t;

    /**
     * @brief Prints location information to a STREAM.
     *
     * This function prints the index, line, and column values of a location_t
     * structure to a given STREAM.
     *
     * @param s Pointer to the STREAM where the information will be printed.
     * @param location Pointer to the location_t structure containing the information.
     * @return Returns true if printing is successful, or false if the STREAM or location is null, or if there is a printing error.
     */
    bool fprint_location(FILE *s, location_t *location);

#endif /* _LOCATION_H_ */
