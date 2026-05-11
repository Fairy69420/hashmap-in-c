# C Hashmap Implementation

A lightweight, dynamic-size hash map implementation in C using **Open Addressing (Linear Probing)** for collision resolution. This library supports generic `void *` values and handles string keys with automatic deep-copying.

## Features
- **Dynamic Resizing**: Automatically doubles the internal array size when the load factor exceeds 0.75.
- **Deep Copying**: Keys and values are copied into the map's own memory space, ensuring data persistence even if the original pointers go out of scope.
- **Linear Probing**: Efficiently handles hash collisions by searching for the next available slot.
- **Generic Values**: Stores any data type by taking a `void *` and the size of the data.

## API Overview

### `Map *map_Create(size_t initial_entry_count)`
Initializes a new hash map with the specified starting capacity.

### `void map_Insert(Map *Map_ptr, const char *key, const void *value, size_t V_Size)`
Inserts a key-value pair. If the key already exists, the old value is freed and replaced with the new one.

### `Data *map_Get(Map *Map_ptr, const char *Search_key)`
Retrieves the internal `Data` struct associated with a key.

### `size_t map_GetValue(Map *Map_ptr, const char *search_Key, void **Value)`
Copies the pointer of the value to the provided buffer and returns the size of the value.

### `char **map_GetKeys(Map *Map_ptr, size_t *key_count)`
Returns an array of all keys currently in the map.

### `void map_Destroy(Map *Map_ptr)`
Recursively frees all memory associated with the map, including all keys and values.

## Current Limitations & Future Improvements
- **Deletion**: Currently, the map does not support deleting specific keys. Implementing this will require "Tombstone" markers to maintain probe chains.
- **Error Handling**: `malloc` and `calloc` calls do not currently check for `NULL` returns (Out of Memory).
- **Thread Safety**: The current implementation is not thread-safe.
