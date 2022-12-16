#include <iostream>
#include <map>
#include <vector>

using namespace std;

bool checkMachine(vector<int>& m, int s, int e) {
    if(m.empty()) {
        return false;
    }
    for(int i = 0; i < m.size(); i++) {
        if(m[i] <= s) {
            m[i] = e;
            return true;
        }
    }
    return false;
}

// Task Scheduling: O(NlogN)
int TaskScheduling(map<int, int> T) {
    vector<int> m; // scheduling machine
    while(!T.empty()) {
        // remove task i w/ smallest key;
        int start = T.begin()->first;
        int end = T.begin()->second;
        T.erase(T.begin());
        if(checkMachine(m, start, end)) {
            // schedule i on machine j in m;
        } else {
            // m <- m + 1
            // schedule i on new machine
            m.push_back(end);
        }     
    }
    return m.size();
}

int main() {
    map<int, int> TestCase1 = { {1, 4}, {1, 3}, {2, 5}, {3, 7}, {4, 7}, {6, 9}, {7, 8} }; // (start time, end time)

    cout << TaskScheduling(TestCase1) << '\n';

    return 0;
}