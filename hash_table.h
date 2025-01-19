//
// Created by michg on 1/18/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_HASH_TABLE_H
#define DS_WET2_WINTER_2024_2025_HASH_TABLE_H

#include <vector>
#include "linked_list.h"

using namespace std;

template<typename K, typename V>
class KeyValuePair {
public:
    K first;
    V second;

    // Default constructor
    //KeyValuePair() = default;

    // Constructor with key and value
    KeyValuePair(const K& key, const V& value) : first(key), second(value) {}

//    // Move constructor with key and value
//    KeyValuePair(K&& key, V&& value)
//            : first(std::move(key)), second(std::move(value)) {}

    // Perfect forwarding constructor
    template<typename U1, typename U2>
    KeyValuePair(U1&& key, U2&& value): first(std::move(key)), second(std::move(value)) {}

};

template<typename T>
class HashTable {
private:
    const unsigned int INITIAL_SIZE = 8;
    std::vector<LinkedList<KeyValuePair<int, T>>> table;
    unsigned int num_elements;


    unsigned int hash(int key) const {
        return key % table.size();
    }

    void rehash(unsigned int new_size) {
        vector<LinkedList<KeyValuePair<int, T>>> new_table(new_size);

        for (const auto& bucket: table) {
            for (auto& pair: bucket) {
                unsigned int new_index = pair.first % new_size;
                new_table[new_index].emplace_back(std::move(pair));
            }
        }

        table = std::move(new_table);
    }

    void check_and_resize() {
        double load_factor = static_cast<double>(num_elements) / table.size();

        if (load_factor > 1.0) {
            rehash(table.size() * 2);
        } else if (load_factor < 0.25 && table.size() > 8) {
            rehash(table.size() / 2);
        }
    }

public:
    HashTable() : table(INITIAL_SIZE), num_elements(0) {}

    // If key does not exist, inserts a new key with a default value.
    // Assumes that if this is not the wanted behavior, the user will use contains() before
    T& operator[](int key) {
        unsigned int index = hash(key);
        for (auto& entry: table[index]) {
            if (entry.first == key) {
                return entry.second;
            }
        }

        // Key not found, insert default value
        auto& x = table[index];
        x.emplace_back(KeyValuePair<int, T>(key, T()));
        ++num_elements;
        check_and_resize();

        return table[hash(key)].back().second;
    }

    void erase(int key) {
        unsigned int index = hash(key);
        auto& bucket = table[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if ((*it).first == key) {
                bucket.erase(it);
                --num_elements;
                check_and_resize();
                return;
            }
        }
    }

    bool contains(int key) const {
        unsigned int index = hash(key);
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
        for (auto& bucket: table) {
            if (!bucket.empty()) {
                return bucket.back().second;
            }
        }
        throw std::runtime_error("No elements found in the hash table");
    }

    unsigned int size() const {
        return num_elements;
    }
};


#endif //DS_WET2_WINTER_2024_2025_HASH_TABLE_H
