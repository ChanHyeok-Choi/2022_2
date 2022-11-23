#ifndef _DEQUE_H_
#define _DEQUE_H_

template <typename T>
class deque_node {
private:
    T elem;
    deque_node<T>* prev;
    deque_node<T>* next;

    template <typename E> friend class deque;
};

template <typename T>
class deque {
private:
    deque_node<T>* head;
    deque_node<T>* tail;
    int n;
public:
    deque() : n(0) {
        head = new deque_node<T>;
        tail = new deque_node<T>;
        head->next = tail;
        tail->prev = head;
    }
    // ~deque() {
    //     while(!empty()) 
    //         pop_front();
    //     delete head;
    //     delete tail;
    // }
    int size() { return n; }
    bool empty() { return n == 0; }
    T front() { return head->next->elem; }
    T back() { return tail->prev->elem; }
    T at(int i) {
        int cnt = 0;
        deque_node<T>* target = head->next;
        while(cnt != i) {
            target = target->next;
            cnt++;
        }
        return target->elem;
    }
    void push_front(T e) {
        deque_node<T>* newNode = new deque_node<T>;
        deque_node<T>* tempNode = head->next;
        newNode->elem = e;
        head->next = newNode;
        newNode->prev = head;
        tempNode->prev = newNode;
        newNode->next = tempNode;
        n++;
    }
    void push_back(T e) {
        deque_node<T>* newNode = new deque_node<T>;
        deque_node<T>* tempNode = tail->prev;
        newNode->elem = e;
        newNode->next = tail;
        tail->prev = newNode;
        tempNode->next = newNode;
        newNode->prev = tempNode;
        n++;
    }
    void pop_front() {
        if(empty())
            throw "DequeEmpty";
        deque_node<T>* oldNode = head->next;
        oldNode->next->prev = head;
        head->next = oldNode->next;
        delete oldNode;
        n--;
    }
    void pop_back() {
        if(empty())
            throw "DequeEmpty";
        deque_node<T>* oldNode = tail->prev;
        oldNode->prev->next = tail;
        tail->prev = oldNode->prev;
        delete oldNode;
        n--;
    }
};

#endif