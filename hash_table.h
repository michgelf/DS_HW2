//
// Created by michg on 1/18/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_HASH_TABLE_H
#define DS_WET2_WINTER_2024_2025_HASH_TABLE_H

#include <cassert>
#include <vector>
#include <list>

using namespace std;

template<typename T>
struct KeyValuePair {
    int key;
    T value;

    KeyValuePair(int key, const T& value) : key(key), value(value) {}
};

template<typename T>
class HashTable {
private:
    const unsigned int INITIAL_SIZE = 8;
    vector<list < KeyValuePair<T>>>
    table;
    unsigned int num_elements;


    unsigned int hash(int key) const {
        return key % table.size();
    }

    void rehash(unsigned int new_size) {
        vector<list < KeyValuePair<T>> > new_table(new_size);

        for (const auto& bucket: table) {
            for (const auto& pair: bucket) {
                unsigned int new_index = pair.key % new_size;
                new_table[new_index].emplace_back(pair);
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

    T& operator[](int key) {
        unsigned int index = hash(key);
        for (auto& entry: table[index]) {
            if (entry.key == key) {
                return entry.value;
            }
        }

        // Key not found, insert default value
        table[index].emplace_back(key, T());
        ++num_elements;
        check_and_resize();

        return table[hash(key)].back().value;
    }

    void erase(int key) {
        unsigned int index = hash(key);
        auto& bucket = table[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
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
            if (entry.key == key) {
                return true;
            }
        }
        return false;
    }


    bool empty() const {
        return num_elements == 0;
    }

    const T& front() {
        assert(!empty());
        for (auto& bucket: table) {
            if (!bucket.empty()) {
                return bucket.front().value;
            }
        }
        // shouldn't reach here
        throw std::runtime_error("No elements found in the hash table");
    }

    unsigned int size() const {
        return num_elements;
    }
};


#endif //DS_WET2_WINTER_2024_2025_HASH_TABLE_H
