// version 1.1
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

#define TOMB 0x1
#define LOAD_FACTOR 0.75

typedef struct {
    const char *Key;
    size_t ValueSize;
    const void *Value;
} Data;

typedef struct {
    Data **Ptr_to_Array_of_Data_ptrs;//pointers to Data structs - Not the Data struct itself
    size_t Element_Count;
    size_t Array_Size;
} Map;

Map *map_Create(size_t initial_entry_count){
    Map *Map_init_ptr = malloc(sizeof(Map));

    Map_init_ptr->Ptr_to_Array_of_Data_ptrs = calloc(initial_entry_count, sizeof(Data *));
    Map_init_ptr->Element_Count = 0;
    Map_init_ptr->Array_Size = initial_entry_count;

    return Map_init_ptr;
}


// does not set TOMB flag after freeing the pointer. MUST SET THE TOMB FLAG IF USED
static void Data_struct_Destroy(Data *Data_ptr){
    if (Data_ptr == NULL || Data_ptr == TOMB) return;
    free((void *)Data_ptr->Key);
    free((void *)Data_ptr->Value);
    free((void *)Data_ptr);
}

void map_Destroy(Map *map){
    for (size_t i = 0; i < map->Array_Size; i++){
        if (map->Ptr_to_Array_of_Data_ptrs[i] != NULL && map->Ptr_to_Array_of_Data_ptrs[i] != TOMB){
            Data_struct_Destroy(map->Ptr_to_Array_of_Data_ptrs[i]);
        }
    }

    free((void *)(map->Ptr_to_Array_of_Data_ptrs));
    free((void *)map);
}

static int hash(const char *key, size_t size) {
    unsigned long hash_value = 5381;
    for (size_t i = 0; key[i]; i++) {
        hash_value = ((hash_value << 5) + hash_value) + key[i];
    }
    return hash_value % size;
}

static void index_Assign(Data **Ptr_to_Array_of_Data_ptrs, Data *Data_ptr, size_t index, size_t Array_Size){
    while(Ptr_to_Array_of_Data_ptrs[index] != NULL && Ptr_to_Array_of_Data_ptrs[index] != TOMB){
        index = (index + 1) % Array_Size;
    }

    Ptr_to_Array_of_Data_ptrs[index] = Data_ptr;
}

//returns -1 if key not found.
static size_t internal_key_exist_search(Map *map, const char *search_key){
    // returns an index that is relative to Ptr_to_Array_of_Data_ptrs: Relative to Data pointer array
    size_t index = hash(search_key, map->Array_Size);
    while (map->Ptr_to_Array_of_Data_ptrs[index] != NULL){
        if ((map->Ptr_to_Array_of_Data_ptrs[index] != TOMB) && (strcmp(map->Ptr_to_Array_of_Data_ptrs[index]->Key, search_key) == 0)){
            return index;
        }
        index = (index + 1) % map->Array_Size;
    }
    return -1;
}

int map_RemoveKey(Map *map, const char *key){
    size_t index = internal_key_exist_search(map, key);
    if (index == -1){
        return -1;
    }

    Data_struct_Destroy(map->Ptr_to_Array_of_Data_ptrs[index]);
    map->Ptr_to_Array_of_Data_ptrs[index] = TOMB;

    return 1;
}


static void map_Resize(Map *map){//Doubles the Arr_size

    size_t old_Size = map->Array_Size;
    size_t new_Size = old_Size*2;
    Data **old_Data_ptr_arr = map->Ptr_to_Array_of_Data_ptrs;
    map->Ptr_to_Array_of_Data_ptrs = calloc(new_Size, sizeof(Data *));

    // Re-Hashing
    for(size_t i = 0; i < old_Size; i++){
        if (old_Data_ptr_arr[i] == NULL || old_Data_ptr_arr[i] == TOMB){
            continue;
        }
        size_t index = hash(old_Data_ptr_arr[i]->Key, new_Size);

        index_Assign(map->Ptr_to_Array_of_Data_ptrs, old_Data_ptr_arr[i], index, new_Size);
        
    }

    free((void *)old_Data_ptr_arr);
    map->Array_Size = new_Size;
}

static Data *create_Data_struct(const char *key, const void *value, size_t value_size){
    Data *data = malloc(sizeof(Data));
    char *tmp_key = malloc(strlen(key) + 1);
    strcpy(tmp_key, key);
    void *tmp_value = malloc(value_size);
    memcpy(tmp_value, value, value_size);

    data->Key = tmp_key;
    data->Value = tmp_value;
    data->ValueSize = value_size;

    return data;
}

void map_Insert(Map *map, const char *key, const void *value, size_t value_size){
    size_t index = internal_key_exist_search(map, key);
    if(index != -1){
        Data_struct_Destroy(map->Ptr_to_Array_of_Data_ptrs[index]);
        map->Ptr_to_Array_of_Data_ptrs[index] = 0x0;

        Data *tmp_data = create_Data_struct(key, value, value_size);
        index_Assign(map->Ptr_to_Array_of_Data_ptrs, tmp_data, index, map->Array_Size);

        return;
    }

    index = hash(key, map->Array_Size);
    Data *tmp_data = create_Data_struct(key, value, value_size);
    index_Assign(map->Ptr_to_Array_of_Data_ptrs, tmp_data, index, map->Array_Size);
    //update element counter
    map->Element_Count += 1;

    //load check
    if ((float)((map->Element_Count)/(map->Array_Size)) >= LOAD_FACTOR){
        map_Resize(map);
    }
}


static Data *map_get_Data(Map *map, const char *Search_key){
    //probing
    size_t index = hash(Search_key, map->Array_Size);
    while(map->Ptr_to_Array_of_Data_ptrs[index] != NULL){//stop at empty
        if((map->Ptr_to_Array_of_Data_ptrs[index] != TOMB) && (strcmp(map->Ptr_to_Array_of_Data_ptrs[index]->Key, Search_key) == 0)){
            return map->Ptr_to_Array_of_Data_ptrs[index];
        }
        index = (index + 1) % map->Array_Size;
    }
    return NULL;// Found an empty slot. Means the key asked for does not exist.
}