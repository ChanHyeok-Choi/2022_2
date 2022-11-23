#ifndef _BTREE_H_
#define _BTREE_H_

#include <iostream>
#include <string>
#include "Stack.h"

template <typename T>
class BTNode {
private:
    T data;
    BTNode<T>* parent;
    BTNode<T>* left;
    BTNode<T>* right;
 
public:
    BTNode<T>() : data(), parent(NULL), left(NULL), right(NULL) {}
    T getData() const { return data; }
    void setData(const T _data) { data = _data; }
    BTNode<T>* getParent() const { return parent; }
    BTNode<T>* getLeft() const { return left; } 
    BTNode<T>* getRight() const { return right; } 
    void setParent(BTNode<T>* parent) { this->parent = parent; }
    void setLeft(BTNode<T>* left) { this->left = left; } 
    void setRight(BTNode<T>* right) { this->right = right; }

    template <typename E> friend class Position;
    template <typename F> friend class BinaryTree;
};

template <typename T>
class Position {
private:
    BTNode<T>* v;

    template <typename E> friend class BinaryTree;
};

template <typename T>
class BinaryTree {
private:
    BTNode<T>* root;
    Position<T>* cursor;
    int n;
public:
    BinaryTree() : root(NULL), cursor(NULL), n(0) {
        root = new BTNode<T>;
        cursor = new Position<T>;
        cursor->v = root;
    }
    // ~BinaryTree() { RemoveLeftSubtree(root); RemoveRightSubtree(root); }

    BTNode<T>* getRoot() const { return root; }

    BTNode<T>* CreateNode() { BTNode<T>* pNode = new BTNode<T>; pNode->setParent(cursor->v); return pNode; }

    BTNode<T>* WhatIsLeftEmptyNode(BTNode<T>* startNode) { 
        BTNode<T>* result;
        BTNode<T>* origin = cursor->v;
        cursor->v = startNode;
        while(cursor->v->left != NULL) {
            cursor->v = cursor->v->left;
        }
        result = cursor->v;
        cursor->v = origin;
        return result;
    }
    BTNode<T>* WhatIsRightEmptyNode(BTNode<T>* startNode) { 
        BTNode<T>* result;
        BTNode<T>* origin = cursor->v;
        cursor->v = startNode;
        while(cursor->v->right != NULL) {
            result = cursor->v;
            cursor->v = cursor->v->right;
        }
        result = cursor->v->parent;
        cursor->v = origin;
        return result;
    }

    void DrawBinaryTree(BTNode<T>& node);
    void PreorderTraversal(BTNode<T>& node);
    void InorderTraversal(BTNode<T>& node);
    void PostorderTraversal(BTNode<T>& node);
    void RemoveLeftSubtree(BTNode<T>& left) { left.setLeft(NULL); }
    void RemoveRightSubtree(BTNode<T>& right) { right.setRight(NULL); }

    BTNode<T>* MakeExpTree(const T exp, int n); // for only string class
};

template <typename T>
void BinaryTree<T>::DrawBinaryTree(BTNode<T>& node) {
    std::cout << node.data << " → ";

    std::cout << "Left child : ";
    if(node.getLeft() == NULL)
        std::cout << "NULL \n";
    else
        std::cout << node.left->data << '\n';

    std::cout << "   Right child : ";
    if(node.getRight() == NULL)
        std::cout << "NULL \n";
    else
        std::cout << node.right->data << '\n';
    
    if(node.left != NULL)
        DrawBinaryTree(*node.getLeft());
    if(node.right != NULL)
        DrawBinaryTree(*node.getRight());
}

template <typename T>
void BinaryTree<T>::PreorderTraversal(BTNode<T>& node) {
    if(&node == NULL)
        return;
    
    std::cout << node.getData() << " → ";
    PreorderTraversal(*node.getLeft());
    PreorderTraversal(*node.getRight());
}

template <typename T>
void BinaryTree<T>::InorderTraversal(BTNode<T>& node) {
    if(&node == NULL)
        return;
    
    InorderTraversal(*node.getLeft());
    std::cout << node.getData() << " → ";
    InorderTraversal(*node.getRight());
}

template <typename T>
void BinaryTree<T>::PostorderTraversal(BTNode<T>& node) {
    if(&node == NULL)
        return;
    
    PostorderTraversal(*node.getLeft());
    PostorderTraversal(*node.getRight());
    std::cout << node.getData() << " → ";
}

