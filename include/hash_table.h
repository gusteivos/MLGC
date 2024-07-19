
#ifndef _MLG_HASH_TABLE_H_
#define _MLG_HASH_TABLE_H_

    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>


    typedef uint64_t (*hash_function_t) (void *, size_t);

    typedef struct hash_table_entry_s
    {

        uint8_t *key;

        size_t key_length;

        void *value;

        struct hash_table_entry_s *next;

    } hash_table_entry_t;

    hash_table_entry_t *create_hash_table_entry(uint8_t *key, size_t key_length, void *value);

    bool fprint_hash_table_entry(FILE *s, hash_table_entry_t *entry);

    bool destroy_hash_table_entry(hash_table_entry_t *entry, void (*) (void *));

    typedef struct hash_table_s
    {

        size_t capacity;

        hash_function_t hash_function;

        hash_table_entry_t **entrys;

    } hash_table_t;

    hash_table_t *create_hash_table(size_t capacity, hash_function_t hash_function);

    bool fprint_hash_table(FILE *s, hash_table_t *table);

    bool hash_table_entry_index(hash_table_t *table, uint8_t *key, size_t key_lenght, size_t *entry_index);

    bool hash_table_insert(hash_table_t *table, uint8_t *key, size_t key_lenght, void *value);

    bool hash_table_lookup(hash_table_t *table, uint8_t *key, size_t key_lenght, void **value);

    bool hash_table_delete(hash_table_t *table, uint8_t *key, size_t key_lenght, void **value);
    
    bool destroy_hash_table(hash_table_t *table, void (*) (void *));

#endif
