#include "location.h"


void fprint_location(FILE *file, location_t *location)
{

    if (!file || !location)
    {
    
        return;
    
    }

    fprintf(file, "Location:\n\tIndex: %zu, Line: %zu, Column: %zu\n", location->index, location->line, location->column);

}
