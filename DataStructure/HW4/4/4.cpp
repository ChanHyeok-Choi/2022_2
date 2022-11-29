// Writer: 20181257 Chanhyeok Choi
/* 4. (bonus, 30 points) Write a C++ class that implements all the functions of the
ordered map ADT using a red-black tree. Your main() should also include code for
visualizing your data structures before and after the operations.
*/

#include <iostream>
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
    RBNode* Recolor(RBNode* z) {
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
                z = Recolor(z);
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
            n++;
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
            RemedyDoubleRed(newNode);
            n++;
            return newNode;
        } else { // v->parent->right == v && v->isExternal() == true
            RBNode* newNode = MakeNewNode(k, e);
            newNode->parent = v->parent;
            v->parent->right = newNode;
            RemedyDoubleRed(newNode);
            n++;
            return newNode;
        }
    }
    RBNode* remove(int k, int e, RBNode* v);
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
            try {
                if(v) // Wrong address
                    throw "Wrong v address";
                cout << "(" << v->key << ", " << v->elem << ") " << " ("; v->Color(); cout << ")" << '\n';
            } catch(string sentence) {
                cout << "(0, 0) (B)\n";
            }
            Show(v->left, blank, false); // first, go left node
            Show(v->right, blank, true); // and then, go right node
        }
    }
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
                // rb.remove(key, element, rb.getRoot());
                break;
            default: 
                cout << "Exit Program\n";
                return 0;
        }
    }

    return 0;
}