//
// Created by michg on 1/20/2025.
//

#ifndef C_SCRIPTS_ARRAY_H
#define C_SCRIPTS_ARRAY_H


#include <iostream>
#include <utility>

template<typename T>
class Array {
    size_t size;
    T* data;

public:
    // Nested iterator class
    class Iterator {
    private:
        T* current;

    public:
        explicit Iterator(T* ptr) : current(ptr) {}

        T& operator*() { return *current; }

        T* operator->() { return current; }

        Iterator& operator++() {
            ++current;
            return *this;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }

        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    // Const iterator
    class ConstIterator {
    private:
        const T* current;

    public:
        explicit ConstIterator(const T* ptr) : current(ptr) {}

        const T& operator*() const { return *current; }

        const T* operator->() const { return current; }

        ConstIterator& operator++() {
            ++current;
            return *this;
        }

        bool operator==(const ConstIterator& other) const { return current == other.current; }

        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };

    // Constructor
    explicit Array(size_t size) : size(size), data(new T[size]) {}

    // Destructor
    virtual ~Array() {
        delete[] data;
    }

    // Copy constructor
    Array(const Array& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Move constructor
    Array(Array&& other) noexcept: data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    // Copy assignment operator
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Move assignment operator
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Access element
    T& operator[](size_t index) { return data[index]; }

    const T& operator[](size_t index) const { return data[index]; }

    // Iterators
    Iterator begin() { return Iterator(data); }

    Iterator end() { return Iterator(data + size); }

    ConstIterator begin() const { return ConstIterator(data); }

    ConstIterator end() const { return ConstIterator(data + size); }

    // Size
    size_t get_size() const { return size; }
};


#endif //C_SCRIPTS_ARRAY_H
