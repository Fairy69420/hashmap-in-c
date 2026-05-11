#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct {
    const char *Key;
    size_t ValueSize;
    const void *Value;
} Data;

typedef struct {
    Data **Data_ptr_arr;//pointers to Data structs - Not the Data struct itself
    size_t elem_Count;
    size_t Arr_Size;
} Map;

// Public API
Map *map_Create(size_t initial_entry_count);
void map_Insert(Map *Map_ptr, const char *key, const void *value, size_t V_Size);
Data *map_Get(Map *Map_ptr, const char *Search_key);
size_t map_GetValue(Map *Map_ptr, const char *search_Key, void **Value);
char **map_GetKeys(Map *Map_ptr, size_t *key_count);
void map_Destroy(Map *Map_ptr);
size_t map_GetKeys_MEMsafe(Map *Map_ptr, char **Keys_BUFF, size_t BUFF_len)

#endif // !HASHMAP_H


//changed key and value to const to prevent mutation. And force explicit use of proper functions