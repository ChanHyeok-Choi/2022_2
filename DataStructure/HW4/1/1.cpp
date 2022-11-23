// Writer: 20181257 ChanHyeok Choi
/* 1. (30 points) Implement merge-sort and quick-sort and perform a series of
benchmarking tests (in your main()) to see which one is faster. Your tests should
include sequences that are “random” as well as “almost" sorted.
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "deque.h"

deque<int> Merge(deque<int> S1, deque<int> S2) {
    deque<int> newS;
    // Compare S1's element and S2's element and then minimum one will be inserted into new S
    while (!S1.empty() && !S2.empty()) {
        if (S1.front() < S2.front()) {
            newS.push_back(S1.front());
            S1.pop_front();
        } else {
            newS.push_back(S2.front());
            S2.pop_front();
        }
    }
    // if there're S1 or S2 have elements after the above operation, the remains will be inserted into new S
    while (!S1.empty()) {
        newS.push_back(S1.front());
        S1.pop_front();
    }
    while (!S2.empty()) {
        newS.push_back(S2.front());
        S2.pop_front();
    }
    return newS;
}

deque<int> MergeSort(deque<int> S, int n) {
    if (S.size() > 1) {
        deque<int> S1;
        deque<int> S2;
        // Divide and Conquer
        // Partition S into S1 and S2
        for (int i = 0; i < (int)n/2; i++) {
            S1.push_back(S.front());
            S.pop_front();
        }
        while (!S.empty()) {
            S2.push_back(S.front());
            S.pop_front();
        }
        // Recur S1 and S2 until their size is 1 or 0
        S1 = MergeSort(S1, S1.size());
        S2 = MergeSort(S2, S2.size());
        // Merge S1 and S2 into new S
        S = Merge(S1, S2);
    }
    return S;    
}

deque<int> QuickSort(deque<int> S, int n) {
    if (S.size() > 1) {
        // Divide and Conquer
        // Partition S into L, E, G
        deque<int> L, E, G;
        // case 1. pick first one for a pivot
        int x = S.front(); S.pop_front(); E.push_back(x);
        // case 2. pick randome one for a pivot
        // int random_idx = rand() % n;
        // int x = S.at(random_idx); S.erase(random_idx); E.push_back(x);
        while(!S.empty()) {
            int y = S.front(); S.pop_front();
            if(y < x)
                L.push_back(y);
            else if(y == x)
                E.push_back(y);
            else // y > x
                G.push_back(y);
        }
        // Recur L and G until their size is 1 or 0
        L = QuickSort(L, L.size());
        G = QuickSort(G, G.size());
        // Merge L, E, G into S
        while(!L.empty()) {
            S.push_back(L.front()); L.pop_front();
        }
        while(!E.empty()) {
            S.push_back(E.front()); E.pop_front();
        }
        while(!G.empty()) {
            S.push_back(G.front()); G.pop_front();
        }
    }
    return S;
}

void printS(deque<int>* S, int size) {
    using namespace std;
    for (int i = 0; i < size; i++)
        cout << S->at(i) << ' ';
    cout << '\n';
}

int main() {
    using namespace std;

    srand((unsigned int)time(NULL));

    deque<int> S_merge, S_quick; int n;
    cout << "How many would you like to create random numbers(0 <= x <= 99) in a sequence? "; cin >> n;
    for(int i = 0; i < n; i++) {
        int random = rand() % 100;
        S_merge.push_back(random);
        S_quick.push_back(random);
    }
    
    // Before sorting
    cout << "Sequence before MergeSort: "; printS(&S_merge, S_merge.size());
    cout << "Sequence before QuickSort: "; printS(&S_quick, S_quick.size());

    // MergeSort: O(NlogN)
    clock_t start_merge = clock();
    S_merge = MergeSort(S_merge, S_merge.size());
    clock_t end_merge = clock();
    double time_merge = double(end_merge - start_merge);
    cout << "Time for MergeSort: " << time_merge << " ms \n";

    // QuickSort: O(NlogN)
    clock_t start_quick = clock();
    S_quick = QuickSort(S_quick, S_quick.size());
    clock_t end_quick = clock();
    double time_quick = double(end_quick - start_quick);
    cout << "Time for QuickSort: " << time_quick << " ms \n";

    // After sorting
    cout << "Sequence after MergeSort: "; printS(&S_merge, S_merge.size());
    cout << "Sequence after QuickSort: "; printS(&S_quick, S_quick.size());

    // I think QuickSort is faster than MergeSort!
    return 0;
}