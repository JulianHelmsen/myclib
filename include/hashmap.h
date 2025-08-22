#ifndef MYLIBC_HASHMAP_H
#define MYLIBC_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

typedef size_t(*hashmap_hash_proc)(const void*);
typedef bool(*hashmap_eq_proc)(const void*, const void*);

typedef struct {
    hashmap_hash_proc hash;
    hashmap_eq_proc eq;
    size_t capacity; // in elements
    size_t size; // number of elements
    void* data;
    

    size_t key_size;
    size_t value_size;
} hashmap;



#define member_size(type, member) sizeof(((type*)NULL)->member)

#define hashmap_create(key_type, value_type, hash, eq) \
    hashmap_create_impl(sizeof(key_type), sizeof(value_type), \
            hash, eq)

#define hashmap_impl(suffix, key_type, value_type) \
    static bool hashmap_put_##suffix(hashmap* map, key_type key, value_type value) { \
        return hashmap_put(map, &key, &value); \
    } \
    static bool hashmap_contains_##suffix(hashmap* map, key_type key) { \
        return hashmap_contains(map, &key); \
    } \
    static bool hashmap_get_##suffix(hashmap* map, key_type key, value_type* value) { \
        return hashmap_get(map, &key, value); \
    } \
    static bool hashmap_remove_##suffix(hashmap* map, key_type key) { \
        return hashmap_remove(map, &key); \
    }





hashmap hashmap_create_impl(size_t key_size, size_t value_size, hashmap_hash_proc hash, hashmap_eq_proc eq);
void hashmap_destroy(hashmap* map);

bool hashmap_contains(hashmap* map, const void* key);
bool hashmap_put(hashmap* map, const void* key, const void* value);
bool hashmap_get(hashmap* map, const void* key, void* value);
bool hashmap_remove(hashmap* map, const void* key);


#endif
