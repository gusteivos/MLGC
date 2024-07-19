#include "hash_table.h"
#include "util.h"


hash_table_entry_t *create_hash_table_entry(uint8_t *key, size_t key_length, void *value)
{

    hash_table_entry_t *entry = (hash_table_entry_t *)a_alloc(sizeof(hash_table_entry_t));

    entry->key_length = key_length;

    entry->key = a_alloc(entry->key_length);

    memcpy(entry->key, key, entry->key_length);

    entry->value = value;

    entry->next = NULL;

    return entry;

}

bool fprint_hash_table_entry(FILE *s, hash_table_entry_t *entry)
{

    if (!s)
    {

        return true;

    }

    abort_if_null(entry);

    int q = fprintf(s, "Hash table entry:\n\t");

    if (q < 0)
    {

        return false;

    }

    while (entry)
    {

        q = fprintf(s, "- Key Pointer: %p, Key Length: %zu, Value: %p ", entry->key, entry->key_length, entry->value);

        if (q < 0)
        {

            return false;

        }

        entry = entry->next;

    }

    q = fputc('\n', s);

    return !(q < 0);

}

bool destroy_hash_table_entry(hash_table_entry_t *entry, void (*destroy_value) (void *value))
{

    abort_if_null(entry);

    if (entry->value && destroy_value)
    {

        destroy_value(entry->value);

    }

    /*TODO: destroy next.*/

    free(entry);

    return true;

}

hash_table_t *create_hash_table(size_t capacity, hash_function_t hash_function)
{

    abort_if_null(hash_function);

    if (capacity == 0)
    {

        return NULL;

    }

    hash_table_t *table = (hash_table_t *)a_alloc(sizeof(hash_table_t));

    table->capacity = capacity;

    table->hash_function = hash_function;

    table->entrys = (hash_table_entry_t **)a_clear_alloc(table->capacity, sizeof(hash_table_entry_t *));

    return table;

}

bool fprint_hash_table(FILE *s, hash_table_t *table)
{

    if (!s)
    {

        return true;

    }

    abort_if_null(table);

    int q = fprintf(s, "Hash table:\n\tCapacity: %zu\n\tHash Function Pointer: %p\n", table->capacity, table->hash_function);

    if (q < 0)
    {

        return false;

    }

    q = fprintf(s, "Start used table entrys:\n");

    for (size_t w = 0; w < table->capacity; w++)
    {

        hash_table_entry_t *current_entry = table->entrys[w];

        if (current_entry)
        {

            q = fprintf(s, "[%zu] - ", w);

            if (q < 0)
            {

                return false;

            }

            bool e = fprint_hash_table_entry(s, current_entry);

            if (!e)
            {

                return false;

            }

        }

    }

    q = fprintf(s, "End used table entrys.\n");

    return !(q < 0);

}

bool hash_table_entry_index(hash_table_t *table, uint8_t *key, size_t key_lenght, size_t *entry_index)
{

    abort_if_null(table);

    abort_if_null(key);

    abort_if_null(entry_index);

    if (key_lenght == 0)
    {

        return false;

    }

    *entry_index = ((size_t)table->hash_function((void *)key, key_lenght) % table->capacity);

    return true;

}

bool hash_table_insert(hash_table_t *table, uint8_t *key, size_t key_lenght, void *value)
{

    abort_if_null(table);

    abort_if_null(key);

    if (key_lenght == 0)
    {

        return false;

    }

    void *lookup_value = NULL;

    if (hash_table_lookup(table, key, key_lenght, &lookup_value) && lookup_value == value)
    {

        return false;

    }

    size_t entry_index = 0;

    hash_table_entry_index(table, key, key_lenght, &entry_index);

    hash_table_entry_t *entry = table->entrys[entry_index];

    hash_table_entry_t *new_entry = create_hash_table_entry(key, key_lenght, value);

    if (!entry)
    {

        table->entrys[entry_index] = new_entry;

        return true;

    }

    while (entry->next)
    {

        entry = entry->next;

    }

    entry->next = new_entry;

    return true;

}

bool hash_table_lookup(hash_table_t *table, uint8_t *key, size_t key_lenght, void **value)
{

    abort_if_null(table);

    abort_if_null(key);

    if (key_lenght == 0)
    {

        return false;

    }

    size_t entry_index = 0;

    hash_table_entry_index(table, key, key_lenght, &entry_index);

    hash_table_entry_t *current_entry = table->entrys[entry_index];

    while (current_entry && current_entry->key_length == key_lenght && memcmp(current_entry->key, key, key_lenght))
    {

        current_entry = current_entry->next;

    }

    if (!current_entry)
    {

        return false;

    }

    if (value)
    {


        *value = current_entry->value;

    }

    return true;

}

bool hash_table_delete(hash_table_t *table, uint8_t *key, size_t key_lenght, void **value)
{

    abort_if_null(table);

    abort_if_null(key);

    if (key_lenght == 0)
    {

        return false;

    }

    size_t entry_index = 0;

    hash_table_entry_index(table, key, key_lenght, &entry_index);

    hash_table_entry_t *current_entry = table->entrys[entry_index];

    hash_table_entry_t *previous_entry = NULL;

    while (current_entry && current_entry->key_length == key_lenght && memcmp(current_entry->key, key, key_lenght))
    {

        previous_entry = current_entry;

        current_entry = current_entry->next;

    }

    if (!current_entry)
    {

        return true;

    }

    if (!previous_entry)
    {

        table->entrys[entry_index] = current_entry->next;

    }
    else
    {

        previous_entry->next = current_entry->next;

    }

    void *entry_value = current_entry->value;

    if (value)
    {

        *value = entry_value;

    }

    return destroy_hash_table_entry(current_entry, NULL);

}

bool destroy_hash_table(hash_table_t *table, void (*destroy_entry_value) (void *entry_value))
{

    abort_if_null(table);

    /*TODO: destroy entrys.*/

    free(table->entrys);

    free(table);

    return true;

}
