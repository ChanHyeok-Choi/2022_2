#include <iostream>
#include <string>
#include "LL.h"

// Writer: 20181257 ChanHyeok Choi
/* Implement the stack ADT in a fully generic manner (through the use
of templates) by means of a singly linked list. (Give your implementation “from
scratch,” without the use of any classes from the Standard Template Library or any
other data structures presented in this course.) */

using namespace std;


int main() {
    cout << "1. Create a singly linked list of an int type, named as intSLL" << '\n';
    SLinkedList<int> intSLL;
    cout << "intSLL.empty(): "<< intSLL.empty() << '\n';
    cout << "intSLL.addFront(3) and intSLL.front(): "; intSLL.addFront(3);
    cout << intSLL.front() << '\n';
    cout << "intSLL.addFront(5) and intSLL.front(): "; intSLL.addFront(5);
    cout << intSLL.front() << '\n';
    cout << "intSLL.removeFront() and intSLL.front(): "; intSLL.removeFront();
    cout << intSLL.front() << '\n';
    cout << "intSLL.removeFront()"; intSLL.removeFront(); cout << '\n';

    cout << "---------------------------------------------------------------------\n";

    cout << "2. Create a singly linked list of an string type, named as strSLL" << '\n';
    SLinkedList<string> strSLL;
    cout << "intSLL.empty(): "<< strSLL.empty() << '\n';
    cout << "intSLL.addFront(Data Structures) and intSLL.front(): "; strSLL.addFront("Data Structures");
    cout << strSLL.front() << '\n';
    cout << "intSLL.addFront(I'm all mighty) and intSLL.front(): "; strSLL.addFront("I'm all mighty");
    cout << strSLL.front() << '\n';
    cout << "intSLL.removeFront() and intSLL.front(): "; strSLL.removeFront();
    cout << strSLL.front() << '\n';
    cout << "intSLL.removeFront()"; strSLL.removeFront(); cout << '\n';
    cout << "intSLL.removeFront()"; strSLL.removeFront();

    return 0;
}