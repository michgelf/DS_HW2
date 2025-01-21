#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef> // For size_t

template<typename T>
class Array {
    size_t m_size;
    T* m_data;

public:

    explicit Array(size_t size);

    virtual ~Array();

    Array(const Array& other);

    Array(Array&& other) noexcept;

    Array& operator=(const Array& other);

    Array& operator=(Array&& other) noexcept;

    T& operator[](size_t index);

    const T& operator[](size_t index) const;

    size_t size() const { return m_size; }

    class ConstIterator;

    ConstIterator begin() const;

    ConstIterator end() const;

};


template<class T>
class Array<T>::ConstIterator {
    const T* m_current;

    friend class Array<T>;

public:
    explicit ConstIterator(const T* ptr);

    const T& operator*() const;

    ConstIterator& operator++();

    bool operator!=(const ConstIterator& other) const;
};

// Array implementation

template<class T>
Array<T>::Array(size_t size) : m_size(size), m_data(new T[size]) {}

template<class T>
Array<T>::~Array() { delete[] m_data; }

template<class T>
Array<T>::Array(const Array& other) : m_size(other.m_size) {
    m_data = new T[m_size];
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] = other.m_data[i];
    }
}

template<class T>
Array<T>::Array(Array&& other) noexcept: m_size(other.m_size), m_data(other.m_data) {
    other.m_size = 0;
    other.m_data = nullptr;
}

template<class T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        delete[] m_data;
        m_size = other.m_size;
        m_data = new T[m_size];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }
    return *this;
}

template<class T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    if (this != &other) {
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
    }
    return *this;
}

template<class T>
T& Array<T>::operator[](size_t index) { return m_data[index]; }

template<class T>
const T& Array<T>::operator[](size_t index) const { return m_data[index]; }

// ConstIterator implementation

template<class T>
Array<T>::ConstIterator::ConstIterator(const T* ptr) : m_current(ptr) {}

template<class T>
const T& Array<T>::ConstIterator::operator*() const { return *m_current; }

template<class T>
typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator++() {
    ++m_current;
    return *this;
}

template<class T>
bool Array<T>::ConstIterator::operator!=(const ConstIterator& other) const {
    return m_current != other.m_current;
}

template<class T>
typename Array<T>::ConstIterator Array<T>::begin() const { return ConstIterator(m_data); }


template<class T>
typename Array<T>::ConstIterator Array<T>::end() const { return ConstIterator(m_data + m_size); }


#endif // ARRAY_H
