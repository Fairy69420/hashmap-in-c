// version 1
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"


#define LOAD_FACTOR 0.75



Map *map_Create(size_t initial_entry_count){
    Map *Map_init_ptr = malloc(sizeof(Map));

    Map_init_ptr->Data_ptr_arr = calloc(initial_entry_count, sizeof(Data *));
    Map_init_ptr->elem_Count = 0;
    Map_init_ptr->Arr_Size = initial_entry_count;

    return Map_init_ptr;
}

int hash(const char *key, size_t size) {
    unsigned long hash_value = 5381;
    for (size_t i = 0; key[i]; i++) {
        hash_value = ((hash_value << 5) + hash_value) + key[i];
    }
    return hash_value % size;
}

void index_Assign(Data **Data_ptr_arr, Data *Data_ptr, size_t index, size_t Arr_Size){
    while(Data_ptr_arr[index] != NULL){
        index = (index + 1) % Arr_Size;
    }

    Data_ptr_arr[index] = Data_ptr;
}

void map_Resize(Map *Map_ptr){//Doubles the Arr_size
    size_t count = 0;
    size_t old_Size = Map_ptr->Arr_Size;
    size_t new_Size = old_Size*2;
    Data **old_Data_ptr_arr = Map_ptr->Data_ptr_arr;
    Map_ptr->Data_ptr_arr = calloc(new_Size, sizeof(Data *));

    // Re-Hashing
    for(size_t i = 0; i < old_Size; i++){
        if (old_Data_ptr_arr[i] == NULL){
            continue;
        }
        size_t index = hash(old_Data_ptr_arr[i]->Key, new_Size);

        index_Assign(Map_ptr->Data_ptr_arr, old_Data_ptr_arr[i], index, new_Size);
        
    }

    free(old_Data_ptr_arr);
    Map_ptr->Arr_Size = new_Size;
}

void map_Insert(Map *Map_ptr, const char *key, const void *value, size_t V_Size){

    //check if key exist
    Data *existing_Data = map_Get(Map_ptr, key);
    if (existing_Data != NULL){//means key exist
        free(existing_Data->Value);
        void *tmp_val_buff = malloc(V_Size);
        //existing_Data->Value = malloc(V_Size);
        existing_Data->ValueSize = V_Size;
        memcpy(tmp_val_buff, value, V_Size);
        existing_Data->Value = tmp_val_buff;

        return;//no need to do anything else since it's value updating in an already existing key
    }

    //writting actual data --- NEW ENTRY
    Data *Data_ptr = malloc(sizeof(Data)); //allocate memory for a data struct

    char *tmp_key_buff = malloc(strlen(key) + 1);
    //Data_ptr->Key = malloc(strlen(key) + 1);
    strcpy(tmp_key_buff, key);
    Data_ptr->Key = tmp_key_buff;
    Data_ptr->ValueSize = V_Size;
    void *tmp_value_buff = malloc(V_Size);
    //Data_ptr->Value = malloc(V_Size);
    memcpy(tmp_value_buff, value, V_Size);
    Data_ptr->Value = tmp_value_buff;

    //assign pointer to the table
    size_t index = hash(key, Map_ptr->Arr_Size);//calculate a hash for the key
    index_Assign(Map_ptr->Data_ptr_arr, Data_ptr, index, Map_ptr->Arr_Size);
    Map_ptr->elem_Count += 1;//increment the element counter

    // check if hashmap cross load factor after this insertion
    if (((float)Map_ptr->elem_Count/Map_ptr->Arr_Size) > LOAD_FACTOR){
        map_Resize(Map_ptr);//if load factor is crossed map will be re-sized
    }
}

void Data_struct_Destroy(Data *Data_ptr){
    if (Data_ptr == NULL) return;
    free(Data_ptr->Key);
    free(Data_ptr->Value);
    free(Data_ptr);
}

void map_Destroy(Map *Map_ptr){
    for (size_t i = 0; i < Map_ptr->Arr_Size; i++){
        if (Map_ptr->Data_ptr_arr[i] != NULL){
            Data_struct_Destroy(Map_ptr->Data_ptr_arr[i]);
        }
    }

    free(Map_ptr->Data_ptr_arr);
    free(Map_ptr);
}

Data *map_Get(Map *Map_ptr, const char *Search_key){
    //probing
    size_t index = hash(Search_key, Map_ptr->Arr_Size);
    while(Map_ptr->Data_ptr_arr[index] != NULL){//stop at empty
        if(strcmp(Map_ptr->Data_ptr_arr[index]->Key, Search_key) == 0){
            return Map_ptr->Data_ptr_arr[index];
        }
        index = (index + 1) % Map_ptr->Arr_Size;
    }
    return NULL;// Found an empty slot. Means the key asked for does not exist.
}

size_t map_GetValue(Map *Map_ptr, const char *search_Key, void **Value){//copy the Value pointer(of the asked key) to Value and returns the size of the value
    Data *retrieved_Data_struct = map_Get(Map_ptr, search_Key);
    if (retrieved_Data_struct == NULL){
        return 0;
    }

    *Value = retrieved_Data_struct->Value;
    return retrieved_Data_struct->ValueSize;

}

char **map_GetKeys(Map *Map_ptr, size_t *key_count){
    if (!Map_ptr->elem_Count) return NULL;
    char **keys = malloc(Map_ptr->elem_Count * sizeof(char *));
    *key_count = 0;
    for (size_t i = 0; i < Map_ptr->Arr_Size; i++){
        if (Map_ptr->Data_ptr_arr[i] != NULL){
            keys[(*key_count)++] = Map_ptr->Data_ptr_arr[i]->Key;
        }
    }
    return keys;
}

size_t map_GetKeys_MEMsafe(Map *Map_ptr, char **Keys_BUFF, size_t BUFF_len){
    if (BUFF_len < Map_ptr->elem_Count){
        return Map_ptr->elem_Count;
    }
    size_t size = 0;
    char **tmp_key_ptr_buff = map_GetKeys(Map_ptr, &size);
    memcpy(Keys_BUFF, tmp_key_ptr_buff, size);
    free(tmp_key_ptr_buff);

    return size;

}