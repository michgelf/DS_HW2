#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "array.h"
#include "linked_list.h"
#include <utility>
#include <stdexcept>


template<typename T>
class HashTable {
private:

    typedef int hash_key_t;
    typedef std::pair<hash_key_t, T> pair_t;
    typedef Array<LinkedList<pair_t>> ArrayOfLists;
    static const size_t INITIAL_SIZE = 8;
    ArrayOfLists m_table;
    size_t m_size;

    /**
    * @brief Hashes a key to an index in the hash m_table
    */
    size_t hash(hash_key_t key) const;

    /**
     * @brief Rehashes the hash m_table to a new size
     */
    void rehash(size_t new_size);

    /**
     * @brief Resizes the hash m_table if the load factor exceeds bounds.
     */
    void resizeIfNeeded();

public:
    HashTable();

    virtual ~HashTable() = default;

    HashTable(const HashTable& other) = default;

    HashTable(HashTable&& other) noexcept = default;

    HashTable& operator=(const HashTable& other) = default;

    HashTable& operator=(HashTable&& other) noexcept = default;

    /**
     * @brief Retrieves or inserts a default value for the given key.
     * if first does not exist, inserts a new first with a default second.
     * assumes that if this is not the wanted behavior, the user will use contains() before
     * @return a reference to the value
     */
    T& operator[](hash_key_t key);

    /**
     * @brief Retrieves a value for the given key (const version).
     * @return a reference to the value
     */
    const T& operator[](hash_key_t key) const;

    /**
    * @brief Removes a key from the hash m_table
    */
    void remove(hash_key_t key);

    /**
    * @brief Checks if a key is in the hash m_table
    */
    bool contains(hash_key_t key) const;

    /**
    * @brief Checks if the hash m_table is empty
    */
    bool empty() const;

    /**
    * @brief Retrieves the first element in the hash m_table
    */
    const T& front() const;

    /**
    * @brief Retrieves the size of the hash m_table
    */
    size_t size() const;

};

// Implementation

template<class T>
HashTable<T>::HashTable() : m_table(INITIAL_SIZE), m_size(0) {}


template<class T>
T& HashTable<T>::operator[](hash_key_t key) {
    size_t index = hash(key);
    for (auto& pair: m_table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }

    // Key not found, insert default second
    m_table[index].append(pair_t(key, T()));
    ++m_size;
    resizeIfNeeded();

    return m_table[hash(key)].back().second;
}

template<class T>
const T& HashTable<T>::operator[](hash_key_t key) const {
    size_t index = hash(key);
    for (const auto& pair: m_table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::runtime_error("Key not found");
}

template<class T>
void HashTable<T>::remove(hash_key_t key) {
    size_t index = hash(key);
    auto& lst = m_table[index];
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        if (it->first == key) {
            lst.remove(it);
            --m_size;
            resizeIfNeeded();
            return;
        }
    }
}

template<class T>
bool HashTable<T>::contains(hash_key_t key) const {
    size_t index = hash(key);
    for (const auto& entry: m_table[index]) {
        if (entry.first == key) {
            return true;
        }
    }
    return false;
}


template<class T>
bool HashTable<T>::empty() const {
    return m_size == 0;
}

template<class T>
const T& HashTable<T>::front() const {
    for (auto& lst: m_table) {
        if (!lst.empty()) {
            return lst.back().second;
        }
    }
    throw std::runtime_error("No elements found in the hash m_table");
}

template<class T>
size_t HashTable<T>::size() const {
    return m_size;
}


// private methods

template<class T>
size_t HashTable<T>::hash(hash_key_t key) const {
    return key % m_table.size();
}

template<class T>
void HashTable<T>::rehash(size_t new_size) {
    ArrayOfLists new_table(new_size);

    for (const auto& lst: m_table) {
        for (const auto& pair: lst) {
            size_t new_index = pair.first % new_size;
            new_table[new_index].append(std::move(pair));
        }
    }

    m_table = std::move(new_table);
}

template<class T>
void HashTable<T>::resizeIfNeeded() {
    size_t tableSize = m_table.size();
    double load_factor = static_cast<double>(m_size) / tableSize;
    if (load_factor > 1.0) {
        rehash(tableSize * 2);
    } else if (load_factor < 0.25 && tableSize > 8) {
        rehash(tableSize / 2);
    }
}


#endif //HASH_TABLE_H
