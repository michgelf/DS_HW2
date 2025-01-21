//
// Created by michg on 1/18/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_HASH_TABLE_H
#define DS_WET2_WINTER_2024_2025_HASH_TABLE_H

#include "linked_list.h"
#include "array.h"

using namespace std;

template<typename K, typename V>
class KeyValuePair {
public:
    K first;
    V second;

    // Constructor with key and value
    KeyValuePair(const K& key, const V& value) : first(key), second(value) {}
};

template<typename T>
class HashTable {
private:

    typedef int hash_key_t;
    typedef Array<LinkedList<KeyValuePair<hash_key_t, T>>> ArrayOfLists;
    typedef KeyValuePair<hash_key_t, T> pair_t;
    static const size_t INITIAL_SIZE = 8;
    ArrayOfLists table;
    size_t num_elements;


    size_t hash(hash_key_t key) const {
        return key % table.get_size();
    }

    void rehash(size_t new_size) {
        ArrayOfLists new_table(new_size);

        for (const auto& lst: table) {
            for (const auto& pair: lst) {
                size_t new_index = pair.first % new_size;
                new_table[new_index].emplace_back(std::move(pair));
            }
        }


        table = std::move(new_table);
    }

    void check_and_resize() {
        size_t tableSize = table.get_size();
        double load_factor = static_cast<double>(num_elements) / tableSize;
        if (load_factor > 1.0) {
            rehash(tableSize * 2);
        } else if (load_factor < 0.25 && tableSize > 8) {
            rehash(tableSize / 2);
        }
    }

public:
    HashTable() : table(INITIAL_SIZE), num_elements(0) {}


    virtual ~HashTable() = default;

    HashTable(const HashTable& other) = default;

    HashTable(HashTable&& other) noexcept = default;


    HashTable& operator=(const HashTable& other) = default;

    HashTable& operator=(HashTable&& other) noexcept = default;

    // If key does not exist, inserts a new key with a default value.
    // Assumes that if this is not the wanted behavior, the user will use contains() before

    // todo consider make a const version
    T& operator[](hash_key_t key) {
        size_t index = hash(key);
        for (auto& pair: table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }

        // Key not found, insert default value
        table[index].emplace_back(pair_t(key, T()));
        ++num_elements;
        check_and_resize();

        return table[hash(key)].back().second;
    }

    const T& operator[](hash_key_t key) const {
        size_t index = hash(key);
        for (const auto& pair: table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::runtime_error("Key not found");
    }


    void erase(hash_key_t key) {
        size_t index = hash(key);
        auto& lst = table[index];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->first == key) {
                lst.erase(it);
                --num_elements;
                check_and_resize();
                return;
            }
        }
    }

    bool contains(hash_key_t key) const {
        size_t index = hash(key);
        for (const auto& entry: table[index]) {
            if (entry.first == key) {
                return true;
            }
        }
        return false;
    }


    bool empty() const {
        return num_elements == 0;
    }

    const T& front() {
        for (auto& lst: table) {
            if (!lst.empty()) {
                return lst.back().second;
            }
        }
        throw std::runtime_error("No elements found in the hash table");
    }

    size_t size() const {
        return num_elements;
    }

};


#endif //DS_WET2_WINTER_2024_2025_HASH_TABLE_H
