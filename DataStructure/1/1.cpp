// Writer: 20181257 ChanHyeok Choi
/* 1. (30 points) Generalize the Heap data structure from a binary tree to a k-ary tree,
for an arbitrary k ≥ 2. Study the relative efficiencies (in terms of actual running
time) of the resulting data structure for various values of k, by inserting and
removing a large number of randomly generated keys into each data structure,
which should be included in your main() function. Your main() should also include
code for visualizing your heap data structures before and after the operations.
*/

// #include <iostream>
// #include "heap.h"

#include <iostream>
#include "vector.h"

using namespace std;

class k_heapNode {
private:
    int elem;
    int key;
    k_heapNode* parent;
    vector<k_heapNode>* children;

    friend class k_heap;
public:
    k_heapNode(int _k) {
        children = new vector<k_heapNode>(_k);
    }
    ~k_heapNode() {
        while(!children->empty())
            children->pop_back();
        delete children;
    }
    void insert(int e, int _key) { elem = e; key = _key; }
    // bool isEmpty() { return elem == NULL; }
    bool isExternal() { return children->empty(); }
    k_heapNode* me() { return this; }
};

class k_heap {
private:
    k_heapNode* root;
    k_heapNode* lastNode;
    int k;
    int size;
public:
    k_heap(int _k) : root(NULL), lastNode(root), k(_k), size(0) {}
    // ~k_heap();
    void insert(int e, int _key) {
        if (size == 0) {
            root = new k_heapNode(k);
            root->insert(e, _key);
            root->parent = NULL;
            lastNode = root;
            size++;
        } else {
            k_heapNode* newNode = FindInsertionNode(); // a next new node
            newNode->insert(e, _key);
            lastNode = newNode;
            size++;
            upHeap(); // Fine-tunning with heap-order and complete k_ary Tree properties
            // During upHeap, maybe the lastNode would be changed
        }
    }
    void removeMin() {
        if (size == 0) {
            throw "HeapEmpty";
        } else if (size == 1) {
            delete root;
        } else {
            lastNode->children = root->children;
            for (int i = 0; i < root->children->sizet(); i++) {
                root->children->at(i)->parent = lastNode;
            }
            root = lastNode;

            k_heapNode* tempParentNode = lastNode->parent;
            lastNode->parent = NULL;
            tempParentNode->children->pop_back();
            lastNode = FindLastNode(tempParentNode, 0); // reset the lastNode
            size--;
            downHeap(); // Fine-tunning with heap-order and complete k_ary Tree properties
        }
    }
    void upHeap() { // reset lastNode and restore the heap-order property, which takes O(logN) time
        k_heapNode* tempNode = lastNode;
        while(tempNode->key < tempNode->parent->key && tempNode->parent != NULL) { // contrast to condition of termination
            k_heapNode* gp = tempNode->parent->parent;
            k_heapNode* p = tempNode->parent;

            vector<k_heapNode>* tempChildren = tempNode->children;
            tempNode->children = p->children;
            p->children = tempChildren;
            for(int i = 0; i < tempNode->children->sizet(); i++) {
                if (tempNode->children->at(i) == tempNode) {
                    tempNode->children->replace(i, p);
                }
                tempNode->children->at(i)->parent = tempNode;
            }

            for(int i = 0; i < p->children->sizet(); i++) {
                p->children->at(i)->parent = p;
            }
            
            tempNode->parent = gp;
            if (gp == NULL) {
                root = tempNode;
                break;
            } else {
                for(int i = 0; i < gp->children->sizet(); i++) {
                    if (gp->children->at(i) == p) {
                        gp->children->replace(i, tempNode);
                        break;
                    }
                }
            }

            if(lastNode == tempNode) { // only once operation
                lastNode = p;
            }
        }
    }
    void downHeap() { // restore the heap-order property, which takes O(logN) time
        k_heapNode* tempNode = root;
        while(true) {
            if (tempNode->isExternal()) break;
            int tempKey = tempNode->key;
            k_heapNode* changeNode;
            for(int i = 0; i < tempNode->children->sizet(); i++) {
                if (tempKey > tempNode->children->at(i)->key) {
                    tempKey = tempNode->children->at(i)->key;
                    changeNode = tempNode->children->at(i);
                }
            }
            if (tempKey == tempNode->key) break; // condition of termination

            k_heapNode* p = tempNode->parent;
            vector<k_heapNode>* tempChildren = changeNode->children;
            changeNode->children = tempNode->children;
            tempNode->children = tempChildren;
            for(int i = 0; i < changeNode->children->sizet(); i++) {
                if (changeNode->children->at(i) == changeNode) {
                    changeNode->children->replace(i, tempNode);
                }
                changeNode->children->at(i)->parent = changeNode;
            }
            
            for(int i = 0; i < tempNode->children->sizet(); i++) {
                tempNode->children->at(i)->parent = tempNode;
            }

            changeNode->parent = p;
            if (p == NULL) {
                root = changeNode;
                break;
            } else {
                for(int i = 0; i < p->children->sizet(); i++) {
                    if (p->children->at(i) == tempNode) {
                        p->children->replace(i, changeNode);
                        break;
                    }
                }
            }

            if(lastNode == changeNode) { // only once operation
                lastNode = tempNode;
            }
        }
    }
    k_heapNode* FindInsertionNode() { // it will give where you insert new node, and connect each other
        k_heapNode* tempNode = lastNode->parent;
        k_heapNode* v = lastNode;
        if (tempNode == NULL) { // if root
            k_heapNode* newNode = new k_heapNode(k);
            root->children->push_back(newNode);
            newNode->parent = root;
            return newNode;
        }
        if (tempNode != NULL && (tempNode->children->sizet() != k && tempNode->children->checkin(v))) { // if lastNode is in the vector and nextNode is empty,
            k_heapNode* newNode = new k_heapNode(k);
            tempNode->children->push_back(newNode);
            newNode->parent = tempNode;
            return newNode;
        }

        while (tempNode != root) {
            v = tempNode;
            tempNode = tempNode->parent; // go up until tempNode is root,
        }
        for (int i = 0; i < tempNode->children->sizet(); i++) {
            if (i != k - 1 && tempNode->children->at(i) == v) {
                v = tempNode->children->at(i + 1);
                break;
            }
            else if (i == k - 1) {
                v = tempNode->children->front();
            }
        } // check whether v is rightmost or not

        while (v->children->front() != NULL) {
            tempNode = v;
            v = v->children->front();
        } // go down until v is NULL

        k_heapNode* newNode = new k_heapNode(k);
        v->children->push_back(newNode);
        newNode->parent = v;
        return newNode;
    }

