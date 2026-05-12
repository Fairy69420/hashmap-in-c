/* This file was automatically generated.  Do not edit! */

#ifndef HASHMAP_H
#define HASHMAP_H


typedef struct {
    const void *value;
    const size_t size;
} Value;

typedef struct Map Map;

void map_Insert(Map *map,const char *key,const void *value,size_t value_size);
int map_RemoveKey(Map *map,const char *key);
void map_Destroy(Map *map);
Map *map_Create(size_t initial_entry_count);

#endif