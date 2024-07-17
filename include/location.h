
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

    bool fprint_location(FILE *s, location_t *location);

#endif /* _LOCATION_H_ */
