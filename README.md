# Hashmap

A generic, open-addressed hashmap in C. Store any value type using string keys. Automatically resizes when needed.

## API

### `Map *map_Create(size_t initial_entry_count)`
Create a new map with given initial capacity.

### `void map_Insert(Map *map, const char *key, const void *value, size_t value_size)`
Insert or update a key-value pair. Automatically resizes when load factor exceeds 0.75.

### `int map_RemoveKey(Map *map, const char *key)`
Remove a key from the map. Returns `1` if found, `-1` if not found.

### `void map_Destroy(Map *map)`
Free all map memory.

## Quick Example

```c
Map *map = map_Create(16);

int val = 42;
map_Insert(map, "answer", &val, sizeof(int));

map_RemoveKey(map, "answer");

map_Destroy(map);
```

## Features

- **Generic values**: Store any data type by passing pointer and size
- **Auto-resize**: Doubles capacity when needed
- **Simple API**: Just 4 functions

## Limitations

- No retrieval API (no `map_Get()`)
- No key iteration
- Not thread-safe
- Deletion uses tombstones (can fragment over time)

## Future Ideas

- `map_Get()` – retrieve values by key
- `map_GetAllKeys()` / iterator API – iterate over keys
- `map_HasKey()` – check key existence
- `map_Rebuild()` – clean up tombstones after deletions
- Thread-safe variants (e.g., with mutex or RWlock)


