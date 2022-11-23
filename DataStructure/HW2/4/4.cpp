#include <iostream>
#include "Stack.h"

// Writer: 20181257 ChanHyeok Choi
/* In the Towers of Hanoi puzzle,we are given a platform with
three pegs, a, b, and c, sticking out of it. On peg a is a stack of n disks, each larger
than the next, so that the smallest is on the top and the largest is on the bottom. The
puzzle is to move all the disks from peg a to peg c, moving one disk at a time, so
that we never place a larger disk on top of a smaller one. Describe a recursive
algorithm for solving the Towers of Hanoi puzzle for arbitrary n. (Hint: Consider
first the subproblem of moving all but the nth disk from peg a to another peg using
the third as “temporary storage.”). Then Write a C++ program that can solve
instances of the Tower of Hanoi problem.
*/

using namespace std;

enum class PEG { a, b, c };
int step;
Stack<int> a_Stk, b_Stk, c_Stk;


void print(PEG x) {
    switch(x) {
        case PEG::a: 
            cout << 'a';
            break;
        case PEG::b: 
            cout << 'b';
            break;
        case PEG::c: 
            cout << 'c';
            break;
    }
}

void HanoiTower(int n, PEG a, PEG b, PEG c) {
    if (n == 1) {
        cout << "Step " << ++step << ": ";
        print(a); cout << " → "; print(c); cout << '\n';
    } else {
        HanoiTower(n-1, a, c, b); // move n-1'th disk from a to b
        cout << "Step " << ++step << ": ";
        print(a); cout << " → "; print(c); cout << '\n'; // move n'th disk from a to c
        HanoiTower(n-1, b, a, c); // move n-1'th disk from b to c
    }
}

void printStk() {
    cout << a_Stk.size() << "   " << b_Stk.size() << "   " << c_Stk.size() << '\n';
}

void HanoiTowerStk(int n, Stack<int>& a, Stack<int>& b, Stack<int>& c) {
    if (n == 1) {
        a.pop_back(); c.push_back(1); 
        cout << "Step " << ++step << ": ";
        printStk();
    } else {
        HanoiTowerStk(n-1, a, c, b);
        a.pop_back(); c.push_back(1);
        cout << "Step " << ++step << ": "; printStk();
        HanoiTowerStk(n-1, b, a, c);
    }
}

int main() {
    int n;
    cout << "How many disks are stacked on a peg 'a'? "; cin >> n;

    HanoiTower(n, PEG::a, PEG::b, PEG::c);

    cout << "-------------------\n";
    for(int i = 0; i < n; i++)
        a_Stk.push_back(1);
    
    step = 0;
    cout << "<PEG>   a   b   c\n";
    cout << "Step " << step << ": "; printStk();
    HanoiTowerStk(n, a_Stk, b_Stk, c_Stk);
    
    return 0;
}