// Writer: 20181257 ChanHyeok Choi
/* 3. (35 points) Write a C++ class that implements all the functions of the ordered
map ADT using an AVL tree. Your main() should also include code for visualizing
your data structures before and after the operations.
*/

#include <iostream>
#include <string>
#define max(a, b) (a > b ? a : b)

using namespace std;

class BNode {
private:
    int key;
    BNode* left; // minimum key will be stored in leftmost node in AVL tree
    BNode* right; // maximum key will be stored in rightmost node in AVL tree
    int height;

    friend class AVLTree;
public:
    bool isExternal() { return left == NULL && right == NULL; }
};

class AVLTree {
private:
    BNode* root;
    int n;
public:
    AVLTree() : root(NULL), n(0) {}
    int size() { return n; }
    int empty() { return n == 0; }
    BNode* getRoot() { return root; }
    int getKey(BNode* v) { if (v == NULL) return 0; else return v->key; }
    int getHeight(BNode* v) {
        if (v == NULL) return 0;
        else return v->height;
    }
    int diff(BNode* v) { // difference of height of left and right from the node
        return getHeight(v->left) - getHeight(v->right);
    }
    BNode* makeNewNode(int k) {
        BNode* newNode = new BNode;
        newNode->key = k;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;

        return newNode;
    }
    BNode* leftRotate(BNode* v) { // single rotate to left
        BNode* r = v->right;
        BNode* rl = r->left;
    
        v->right = rl;
        r->left = v; // rotate

        v->height = max(getHeight(v->left), getHeight(v->right)) + 1; 
        r->height = max(getHeight(r->left), getHeight(r->right)) + 1; // update height of each changed node

        return r;
    }
    BNode* rightRotate(BNode* v) { // single rotate to right
        BNode* l = v->left;
        BNode* lr = l->right;

        v->left = lr;
        l->right = v; // rotate
        
        v->height = max(getHeight(v->left), getHeight(v->right)) + 1; 
        l->height = max(getHeight(l->left), getHeight(l->right)) + 1; // update height of each changed node

        return l;
    }
    BNode* Restructure(BNode* v) {
        // step 1. check balancing
        // step 2. rotate the trinode by cases
        if (diff(v) > 1) { // 1-1. height of left node is larger than right node, which means that new node is inserted to left subtree
            if (diff(v->left) > 0) { // new node is left left node
                v = rightRotate(v);
            } else { // new node is left right node
                v->left = leftRotate(v->left);
                v = rightRotate(v);
            }
        } else if (diff(v) < -1) { // 1-2. height of right node is larger than left node, which means that new node is inserted to right subtree
            if (diff(v->right) > 0) { // new node is right left node
                v->right = rightRotate(v->right);
                v = leftRotate(v);
            } else { // new node is right right node
                v = leftRotate(v);
            }
        }
        return v;
    }
    BNode* put(BNode* v, int k) { // insert a new key into AVL tree
        // csae 1. if as like inorder traversal, k is smaller than v->key, k should be inserted into new left node
        // case 2. else, k should be inserted into new right node
        // And then, we should do balancing the AVL tree

        if (v == NULL) { // if AVL tree is empty
            v = makeNewNode(k);
            root = v;
            return v;
        } else if (k < v->key) { // case 1.
            v->left = put(v->left, k);
        } else { // case 2.
            v->right = put(v->right, k);
        }

        v->height = max(getHeight(v->left), getHeight(v->right)) + 1; // update height of node
        v = Restructure(v); // balancing

        return v;
    }
    BNode* erase(BNode* v, int k) {
        // case 1. tree is empty
        // case 2. if k is found, k has no child or only one child, and then copy that and remove k
        // case 3. if k is found, k has more than two children, and then find minimum key, copy that and remove k
        // And then, we should do balancing the AVL tree during erase

        if (v == NULL) // case 1.
            return v;
        if (k < v->key)
            v->left = erase(v->left, k); // go left node
        else if( k > v->key )
            v->right = erase(v->right, k); // go right node
        else { // if k is found
            if ((v->left == NULL) || (v->right == NULL)) { // case 2.
                BNode* tempNode = v->left ? v->left : v->right; 
                
                if (tempNode == NULL) { // case 2-1. there's no child
                    tempNode = v; 
                    v = NULL; 
                } else {// case 2-2. there's one child
                    v->key = tempNode->key;
                    v->height = tempNode->height;
                    v->right = tempNode->right;
                    v->left = tempNode->left;
                }

                delete tempNode; 
            } else { // case 3.
                BNode* tempNode = v->right;
                while (tempNode->left != NULL) {
                    tempNode = tempNode->left;
                } // set minimum node
                v->key = tempNode->key;
                v->right = erase(v->right, tempNode->key);
            } 
        } 

        if (v != NULL) { // if v is not erased
            v->height = max(getHeight(v->left), getHeight(v->right)) + 1; // update height of node
            v = Restructure(v); // balancing
        }
    
        return v; 
    }
    BNode* find(BNode* v, int k) {
        if (v->key < k && v->right != NULL) {
            return find(v->right, k); // go right node
        } else if (v->key > k && v->left != NULL) {
            return find(v->left, k); // go left node
        } else if (v->key == k) {
            return v; // found
        } else {
            return NULL; // not exist
        }
    }
    void Show(BNode* v, string blank, bool rightOrleft) { // show the AVL tree
        if (v != NULL) {
            cout << blank; // print blank by depth of v
            if (rightOrleft) {
                cout << "R--->";
                blank += "     "; // add blank by depth of v
            } else {
                cout << "L--->";
                blank += "|    "; // bridge with right node
            }
            cout << v->key << " (" << max(getHeight(v->left), getHeight(v->right)) + 1 << ")" << '\n';
            Show(v->left, blank, false); // first, go left node
            Show(v->right, blank, true); // and then, go right node
        }
    }
};

int main() {
    AVLTree AVLTree;
    BNode* rootNode = AVLTree.getRoot();

    // insert 44, 17, 62, 32, 50, 78, 48, 54, 9, 12, 14
    cout << "<insert and then Show operation>\n";
    rootNode = AVLTree.put(rootNode, 44); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 17); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 62); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 32); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 50); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 78); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 48); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 54); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 88); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.put(rootNode, 12); AVLTree.Show(rootNode, "", true); 
    rootNode = AVLTree.put(rootNode, 14); AVLTree.Show(rootNode, "", true);

    // find 14, 54, 15
    cout << "\n<find operation>\n";
    cout << AVLTree.find(rootNode, 14) << "->" << AVLTree.getKey(AVLTree.find(rootNode, 14)) << '\n';
    cout << AVLTree.find(rootNode, 54) << "->" << AVLTree.getKey(AVLTree.find(rootNode, 54)) << '\n';
    cout << AVLTree.find(rootNode, 15) << "->" << AVLTree.getKey(AVLTree.find(rootNode, 15)) << '\n';

    // erase 14, 54
    cout << "\n<erase and then Show operation>\n";
    rootNode = AVLTree.erase(rootNode, 14); AVLTree.Show(rootNode, "", true);
    rootNode = AVLTree.erase(rootNode, 54); AVLTree.Show(rootNode, "", true);

    return 0;
}