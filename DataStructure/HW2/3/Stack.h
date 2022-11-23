#ifndef _STACK_H_
#define _STACK_H_

#include <cstddef>

template <typename T>
struct Node {
    T elem;
    struct Node<T>* next;
    struct Node<T>* prev;
};

template <typename T>
class Stack {
private:
    Node<T>* head;
    Node<T>* tail;
    int n;
public:
    Stack() : head(NULL), tail(NULL), n(0) { 
        head = new Node<T>; tail = new Node<T>;
        head->next = tail;
        tail->prev = head;
    }
    ~Stack() { while(!empty()) pop_back(); }
    bool empty() { return n == 0; }
    int size() { return n; }
    T front() { return head->next->elem; }
    T back() { return tail->prev->elem; }
    void push_back(const T e);
    T pop_back();
    T pop_front();
};

template <typename T>
void Stack<T>::push_back(const T e) {
    Node<T>* newNode = new Node<T>;
    Node<T>* refNode = tail->prev;
    newNode->elem = e;
    newNode->next = tail;
    tail->prev = newNode;
    newNode->prev = refNode;
    refNode->next = newNode;
    n++;
}

template <typename T>
T Stack<T>::pop_back() {
    Node<T>* refNode = tail->prev;
    T elem = refNode->elem;
    tail->prev = refNode->prev;
    refNode->prev->next = tail;
    delete refNode;
    n--;
    return elem;
}

template <typename T>
T Stack<T>::pop_front() {
    Node<T>* refNode = head->next;
    T elem = refNode->elem;
    head->next = refNode->next;
    refNode->next->prev = head;
    delete refNode;
    n--;
    return elem;
}

#endif