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
    ~deque() {
        while(!empty()) 
            pop_front();
        delete head;
        delete tail;
    }
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
        newNode->elem = e;
        deque_node<T>* tempNode = head->next;
        head->next = newNode;
        newNode->prev = head;
        tempNode->prev = newNode;
        newNode->next = tempNode;
    }
    void push_back(T e) {
        deque_node<T>* newNode = new deque_node<T>;
        newNode->elem = e;
        deque_node<T>* tempNode = tail->prev;
        tail->prev = newNode;
        newNode->next = tail;
        tempNode->next = newNode;
        newNode->prev = tempNode;
    }
    T pop_front() {
        deque_node<T>* oldNode = head->next;
        T pop = oldNode->elem;
        head->next = oldNode->next;
        oldNode->next->prev = head;
        delete oldNode;
        return pop;
    }
    T pop_back() {
        deque_node<T>* oldNode = tail->prev;
        T pop = oldNode->elem;
        tail->prev = oldNode->prev;
        oldNode->prev->next = tail;
        delete oldNode;
        return pop;
    }
};

#endif