#ifndef _DLL_H_
#define _DLL_H_

template <typename T>
struct DNode {
    T elem;
    struct DNode<T>* next;
    struct DNode<T>* prev;
};

template <typename T>
struct Position {
    struct DNode<T>* v;
    Position(DNode<T>* DN) { v = DN; }
};

template <typename T>
class DLinkedList {
private:
    DNode<T>* header;
    DNode<T>* trailer;
    Position<T>* cursor;
    int n;
public:
    DLinkedList() : n(0) {
        header = new DNode<T>;
        trailer = new DNode<T>;
        cursor = new Position<T>(header);
        header->next = trailer;
        trailer->prev = header;
    }
    ~DLinkedList() { while(!empty()) removeFront(); }
    bool empty() { return n == 0; }
    int size() { return n; }
    // const Position<T>& front() { return Position(header->next); }
    // const Position<T>& back() { return Position(trailer->prev); }
    
    void addFront(const T e);
    void addBack(const T e);
    void removeFront();
    void removeBack();

    const T getCursor() { return cursor->v->elem; }
    void left();
    void right();
    void Delete();
    void insert(const T e);

    void print();
};

#include <iostream>

template <typename T>
void DLinkedList<T>::addFront(const T e) {
    DNode<T>* newN = new DNode<T>;
    newN->elem = e;
    newN->next = header->next;
    header->next = newN;
    newN->prev = header;
    n++;

    cursor->v = newN;
}

template <typename T>
void DLinkedList<T>::addBack(const T e) {
    DNode<T>* newN = new DNode<T>;
    newN->elem = e;
    newN->next = trailer;
    trailer->prev = newN;
    newN->prev = trailer->prev;
    n++;
    
    cursor->v = newN;
}

template <typename T>
void DLinkedList<T>::removeFront() {
    DNode<T>* ref = header->next;
    header->next = ref->next;
    ref->prev = header;
    delete ref;
    n--;

    cursor->v = header->next;
}

template <typename T>
void DLinkedList<T>::removeBack() {
    DNode<T>* ref = trailer->prev;
    trailer->prev = ref->prev;
    ref->next = trailer;
    delete ref;
    n--;

    cursor->v = trailer->prev;
}

template <typename T>
void DLinkedList<T>::left() {
    if (cursor->v->prev == header)
        return;
    else
        cursor->v = cursor->v->prev;
}

template <typename T>
void DLinkedList<T>::right() {
    if (cursor->v->next == trailer)
        return;
    else
        cursor->v = cursor->v->next;
}

template <typename T>
void DLinkedList<T>::Delete() {
    if (cursor->v->next == trailer)
        return;
    else {
        DNode<T>* ref = cursor->v->next;
        DNode<T>* next = ref->next;
        DNode<T>* prev = ref->prev;
        next->prev = prev;
        prev->next = next;
        delete ref;
        n--;
    }
}

template <typename T>
void DLinkedList<T>::insert(const T e) {
    DNode<T>* ref = cursor->v;
    DNode<T>* newN = new DNode<T>;
    newN->elem = e;
    newN->next = ref->next;
    ref->next->prev = newN;
    newN->prev = ref;
    ref->next = newN;
    n++;

    cursor->v = newN;
}

template <typename T>
void DLinkedList<T>::print() {
    // DNode<T>* i = new DNode<T>;

    for(DNode<T>* i = header->next; i != trailer; i = i->next)
        std::cout << i->elem;
    std::cout << '\n';
    // delete i;
}

#endif