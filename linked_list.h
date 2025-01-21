#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template<typename T>
class LinkedList {
private:

    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* next_node = nullptr) : data(value), next(next_node) {}
    };

    Node* m_head;
    Node* m_tail;

    void clear();

    void removeHead();

public:

    LinkedList();

    virtual ~LinkedList();

    LinkedList(const LinkedList& other);

    LinkedList(LinkedList&& other) noexcept;

    LinkedList& operator=(const LinkedList& other);

    LinkedList& operator=(LinkedList&& other) noexcept;

    class Iterator;

    class ConstIterator;

    bool empty() const;

    T& back();

    const T& back() const;

    // Emplace back
    void append(const T& value);

    void remove(Iterator it);

    Iterator begin();

    Iterator end();

    ConstIterator begin() const;

    ConstIterator end() const;

};

template<class T>
class LinkedList<T>::Iterator {
    Node* m_current;

    friend class LinkedList<T>;

public:
    explicit Iterator(Node* node);

    T& operator*();

    T* operator->();

    Iterator& operator++();

    bool operator!=(const Iterator& other) const;
};

template<class T>
class LinkedList<T>::ConstIterator {
    const Node* m_current;

    friend class LinkedList<T>;


public:
    explicit ConstIterator(const Node* node);

    const T& operator*() const;

    const T* operator->() const;

    ConstIterator& operator++();

    bool operator!=(const ConstIterator& other) const;
};


// LinkedList implementation

template<class T>
LinkedList<T>::LinkedList() : m_head(nullptr), m_tail(nullptr) {}

template<class T>
LinkedList<T>::~LinkedList() { clear(); }


template<class T>
LinkedList<T>::LinkedList(const LinkedList& other) : m_head(nullptr), m_tail(nullptr) {
    for (const auto& item: other) {
        append(item);
    }
}

template<class T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept: m_head(other.m_head), m_tail(other.m_tail) {
    other.m_head = other.m_tail = nullptr;
}


template<class T>
void LinkedList<T>::clear() {
    while (m_head) {
        Node* temp = m_head;
        m_head = m_head->next;
        delete temp;
    }
    m_tail = nullptr;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this != &other) {
        clear();
        for (const auto& item: other) {
            append(item);
        }
    }
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) noexcept {
    if (this != &other) {
        clear();
        m_head = other.m_head;
        m_tail = other.m_tail;
        other.m_head = other.m_tail = nullptr;
    }
    return *this;
}

template<class T>
bool LinkedList<T>::empty() const {
    return m_head == nullptr;
}

template<class T>
void LinkedList<T>::append(const T& value) {
    Node* new_node = new Node(value);
    if (m_tail) {
        m_tail->next = new_node;
    } else {
        m_head = new_node;
    }
    m_tail = new_node;
}

template<class T>
void LinkedList<T>::removeHead() {
    if (m_head) {
        Node* temp = m_head;
        m_head = m_head->next;
        delete temp;
        if (!m_head) {
            m_tail = nullptr;
        }
    }
}

template<class T>
T& LinkedList<T>::back() {
    return m_tail->data;
}

template<class T>
const T& LinkedList<T>::back() const {
    return m_tail->data;
}

template<class T>
void LinkedList<T>::remove(Iterator it) {
    if (!m_head || !it.m_current) {
        return;
    }

    if (it.m_current == m_head) {
        removeHead();
        return;
    }

    // find the node before the node to be deleted
    Node* prev = m_head;
    while (prev->next && prev->next != it.m_current) {
        prev = prev->next;
    }

    if (prev->next) {
        Node* temp = prev->next;
        prev->next = temp->next;
        if (temp == m_tail) {
            m_tail = prev;
        }
        delete temp;
    }
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() { return Iterator(m_head); }

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::end() { return Iterator(nullptr); }

template<class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::begin() const { return ConstIterator(m_head); }

template<class T>
typename LinkedList<T>::ConstIterator LinkedList<T>::end() const { return ConstIterator(nullptr); }


// Iterator implementation

template<class T>
LinkedList<T>::Iterator::Iterator(Node* node) : m_current(node) {}

template<class T>
T& LinkedList<T>::Iterator::operator*() { return m_current->data; }

template<class T>
T* LinkedList<T>::Iterator::operator->() { return &m_current->data; }


template<class T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    m_current = m_current->next;
    return *this;
}

template<class T>
bool LinkedList<T>::Iterator::operator!=(const Iterator& other) const {
    return m_current != other.m_current;
}


// ConstIterator implementation

template<class T>
LinkedList<T>::ConstIterator::ConstIterator(const Node* node) : m_current(node) {}

template<class T>
const T& LinkedList<T>::ConstIterator::operator*() const { return m_current->data; }

template<class T>
const T* LinkedList<T>::ConstIterator::operator->() const { return &m_current->data; }

template<class T>
typename LinkedList<T>::ConstIterator& LinkedList<T>::ConstIterator::operator++() {
    m_current = m_current->next;
    return *this;
}

template<class T>
bool LinkedList<T>::ConstIterator::operator!=(const ConstIterator& other) const {
    return m_current != other.m_current;
}

#endif // LINKED_LIST_H
