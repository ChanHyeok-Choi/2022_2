// Writer: 20181257 Chanhyeok Choi
/* 4. (bonus, 30 points) Write a C++ class that implements all the functions of the
ordered map ADT using a red-black tree. Your main() should also include code for
visualizing your data structures before and after the operations.
*/

#include <iostream>
#include <exception>
#include <string>

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

enum COLOR { RED, BLACK };

class RBNode {
private:
    COLOR color;
    int key;
    int elem;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
public:
    bool isExternal() { return left == NULL && right == NULL; }
    void Color() {
        if(color == COLOR::BLACK) cout << "B";
        else cout << "R";
    }
    void Clean() {
        left->left = NULL;
        left->right = NULL;
        right->left = NULL;
        right->right = NULL;
    }
    friend class RBTree;
};

class RBTree {
private:
    RBNode* root;
    int n;
public:
    RBTree() : n(0), root(NULL) {}
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
        newNode->color = COLOR::RED;
        newNode->parent = NULL;
        newNode->left = new RBNode; newNode->left->parent = newNode; newNode->left->color = COLOR::BLACK;
        newNode->right = new RBNode; newNode->right->parent = newNode; newNode->right->color = COLOR::BLACK;
        n++;
        return newNode;
    }
    RBNode* Restructure(RBNode* z) {
        // There are four cases
        RBNode* pNode = z->parent;
        RBNode* gpNode = z->parent->parent;
        if(z->parent->left == z) {
            if(z->parent->parent->left == z->parent) {
                RBNode* sibling = getSibling(z);
                pNode->color = COLOR::BLACK;
                gpNode->color = COLOR::RED; 
                if(gpNode->parent != NULL) { // gpNode is not root
                    if(gpNode->parent->left == gpNode) {
                        gpNode->parent->left = pNode;
                    } else { // gpNode->parent->right == gpNode
                        gpNode->parent->right = pNode;
                    }
                }
                gpNode->left = sibling; sibling->parent = gpNode;
                pNode->right = gpNode;
                pNode->parent = gpNode->parent;
                gpNode->parent = pNode;
                return pNode;
            } else { // z->parent->parent->right == z->parent
                RBNode* leftChild = z->left;
                RBNode* rightChlid = z->right;
                z->color = COLOR::BLACK;
                gpNode->color = COLOR::RED;
                z->parent = gpNode->parent; 
                if(gpNode->parent != NULL) { // gpNode is not root
                    if(gpNode->parent->left == gpNode) {
                        gpNode->parent->left = z;
                    } else { // gpNode->parent->right == gpNode
                        gpNode->parent->right = z;
                    }
                }
                z->left = gpNode; gpNode->parent = z;
                z->right = pNode; pNode->parent = z;
                gpNode->right = leftChild; leftChild->parent = gpNode;
                pNode->left = rightChlid; rightChlid->parent = pNode;
                return z;
            }
        } else { // z->parent->right == z
            if(z->parent->parent->left == z->parent) {
                RBNode* leftChild = z->left;
                RBNode* rightChlid = z->right;
                z->color = COLOR::BLACK;
                gpNode->color = COLOR::RED;
                z->parent = gpNode->parent;
                if(gpNode->parent != NULL) { // gpNode is not root
                    if(gpNode->parent->left == gpNode) {
                        gpNode->parent->left = z;
                    } else { // gpNode->parent->right == gpNode
                        gpNode->parent->right = z;
                    }
                }
                z->right = gpNode; gpNode->parent = z;
                z->left = pNode; pNode->parent = z;
                gpNode->left = rightChlid; rightChlid->parent = gpNode;
                pNode->right = leftChild; leftChild->parent = pNode;
                return z;           
            } else { // z->parent->parent->right == z->parent
                RBNode* sibling = getSibling(z);
                pNode->color = COLOR::BLACK;
                gpNode->color = COLOR::RED;
                if(gpNode->parent != NULL) { // gpNode is not root
                    if(gpNode->parent->left == gpNode) {
                        gpNode->parent->left = pNode;
                    } else { // gpNode->parent->right == gpNode
                        gpNode->parent->right = pNode;
                    }
                }
                gpNode->right = sibling; sibling->parent = gpNode;
                pNode->left = gpNode;
                pNode->parent = gpNode->parent;
                gpNode->parent = pNode;
                return pNode;
            }            
        }
    }
    RBNode* InsertionRecolor(RBNode* z) {
        RBNode* v = z->parent;
        RBNode* w = getSibling(v);
        v->color = COLOR::BLACK;
        w->color = COLOR::BLACK;
        if(v->parent != root)
            v->parent->color = COLOR::RED;
        return v->parent;
    }
    void RemedyDoubleRed(RBNode* z) {
        while(z->parent->color == COLOR::RED && z->color == COLOR::RED) { // double red
            RBNode* v = z->parent;
            RBNode* w = getSibling(v);
            if(w->color == COLOR::BLACK) { // sibling color is black
                z = Restructure(z);
                if(z->parent == NULL)
                    root = z;
                return;
            } else { // sibling color is red
                z = InsertionRecolor(z);
                if(z == root)
                    return;
            }
        }
    }
    RBNode* insert(int k, int e, RBNode* v) {
        // step 1. Search for key k to locate the insertion node z
        // step 2. Add the new entry (k, o) at node z and color z red 
        // step 3. Remedy a Double-Red
        if(n == 0) {
            RBNode* newNode = MakeNewNode(k, e);
            root = newNode;
            root->color = COLOR::BLACK;
            return newNode;
        }
        if(v->key > k && v->isExternal() != true) {
            return insert(k, e, v->left);
        } else if(v->key <= k && v->isExternal() != true) {
            return insert(k, e, v->right);
        } else if(v->parent->left == v && v->isExternal() == true) {
            RBNode* newNode = MakeNewNode(k, e);
            newNode->parent = v->parent;
            v->parent->left = newNode;
            newNode->Clean();
            RemedyDoubleRed(newNode);
            return newNode;
        } else { // v->parent->right == v && v->isExternal() == true
            RBNode* newNode = MakeNewNode(k, e);
            newNode->parent = v->parent;
            v->parent->right = newNode;
            newNode->Clean();
            RemedyDoubleRed(newNode);
            return newNode;
        }
    }
    RBNode* DeletionRestructure(RBNode* r) {
        // RBNode* x = r->parent;
        RBNode* y = getSibling(r);
        if(y->left->color == COLOR::RED) {
            RBNode* z = y->left;
            y = Restructure(z);
            y->color = COLOR::RED; y->left->color = COLOR::BLACK; y->right->color = COLOR::BLACK;
            return y;
        } else { // r->right->color == COLOR::RED
            RBNode* z = y->right;
            y = Restructure(z);
            y->color = COLOR::RED; y->left->color = COLOR::BLACK; y->right->color = COLOR::BLACK;
            return y;
        }
    }
    RBNode* DeletionRecolor(RBNode* r) {
        // case 2-1. x (r's parent) is red
        // case 2-2. x (r's parent) is black
        RBNode* x = r->parent;
        RBNode* y = getSibling(r);
        if(x->color == COLOR::RED) { // case 2-1.
            x->color = COLOR::BLACK;
            y->color = COLOR::RED;
        } else { // x->color == COLOR::BLACK // case 2-2.
            y->color = COLOR::RED;
        }
        return x;
    }
    RBNode* DeletionAdjustment(RBNode* r) {
        // If y is the right child of x, then let z be the right child of y
        // If y is the left child of x, then let z be the left child of y
        // case 3-1: z is the left child of y
        // case 3-2: z is the right child of y
        RBNode* x = r->parent;
        RBNode* y = getSibling(r);
        if(x->left == y) { // case 3-2
            RBNode* z = y->left;
            y = Restructure(z);       
        } else { // case 3-1
            RBNode* z = y->right;
            y = Restructure(z);
        }
        if(x == root)
            root = y;
        y->color = COLOR::BLACK; y->left->color = COLOR::BLACK; y->right->color = COLOR::RED;
        return r;
    }
    void RemedyDoubleBlack(RBNode* r) {
        // case 1: The sibling y of r is black, and has a red child z
        // case 2: The sibling y of r is black, and y’s both children are black
        // case 3: The sibling y of r is red
        RBNode* y = getSibling(r);
        while(r != root && r->color == COLOR::BLACK) {
            if(y->color == COLOR::BLACK && (y->left->color == COLOR::RED || y->right->color == COLOR::RED)) { // case 1
                r = DeletionRestructure(r);
                return;
            } else if(y->color == COLOR::BLACK && (y->left->color == COLOR::BLACK && y->right->color == COLOR::BLACK)) { // case 2
                r = DeletionRecolor(r);
                if(r->parent == root)
                    return;
            } else { // y->color == COLOR::RED // case 3
                r = DeletionAdjustment(r);
            }
            y = getSibling(r);
        }
        return;
    }
    void remove(int k, RBNode* v) {
        // step 1. Find the deletion node of k by executing binary tree algorithm
        // step 2. Remedy Double-Black
        if(n == 0) {
            cout << "EmptyRBTree!" << '\n';
            return;
        }
        if(v->key > k && v->isExternal() != true) {
            remove(k, v->left);
        } else if(v->key < k && v->isExternal() != true) {
            remove(k, v->right);
        } else if(v->key == k) {
            if(n == 1) {
                delete root;
                return;
            }
            RBNode* tempNode;
            if (!(v->left->isExternal()) && !(v->right->isExternal())){ // when node have 2 children
                tempNode = v->right;
                while(!(tempNode->left->isExternal())) {
                    tempNode = tempNode->left;
                } // set inorder successor
            }
            else if (v->left->isExternal() && v->right->isExternal()) // when leaf
                tempNode = v;
            else if (!(v->left->isExternal())) // when single child
                tempNode = v->left;
            else // !(v->right->isExternal())
                tempNode = v->right;
            v->key = tempNode->key; v->elem = tempNode->elem; // copy inorder successor
            // Let v be the internal node removed, w the external node removed, and r the sibling of w
            v = tempNode; RBNode* w = (v->left->isExternal() ? v->left : v->right); RBNode* r = getSibling(w);
            // case 0: If either v or r was red, we color r black and we are done
            // case 1~3: If v and r were both BLACK, w color r double-black
            if(v->color == COLOR::RED || r->color == COLOR::RED) { // case 0
                if(v->parent->right == v) {
                    v->parent->right = r;
                } else { // v->parent->left == v
                    v->parent->left = r;
                }
                r->parent = v->parent;
                r->color = COLOR::BLACK;
                delete v; n--;
                return;
            } else { // v->color == COLOR::BLACK && r->color == COLOR::BLACK // case 1~3
                if(v->parent->right == v) {
                    v->parent->right = r;
                } else { // v->parent->left == v
                    v->parent->left = r;
                }
                r->parent = v->parent;
                delete v; n--;
                // Regard x as "double black nodes"
                RemedyDoubleBlack(r);
                return;
            }
        } else {
            cout << "There is no entry having such key" << '\n';
            return;
        }
    }
    void Show(RBNode* v, string blank, bool rightOrleft) { // show the RB Tree
        if (v != NULL) {
            cout << blank; // print blank by depth of v
            if (rightOrleft) {
                cout << "R--->";
                blank += "     "; // add blank by depth of v
            } else {
                cout << "L--->";
                blank += "|    "; // bridge with right node
            }
            // cout << v->key << " (" << max(getHeight(v->left), getHeight(v->right)) + 1 << ")" << '\n';
            cout << "(" << v->key << ", " << v->elem << ") " << " ("; v->Color(); cout << ")" << '\n';
            Show(v->left, blank, false); // first, go left node
            Show(v->right, blank, true); // and then, go right node
        }
    }
};

void printMenu() {
    cout << "<---------Menu--------->\n";
    cout << "1. Print Red-Black Tree\n";
    cout << "2. Insert (key, element)\n";
    cout << "3. Remove (key)\n";
    cout << "4. Exit\n";
    cout << "Input your choice: ";
}

int main() {
    RBTree rb;

    int choice;
    while(true) {
        printMenu(); cin >> choice;
        int key, element;
        switch(choice) {
            case 1:
                rb.Show(rb.getRoot(), "", true);
                break;
            case 2:
                cout << "Input your \'key element\' pair for insertion: "; cin >> key >> element;
                rb.insert(key, element, rb.getRoot());
                break;
            case 3:
                cout << "Input your \'key\' for removal: "; cin >> key;
                rb.remove(key, rb.getRoot());
                break;
            default: 
                cout << "Exit Program\n";
                return 0;
        }
    }

    return 0;
}