    k_heapNode* FindLastNode(k_heapNode* v, int count) {
        if (v->children->sizet() > 0) {
            return v->children->back();
        }
        else {
            if (v == root) {
                return v;
            } else {
                k_heapNode* tempNode = v;
                k_heapNode* tempParent = v->parent;
                while (tempParent != root) {
                    tempNode = tempParent;
                    tempParent = tempParent->parent; // go up until tempParent is root,
                }
                for (int i = 0; i < tempParent->children->sizet(); i++) {
                    if (i == 0 && tempParent->children->at(i) == tempNode) {
                        tempNode = tempParent->children->at(k - 1);
                        break;
                    }
                    else if (tempParent->children->at(i) == tempNode) {
                        tempNode = tempParent->children->at(i - 1);
                        break;
                    }
                } // check whether v is leftmost or not
                while (tempNode->children->back() != NULL) {
                    tempParent = tempNode;
                    tempNode = tempNode->children->back();
                } // go down until v is NULL
                return tempNode;
            }
        }
    }

    int Depth(k_heapNode* v) {
        if (v == root) return 0;
        else return Depth(v->parent) + 1;
    }
    
    void DrawTree(k_heapNode* v) {
        DrawElemKey(v);
        if (v->isExternal()) {
            return;
        }

        for(int i = 0; i < v->children->sizet(); i++) {
            for(int j = 1; j <= Depth(v->children->at(i)); j++) {
                cout << "     ";
            }
            cout << "+--- ";
            DrawTree(v->children->at(i));
        }
    }

    void DrawElemKey(k_heapNode* v) {
        cout << "(" << v->elem << ", " << v->key << ")" << '\n';
    }

    k_heapNode* getRoot() { return root; }
};


int main() {
    k_heap binaryheap(3);

    // insert operation
    cout << "<insert operation>\n";
    binaryheap.insert(1, 3); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(2, 6); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(3, 9); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(4, 8); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(5, 5); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(6, 2); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(7, 11); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.insert(8, 13); binaryheap.DrawTree(binaryheap.getRoot());

    // removeMin operation
    cout << "\n<removeMin operation>\n";
    binaryheap.removeMin(); binaryheap.DrawTree(binaryheap.getRoot());
    binaryheap.removeMin(); binaryheap.DrawTree(binaryheap.getRoot());
    
    return 0;
}