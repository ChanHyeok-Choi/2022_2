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
    int elem;
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
    RBTree(int e) : n(0) {
        root = new RBNode;
        root->color = false;
        root->elem = e;
    }
    // ~RBTree();

};

int main() {
    RBTree rb(0);

    return 0;
}