//
// Created by michg on 1/19/2025.
//

#ifndef C_SCRIPTS_LINKED_LIST_H
#define C_SCRIPTS_LINKED_LIST_H


#include <iostream>
#include <utility>

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* next_node = nullptr) : data(value), next(next_node) {}

        Node(T&& value, Node* next_node = nullptr) : data(std::move(value)), next(next_node) {}
    };

    Node* head;
    Node* tail;

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

public:
    // Nested iterator class
    class Iterator {
        Node* current;

        friend class LinkedList<T>;

    public:
        explicit Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }

        T* operator->() { return &current->data; }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }

        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    // Const iterator
    class ConstIterator {
    private:
        const Node* current;

        friend class LinkedList<T>;


    public:
        explicit ConstIterator(const Node* node) : current(node) {}

        const T& operator*() const { return current->data; }

        const T* operator->() const { return &current->data; }

        ConstIterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator==(const ConstIterator& other) const { return current == other.current; }

        bool operator!=(const ConstIterator& other) const { return !(*this == other); }
    };

    // Default constructor
    LinkedList() : head(nullptr), tail(nullptr) {}

    // Destructor
    virtual ~LinkedList() { clear(); }

    // Copy constructor
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
        for (const auto& item: other) {
            emplace_back(item);
        }
    }

    // TODO
    // Move constructor
    LinkedList(LinkedList&& other) noexcept: head(other.head), tail(other.tail) {
        other.head = other.tail = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }

    // Copy assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            for (const auto& item: other) {
                emplace_back(item);
            }
        }
        return *this;
    }

    // TODO
    // Move assignment operator
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            other.head = other.tail = nullptr;
        }
        return *this;
    }

    // Emplace back
    void emplace_back(const T& value) {
        Node* new_node = new Node(value);
        if (tail) {
            tail->next = new_node;
        } else {
            head = new_node;
        }
        tail = new_node;
    }

    // Back

    T& back() {
        return tail->data;
    }

    // Erase
    void erase(Iterator pos) {
        if (!head || !pos.current) {
            return;
        }

        if (pos.current == head) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (!head) {
                tail = nullptr;
            }
            return;
        }

        Node* prev = head;
        while (prev->next && prev->next != pos.current) {
            prev = prev->next;
        }

        if (prev->next) {
            Node* temp = prev->next;
            prev->next = temp->next;
            if (temp == tail) {
                tail = prev;
            }
            delete temp;
        }
    }

    // Iterators
    Iterator begin() { return Iterator(head); }

    Iterator end() { return Iterator(nullptr); }

    ConstIterator begin() const { return ConstIterator(head); }

    ConstIterator end() const { return ConstIterator(nullptr); }

};


#endif //C_SCRIPTS_LINKED_LIST_H
