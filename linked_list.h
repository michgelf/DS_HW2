//
// Created by michg on 1/18/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_LINKED_LIST_H
#define DS_WET2_WINTER_2024_2025_LINKED_LIST_H

#include <memory>
#include <utility>
#include <stdexcept>

template<typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& data) : data(std::move(data)), next(nullptr) {}

        ~Node() { delete next; }
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    LinkedList() : head(nullptr), tail(nullptr) {};

    ~LinkedList() {
        delete head;
    }

    // Disable copy and assignment
    LinkedList(const LinkedList&) = delete;

    LinkedList& operator=(const LinkedList&) = delete;


    void emplace_back(const T& value) {
        Node* newNode = new Node(std::move(value));
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    bool empty() const {
        return tail == nullptr;
    }

    T& back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return tail->data;
    }

    struct Iterator;

    void erase(Iterator pos) {
        if (!head || !pos.current) {
            return;
        }

        // remove head
        if (pos.current == head) {
            Node* tmp = head;
            head = head->next;
            tmp->next = nullptr;
            delete tmp;
        }

        // Find the nodeBefore before the node to remove
        Node* nodeBefore = head;
        while (nodeBefore->next != nullptr && nodeBefore->next != pos.current) {
            nodeBefore = nodeBefore->next;
        }

        // Delete the node after nodeBefore only if found
        if (nodeBefore->next) {
            Node* tmp = nodeBefore->next;
            nodeBefore->next = tmp->next;
            tmp->next = nullptr;
            delete tmp;
        }

    }

    // Range-based for loop support
    struct Iterator {
        Node* current;

        explicit Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }

    Iterator end() { return Iterator(nullptr); }
};


#endif //DS_WET2_WINTER_2024_2025_LINKED_LIST_H
