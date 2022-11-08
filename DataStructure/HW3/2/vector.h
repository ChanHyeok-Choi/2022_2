#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstddef>

template <typename T>
class VectorNode {
private:
    T* elem;
    VectorNode<T>* next;
    VectorNode<T>* prev;

    template <typename E> friend class vector;
};

template <typename T>
class vector {
private:
    VectorNode<T>* head;
    VectorNode<T>* tail;
    int k;
    int size;
public:
    vector(int _k) : k(_k), size(0) {
        head = new VectorNode<T>;
        tail = new VectorNode<T>;
        head->next = tail; tail->prev = head;
    }
    ~vector() {
        while (!empty())
            pop_back();
        delete head;
        delete tail;
    }
    int sizet() { return size; }
    bool empty() { return size == 0; }
    void push_back(T* e) {
        if (k == size) {
            throw "VectorFull";
        }
        VectorNode<T>* newNode = new VectorNode<T>;
        VectorNode<T>* temp = tail->prev;
        newNode->elem = e;
        newNode->next = tail;
        tail->prev = newNode;
        temp->next = newNode;
        newNode->prev = temp;
        size++;
    }
    void pop_back() {
        if (size == 0) {
            throw "VectorEmpty";
        }
        VectorNode<T>* oldNode = tail->prev;
        oldNode->prev->next = tail;
        tail->prev = oldNode->prev;
        
        delete oldNode;
        size--;
    }
    T* at(int idx) {
        VectorNode<T>* temp = head->next;
        for(int i = 0; i < size; i++) {
            if (i == idx) {
                return temp->elem;
            }
            temp = temp->next;
        }
        return temp->elem;
    }
    void replace(int idx, T* v) {
        VectorNode<T>* temp = head->next;
        for(int i = 0; i < size; i++) {
            if (i == idx) {
                temp->elem = v;
                break;
            }
            temp = temp->next;
        }
    }
    T* back() { return tail->prev->elem; }
    T* front() { return head->next->elem; }
    bool checkin(T* v) {
        for (VectorNode<T>* i = head->next; i != tail; i = i->next) {
            if (i->elem == v)
                return true;
        }
        return false;
    }
};

#endif