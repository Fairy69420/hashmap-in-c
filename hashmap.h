/* This file was automatically generated.  Do not edit! */
#ifndef HASHMAP_H
#define HASHMAP_H


typedef struct {
    const void *value;
    const size_t size;
} Value;

typedef struct Map Map;


const char *map_get_valueStr(Map *map,const char *search_key);
int map_HasKey(Map *map,const char *search_key);
size_t map_len(Map *map);
Value map_get_value(Map *map,const char *search_key);
void map_Insert(Map *map,const char *key,const void *value,size_t value_size);
int map_RemoveKey(Map *map,const char *key);
void map_Destroy(Map *map);
Map *map_Create(size_t initial_entry_count);

#endif