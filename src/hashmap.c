#include <hashmap.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define HASHMAP_MIN_CAPACITY (1024)
#define MAX_FILL (4)

static bool ensure_capacity(hashmap* map);

enum bucket_state {
    EMPTY,
    TOMBSTONE,
    OCCUPIED
};

hashmap hashmap_create_impl(size_t key_size, size_t value_size, hashmap_hash_proc hash, hashmap_eq_proc eq) {
    return (hashmap) {
        .capacity = 0,
        .size = 0,
        .data = NULL,
        .hash = hash,
        .eq = eq,
        .key_size = key_size,
        .value_size = value_size,
    };
}

void hashmap_destroy(hashmap* map) {
    free(map->data);
    memset(map, 0, sizeof(*map));
}

size_t round_up_2(size_t v) {
    // https://en.wikipedia.org/wiki/Quadratic_probing
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}


static inline void bucket_at(hashmap* map, size_t idx, enum bucket_state** dst_state, void** dst_key_addr, void** dst_value_addr) {
    const size_t bucket_size = sizeof(enum bucket_state) + map->key_size + map->value_size;
    void* bucket = (char*) map->data + bucket_size * idx;

    enum bucket_state* pstate = bucket;
    void* pkey = (void*) (pstate + 1);
    void* pvalue = (void*) ((char*) pkey + map->key_size);

    *dst_state = pstate;
    *dst_key_addr = pkey;
    *dst_value_addr = pvalue;
}


bool hashmap_put_impl_suff_cap(hashmap* map, const void* key, const void* value) {
    assert(map->data != NULL);

    const size_t hash = map->hash(key);
    const size_t mod = round_up_2(map->capacity);

    size_t slot;
    size_t i = 0;
    enum bucket_state* bucket_state;

    void* bucket_key = NULL;
    void* bucket_value = NULL;
    bool is_replacement = false;
    do {
        do {
            slot = (hash + (i + i * i) / 2) % mod;
            ++i;
        } while(slot >= map->capacity);

        bucket_at(map, slot, &bucket_state, &bucket_key, &bucket_value);

        if(*bucket_state == EMPTY) break;
        if(*bucket_state == OCCUPIED && map->eq(key, bucket_key)) {
            is_replacement = true;
            break;
        }

    } while(1);

    if(!is_replacement) map->size += 1;
    *bucket_state = OCCUPIED;
    memcpy(bucket_key, key, map->key_size);
    memcpy(bucket_value, value, map->value_size);

    return true;
}

bool hashmap_put(hashmap* map, const void* key, const void* value) {
    if(!ensure_capacity(map))
        return false;
    return hashmap_put_impl_suff_cap(map, key, value);
}

bool hashmap_get(hashmap* map, const void* key, void* value) {
    if(map->size == 0) return false;
    assert(map->data != NULL);

    const size_t hash = map->hash(key);
    const size_t mod = round_up_2(map->capacity);

    size_t slot;
    size_t i = 0;
    do {
        do {
            slot = (hash + (i + i * i) / 2) % mod;
            ++i;
        } while(slot >= map->capacity);

        enum bucket_state* bucket_state;
        void* bucket_key = NULL;
        void* bucket_value = NULL;
        bucket_at(map, slot, &bucket_state, &bucket_key, &bucket_value);

        if(*bucket_state == EMPTY) return false;

        if(*bucket_state == OCCUPIED && map->eq(key, bucket_key)) {
            memcpy(value, bucket_value, map->value_size);
            return true;
        }

    } while(1);

    return false;
}

bool hashmap_contains(hashmap* map, const void* key) {
    if(map->size == 0) return false;
    assert(map->data != NULL);

    const size_t hash = map->hash(key);
    const size_t mod = round_up_2(map->capacity);

    size_t slot;
    size_t i = 0;
    do {
        do {
            slot = (hash + (i + i * i) / 2) % mod;
            ++i;
        } while(slot >= map->capacity);

        enum bucket_state* bucket_state;
        void* bucket_key = NULL;
        void* bucket_value = NULL;
        bucket_at(map, slot, &bucket_state, &bucket_key, &bucket_value);

        if(*bucket_state == EMPTY) return false;

        if(*bucket_state == OCCUPIED && map->eq(key, bucket_key)) return true;
    } while(1);

    return false;
}


static bool ensure_capacity(hashmap* map) {
    if(map->capacity >= HASHMAP_MIN_CAPACITY && map->size * MAX_FILL <= map->capacity)
        return true;

    const size_t new_cap = map->capacity < HASHMAP_MIN_CAPACITY / MAX_FILL ? HASHMAP_MIN_CAPACITY : map->capacity * MAX_FILL;

    hashmap new_map = hashmap_create_impl(map->key_size, map->value_size, map->hash, map->eq);
    new_map.capacity = new_cap;
    new_map.data = calloc(new_cap, (sizeof(enum bucket_state) + map->key_size + map->value_size));
    if(new_map.data == NULL)
        return false;

    for(size_t i = 0; i < map->capacity; ++i) {
        enum bucket_state* state = NULL;
        void* key = NULL;
        void* value = NULL;
        bucket_at(map, i, &state, &key, &value);

        if(*state == OCCUPIED) {
            hashmap_put_impl_suff_cap(&new_map, key, value);
        }
    }
    hashmap_destroy(map);
    *map = new_map;
    return true;
}

bool hashmap_remove(hashmap* map, const void* key) {
    if(map->size == 0) return false;
    assert(map->data != NULL);

    const size_t hash = map->hash(key);
    const size_t mod = round_up_2(map->capacity);

    size_t slot;
    size_t i = 0;
    do {
        do {
            slot = (hash + (i + i * i) / 2) % mod;
            ++i;
        } while(slot >= map->capacity);

        enum bucket_state* bucket_state;
        void* bucket_key = NULL;
        void* bucket_value = NULL;
        bucket_at(map, slot, &bucket_state, &bucket_key, &bucket_value);

        if(*bucket_state == EMPTY) return false;

        if(*bucket_state == OCCUPIED && map->eq(key, bucket_key)) {
            *bucket_state = TOMBSTONE;
            map->size -= 1;
            return true;
        }
    } while(1);
    return false;
}