template <typename T>
BTNode<T>* BinaryTree<T>::MakeExpTree(const T exp, int p) {
    Stack<T> valStk;
    T tempVal;
    int count_blank = 0;

    for (int i = exp.size() - 1; i >= 0; i--) {
        BTNode<T>* newNode;
        if (exp[i] == ')') {
            if (exp[i+1] == '\0' || exp[i+1] == ')') { // 완전 초기 )
                count_blank++;
                newNode = new BTNode<T>;
                newNode->parent = cursor->v;
                if(cursor->v->right == NULL) {
                    cursor->v->right = newNode;
                    cursor->v = cursor->v->right;
                } else if (cursor->v->left == NULL) {
                    cursor->v->left = newNode;
                    cursor->v = cursor->v->left;
                } else {
                    BTNode<T>* targetNode = WhatIsLeftEmptyNode(cursor->v);
                    targetNode->left = newNode;
                    cursor->v = targetNode;
                }
            } else { // )+ or )- or )* or )/
                count_blank++;
                newNode = new BTNode<T>;
                newNode->parent = cursor->v; cursor->v->left = newNode;
                cursor->v = cursor->v->left;
            }

        } else if (exp[i] == '(') {
            BTNode<T>* currentNode = WhatIsLeftEmptyNode(getRoot()->right);
            while(!valStk.empty()) // 이전까지 스택에 숫자 채워넣기
                tempVal += valStk.pop_front();
            BTNode<T>* valNode = new BTNode<T>; valNode->data = tempVal;
            currentNode->left = valNode; valNode->parent = currentNode;

            tempVal.clear();        
        } else if (exp[i] == '+' || exp[i] == '-') {
            BTNode<T>* currentNode = cursor->v;
            bool IsEmpty = false;
            while(!valStk.empty()) { // 이전까지 스택에 숫자 채워넣기
                tempVal += valStk.pop_front();
                IsEmpty = true;
            }
            if (!IsEmpty) { // 이전까지 숫자가 없다면, 즉 +( or -(, 완전한 트리
                currentNode = currentNode->parent;
            } else { // 이전까지 숫자였다면 새 노드 할당해서 값 넣고 부모노드와 연결
                if (currentNode->right == NULL) {
                    BTNode<T>* valNode = new BTNode<T>; valNode->data = tempVal;
                    valNode->parent = currentNode;
                    // cursor->v = currentNode->parent;
                    currentNode->right = valNode;
                } else if(currentNode->left == NULL) {
                    BTNode<T>* valNode = new BTNode<T>; valNode->data = tempVal;
                    valNode->parent = currentNode;
                    currentNode->left = valNode;
                    // cursor->v = currentNode->parent;
                } else {
                    BTNode<T>* targetNode = WhatIsLeftEmptyNode(currentNode);
                    BTNode<T>* valNode = new BTNode<T>; valNode->data = tempVal;
                    valNode->parent = targetNode;
                    targetNode->left = valNode;
                }
            }

            if (count_blank > 0) { // ) 로 인해 생성된 노드가 있으면
                newNode = currentNode;
                newNode->data = exp[i]; // 해당 노드에 값 넣기
                count_blank--;
            } else { // 그냥 새로운 항이면
                newNode = new BTNode<T>;
                newNode->data = exp[i]; // 새 노드 생성하고 연산자 넣기
                newNode->parent = currentNode->parent;
                currentNode->parent = newNode;
                newNode->right = currentNode;
                currentNode = newNode; // cursor->v = newNode;
                currentNode->parent->right = newNode;
            }
            

            tempVal.clear();
        } else if (exp[i] == '*' || exp[i] == '/') {
            BTNode<T>* currentNode = cursor->v;
            if (count_blank > 0) { // ) 로 인해 생성된 노드가 있으면
                newNode = currentNode;
                newNode->data = exp[i]; // 해당 노드에 값 넣기
                count_blank--;
            } else { // 그냥 새로운 항이면
                newNode = new BTNode<T>;
                newNode->data = exp[i]; // 새 노드 생성하고 연산자 넣기
                newNode->parent = currentNode;
                currentNode->left = newNode;
                currentNode = newNode;
            }
            bool IsEmpty = false;
            while(!valStk.empty()) { // 이전까지 스택에 숫자 채워넣기
                tempVal += valStk.pop_front();
                IsEmpty = true;
            }
            if (!IsEmpty) { // 이전까지 숫자가 없다면, 즉 *( or /( 
                currentNode = currentNode->parent;
            } else { // 이전까지 숫자였다면 새 노드 할당해서 값 넣고 부모노드와 오른쪽 연결
                BTNode<T>* valNode = new BTNode<T>; valNode->data = tempVal;
                valNode->parent = currentNode;
                currentNode->right = valNode;
                // cursor->v = currentNode;
            }

            tempVal.clear();
        } else {
            tempVal += exp[i];
            valStk.push_back(tempVal);
            tempVal.clear();
        }
    }

    return cursor->v;
}

#endif