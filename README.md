# Hashmap

A generic, open-addressed hashmap in C. Store any value type using string keys. Automatically resizes when needed.

## API

### `Map *map_Create(size_t initial_entry_count)`
Create a new map with given initial capacity.

### `void map_Insert(Map *map, const char *key, const void *value, size_t value_size)`
Insert or update a key-value pair. Automatically resizes when load factor exceeds 0.75.

### `Value map_get_value(Map *map, const char *search_key)`
Retrieve a value by key. Returns `{NULL, -1}` if key not found.

```c
Value result = map_get_value(map, "answer");
if (result.size != -1) {
    // key exists
}
```

### `int map_HasKey(Map *map, const char *search_key)`
Check if a key exists. Returns `1` if found, `0` if not.

### `size_t map_len(Map *map)`
Get the number of entries in the map (excluding tombstones).

### `int map_RemoveKey(Map *map, const char *key)`
Remove a key from the map. Returns `1` if found, `-1` if not found.

### `void map_Destroy(Map *map)`
Free all map memory.

### `const char *map_get_valueStr(Map *map, const char *search_key)` ⚠️
**Unsafe.** Only use if you know values are strings. Does not validate. Returns `"0"` if key not found.

## Quick Example

```c
Map *map = map_Create(16);

// Store an integer
int val = 42;
map_Insert(map, "answer", &val, sizeof(int));

// Retrieve it
Value result = map_get_value(map, "answer");
if (result.size != -1) {
    int *retrieved = (int *)result.value;
    printf("Value: %d\n", *retrieved);
}

// Check existence
if (map_HasKey(map, "answer")) {
    printf("Key exists\n");
}

// Get size
printf("Map has %zu entries\n", map_len(map));

// Remove
map_RemoveKey(map, "answer");

map_Destroy(map);
```

## Features

- **Generic values**: Store any data type by passing pointer and size
- **Auto-resize**: Doubles capacity when needed
- **Value retrieval**: `map_get_value()` returns both data and size
- **Key checking**: `map_HasKey()` for existence checks
- **Entry counting**: `map_len()` returns actual entry count

## Limitations

- No key iteration
- Not thread-safe
- Deletion uses tombstones (can fragment over time)
- String keys only

## Future Ideas

- `map_Rebuild()` – clean up tombstones and compact
- Iterator API – iterate over all keys
- Thread-safe variants (e.g., with mutex)
- Double hashing – reduce clustering in probe chains

---

## Changelog

### v0.03 - Beta
- Added `map_get_value()` – retrieve values with size info via `Value` struct
- Added `map_HasKey()` – check if key exists (returns 0/1)
- Added `map_len()` – get entry count
- Added `map_get_valueStr()` – unsafe string-specific retrieval (use with caution)

### v0.01 - Initial
- `map_Create()`, `map_Insert()`, `map_RemoveKey()`, `map_Destroy()`
- Linear probing with tombstone deletion
- Automatic resizing at 0.75 load factor
