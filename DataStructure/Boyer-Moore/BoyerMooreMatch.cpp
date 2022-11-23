#include <iostream>
#include <map>
#include <string>

using namespace std;

void lastOccurenceFunction(string P, map<char, int>* alphabet) {
    for(map<char, int>::iterator iter = alphabet->begin(); iter != alphabet->end(); iter++) {
        char c = iter->first;
        int max_idx = 0;
        for(int i = 0; i < P.length(); i++) {
            if(P[i] == c) {
                max_idx = max(max_idx, i);
            }
        }
        iter->second = max_idx;
    }
}

int BoyerMooreMatch(string T, string P, map<char, int>* alphabet) {
    lastOccurenceFunction(P, alphabet);
    int n = T.length();
    int m = P.length();
    int i = m - 1;
    int j = m - 1;
    while(i > n-1) {
        if(T[i] == P[j]) {
            if(j == 0) return i; // match at i
            else {
                i--;
                j--;
            }
        } else {
            // character-jump
            int l = alphabet->find(T[i])->second;
            i += m - min(j, 1 + l);
            j = m - 1;
        }
    }
    return -1; // no match
}

int main() {
    map<char, int> alphabet;
    // a ~ z ASCII code: 97 ~ 128
    for(int i = 0; i < 26; i++) {
        char c = (char)(i + 97);
        alphabet.insert({c, 0});
    }

    // Test #1
    string T = "abacaabadcabacabaabb";
    string P = "abacab";
    // Output: 

    // cin >> T;
    // cin >> P;

    cout << "Where is matched? " << BoyerMooreMatch(T, P, &alphabet) << '\n';

    return 0;
}