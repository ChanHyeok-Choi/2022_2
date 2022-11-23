#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

using namespace std;

#include <iostream>

template <typename T>
class SNode {
private:
    T elem;
    SNode* next;

    template <typename E> friend class SLinkedList;
};

template <typename T>
class SLinkedList {
private:
    SNode<T>* head;
public:
    SLinkedList() { head = NULL; }
    ~SLinkedList() { while(!empty()) removeFront(); }
    bool empty() { return head == NULL; }
    const T front() const { return head->elem; }
    void addFront(const T e);
    void removeFront();
};

template <typename T>
void SLinkedList<T>::addFront(const T e) {
    SNode<T>* newNode = new SNode<T>;
    newNode->elem = e;
    newNode->next = head;
    head = newNode;
}

template <typename T>
void SLinkedList<T>::removeFront() {
    try {
        if (empty())
            throw "Error: you attempted to remove a front element of the empty singly linked list.";
        SNode<T>* old = head;
        head = head->next;
        delete old;
    } 
    catch(const char * err) {
        std::cout << "Warning!" << '\n';
        std::cout << err << '\n';
    }
}

#endif