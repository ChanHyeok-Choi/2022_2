#include <iostream>
#include <deque>

using namespace std;

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
        deque<int> S1, S2;
        // Partition S into S1 and S2
        for (size_t i = 0; i < (size_t)n/2; i++) {
            S1.push_back(S[i]);
            S2.push_back(S[i + n/2]);
        }
        // Recur S1 and S2 until size is 1 or 0
        S1 = MergeSort(S1, n/2);
        S2 = MergeSort(S2, n/2);
        // Merge S1 and S2 into new S
        S = Merge(S1, S2);
    }
    return S;    
}

int main() {
    deque<int> S = { 7, 2, 9, 4, 3, 8, 6, 1 };

    S = MergeSort(S, S.size());

    for (size_t i = 0; i < S.size(); i++)
        cout << S[i] << ' ';
    cout << '\n';

    return 0;
}