#ifndef CUSTOM_LIST_H
#define CUSTOM_LIST_H

//doubly linked list from lab
template <typename T>
struct ListNode {
    T data;
    ListNode* prev;
    ListNode* next;
};

//custom list used for graph adj lists
template <typename T>
class CustomList {
public:
    ListNode<T>* head;
    ListNode<T>* tail;
    int size;

    CustomList() : head(nullptr), tail(nullptr), size(0) {}

    //push at end
    void append(T val) {
        ListNode<T>* node = new ListNode<T>{val, tail, nullptr};
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = node;
        size++;
    }

    //delete from end
    void removeLast() {
        if (!tail) return;
        ListNode<T>* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        size--;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    ListNode<T>* getFirst() const {
        return head;
    }

    ListNode<T>* getLast() const {
        return tail;
    }

    ~CustomList() {
        while (head) {
            ListNode<T>* next = head->next;
            delete head;
            head = next;
        }
    }
};

#endif
