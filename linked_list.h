//
// Created by michg on 1/19/2025.
//

#ifndef DS_WET2_WINTER_2024_2025_LINKED_LIST_H
#define DS_WET2_WINTER_2024_2025_LINKED_LIST_H


#include <memory>
#include <utility>

// Simplified templated list
template<typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        Node(T&& val, Node* nextNode = nullptr)
                : value(std::move(val)), next(nextNode) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    LinkedList() = default;

    ~LinkedList() {
        clear();
    }

    // Disable copy
    LinkedList(const LinkedList&) = delete;

    LinkedList& operator=(const LinkedList&) = delete;

    // Enable move
    LinkedList(LinkedList&& other) noexcept
            : head(other.head), tail(other.tail) {
        other.head = nullptr;
        other.tail = nullptr;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    void emplace_back(T&& value) {
        Node* newNode = new Node(std::forward<T>(value));
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    T& back() {
        if (!tail) {
            throw std::runtime_error("List is empty");
        }
        return tail->value;
    }


    struct Iterator;

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

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    bool empty() const {
        return head == nullptr;
    }

    // Range-based for loop support
    struct Iterator {
        Node* current;

        explicit Iterator(Node* node) : current(node) {}

        T& operator*() { return current->value; }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }

    Iterator end() { return Iterator(nullptr); }

    // Const versions for const objects
    Iterator begin() const { return Iterator(head); }

    Iterator end() const { return Iterator(nullptr); }

};


#endif //DS_WET2_WINTER_2024_2025_LINKED_LIST_H
