#include <iostream>
#include <string>
#include "BTree.h"

// Writer : 20181257 ChanHyeok Choi
/* Write a program that takes, as input, a fully parenthesized, arithmetic
expression and converts it to a binary expression tree. Your program should display
the tree in some way and also print the value associated with the root. For an
additional challenge, allow for the leaves to store variables of the form x1, x2, x3,
and so on, which are initially 0 and which can be updated interactively by your
program, with the corresponding update in the printed value of the root of the
expression tree.
*/

using namespace std;

int main() {
    // input : ((1+22)*13-4/2+10)
    string expression;
    cout << "Input your expression (ex: ((1+22)*13-4/2+10): "; cin >> expression;

    BinaryTree<string> expTree;
    BTNode<string>* expressionTree = expTree.MakeExpTree(expression, 0);
    expTree.DrawBinaryTree(*(expTree.getRoot()));
    expTree.PreorderTraversal(*(expTree.getRoot()));


    return 0;
}