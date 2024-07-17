#include "location.h"


bool fprint_location(FILE *s, location_t *location)
{

    if (!s)
    {

        return true;

    }

    if (!location)
    {

        return false;

    }

    int q = fprintf
    (
        s,
        "Location:\n\tIndex: %zu\n\tLine: %zu\n\tColumn: %zu\n",
        location->index, location->line, location->column
    );

    return !(q < 0);

}
