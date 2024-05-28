
#ifndef _MLGC_LOCATION_H_
#define _MLGC_LOCATION_H_

    #include <stdio.h>
    #include <stdlib.h>

    typedef struct location_s
    {

        size_t index;

        size_t line;

        size_t column;

    } location_t;

    /**
     * Prints the location information to the given file.
     *
     * @param file The file to print to.
     * @param location The location structure containing the index, line, and column.
     */
    void fprint_location(FILE *file, location_t *location);

#endif
