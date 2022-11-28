// Writer: 20181257 Chanhyeok Choi
/* 4. (bonus, 30 points) Write a C++ class that implements all the functions of the
ordered map ADT using a red-black tree. Your main() should also include code for
visualizing your data structures before and after the operations.
*/

#include <iostream>

using namespace std;

/* Red-Black Tree: O(logN)
    Properties:
        1. Root Property: the root is black
        2. External Property: every leaf is black
        3. Internal Property: the children of a red node are black
        4. Depth Property: all the leaves have the same black depth

    Insertion
        1. Remedying a Double Red for child z and parent v, siling w of v
            case 1. w is black -> Restructuring
            case 2. w is red -> Recoloring
*/

class RBNode {
private:
    bool color; // true: red, false: black
    int key;
    int elem;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
public:
    bool isExternal() { return left == NULL && right == NULL; }
    friend class RBTree;
};

class RBTree {
private:
    RBNode* root;
    int n;
public:
    RBTree(int k, int e) : n(0) {
        root = new RBNode;
        root->color = false;
        root->key = k;
        root->elem = e;
        root->parent = NULL; root->left = NULL; root->right = NULL;
        n++;
    }
    // ~RBTree();
    int size() { return n; }
    bool isEmpty() { return n == 0; }
    RBNode* getRoot() { return root; }
    RBNode* getSibling(RBNode* z) {
        if(z->parent->left == z)
            return z->parent->right;
        else
            return z->parent->left;
    }
    RBNode* MakeNewNode(int k, int e) {
        RBNode* newNode = new RBNode;
        newNode->key = k;
        newNode->elem = e;
        newNode->color = true; // red
        newNode->parent = NULL; newNode->left = NULL; newNode->right = NULL;
        n++;
        return newNode;
    }
    RBNode* Restructure(RBNode* z) {

    }
    RBNode* Recolor(RBNode* z) {
        
    }
    void RemedyDoubleRed(RBNode* z) {
        while(z->parent->color == z->color) { // double red
            RBNode* w = getSibling(z);
            if(w->color == false) { // sibling color is black
                z = Restructure(z);
                return;
            } else { // sibling color is red
                z = Recolor(z);
            }
        }
    }
    RBNode* insert(int k, int e, RBNode* v) {
        // step 1. Search for key k to locate the insertion node z
        // step 2. Add the new entry (k, o) at node z and color z red 
        // step 3. Remedy a Double-Red
        if(v->key > k && v->isExternal() != true) {
            return insert(k, e, v->left);
        } else if(v->key <= k && v->isExternal() != true) {
            return insert(k, e, v->right);
        } else if(v->key > k && v->isExternal() == true) {
            RBNode* newNode = MakeNewNode(k, e);
            v->left = newNode; newNode->parent = v;
            RemedyDoubleRed(newNode);
            return newNode;
        } else { // v->key <= k && v->isExternal() == true
            RBNode* newNode = MakeNewNode(k, e);
            v->right = newNode; newNode->parent = v;
            RemedyDoubleRed(newNode);
            return newNode;
        }
    }
    RBNode* remove(int k, int e, RBNode* v);
    void DrawTree();
};

void printMenu() {
    cout << "<---------Menu--------->\n";
    cout << "1. Print Red-Black Tree\n";
    cout << "2. Insert (key, element)\n";
    cout << "3. Remove (key, element)\n";
    cout << "4. Exit\n";
    cout << "Input your choice: ";
}

int main() {
    RBTree rb(0, 0);

    int choice;
    while(true) {
        printMenu(); cin >> choice;
        switch(choice) {
            case 1:
                rb.DrawTree();
                break;
            case 2:
                int key, element;
                cout << "Input your \'key, element\' pair for insertion: "; cin >> key >> element;
                rb.insert(key, element, rb.getRoot());
                break;
            case 3:
                int key, element;
                cout << "Input your \'key, element\' pair for removal: "; cin >> key >> element;
                rb.remove(key, element, rb.getRoot());
                break;
            default: exit(0);
        }
    }

    return 0;
}