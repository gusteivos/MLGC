
#ifndef _MLG_TYPE_H_
#define _MLG_TYPE_H_

    #include <stdlib.h>
    #include <stdint.h>


    typedef enum type_kind_e
    {

        TYPE_KIND_UNKNOWN = 0,

        TYPE_KIND_VOID,

        TYPE_KIND_INT8 ,
        TYPE_KIND_INT16,
        TYPE_KIND_INT32,
        TYPE_KIND_INT64,

        TYPE_KIND_FLOAT32,
        TYPE_KIND_FLOAT64,

    } type_kind_t;
 
    typedef enum type_flag_e
    {

        TYPE_FLAG_SIGNED = 0,

    } type_flag_t;

    typedef struct type_t
    {

        type_kind_t kind;

        char *name;

        uint32_t kind_id;

        size_t size;

    } type_t;

    size_t   get_type_size(type_kind_t kind, type_t **types, size_t types_count);

    uint32_t get_type_kind(type_kind_t kind, type_t **types, size_t types_count);

#endif
