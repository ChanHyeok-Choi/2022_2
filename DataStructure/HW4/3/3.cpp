// Writer: 20181257 Chanhyeok Choi
/* 3. (40 points) Perform an experimental comparison of the relative speeds of the
brute-force, KMP, and BM pattern matching algorithms. Document the time taken
for coding up each of these algorithms as well as their relative running times on
documents found on the Internet that are then searched using varying-length
patterns.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int BruteForceMatch(string T, string P) {
    int n = T.length();
    int m = P.length();
    for(int i = 0; i <= n-m; i++) {
        // test shift i of the pattern
        int j = 0; // index of pattern
        while(j < m && T[i+j] == P[j])
            j++;
        if(j == m)
            return i; // match at i
        else // j < m
            continue;
            // break; // mismatch
    }
    return -1;
}

void failureFunction(string P, int* F) {
    int m = P.length();
    int i = 1;
    int j = 0;
    while(i < m) {
        if(P[i] == P[j]) {
            // we have matched j+1 chars
            F[i] = j + 1;
            i++;
            j++;
        } else if(j > 0) {
            // use failure function to shift P
            j = F[j - 1];
        }
        else {
            F[i] = 0; // no match
            i++;
        }
    }
}

int KMPMatch(string T, string P) {
    int n = T.length();
    int m = P.length();
    int F[100] = { 0, }; failureFunction(P, F); // temporarily array size is assigned to 100 
    // initialize starting index for comparison
    int i = 0;
    int j = 0;
    while(i < n) {
        if(T[i] == P[j]) {
            if(j == m-1)
                return i - j; // match
            else {
                i++;
                j++;
            }
        } else {
            if(j > 0)
                j = F[j - 1];
            else
                i++;
        }
    }
    return -1; // no match
}


void lastOccurenceFunction(string P, int* alphabet) {
    // A ~ Z, a ~ z -> ASCII code: 65 ~ 90, 97 ~ 122
    int m = P.length();
    for(int i = 0; i < 256; i++)
        alphabet[i] = -1; // all elements are initalized with -1
    for(int i = 0; i < m; i++) {
        alphabet[(int)P[i]] = i; // if duplicate characters are in P, a last character index will be set to alphabet array
    }
}


int BoyerMooreMatch(string T, string P) {
    int alphabet[256] = { 0, };
    lastOccurenceFunction(P, alphabet);
    int n = T.length();
    int m = P.length();
    // initialize starting index for comparison
    int i = m - 1;
    int j = m - 1;
    while(i <= n-1) {
        if(T[i] == P[j]) {
            if(j == 0) 
                return i; // match at i
            else {
                i--;
                j--;
            }
        } else {
            // character-jump
            int l = alphabet[(int)T[i]];
            i += m - min(j, 1 + l);
            j = m - 1;
        }
    }
    return -1; // no match
}

int main() {
    // Test #1
    // string T = "abacaabadcabacabaabb";
    // string P = "abacab";
    // Output: 10

    string T = ""; string P = "";
    cout << "Input your text: "; cin >> T;
    cout << "Input your pattern for comparison with your text: "; cin >> P;

    // Brute-Force Pattern Matching: O(nm)
    // Example of worst case: T = aaa ... ah, P = aaah
    clock_t start_BruteForce = clock();
    cout << "Where is matched? " << BruteForceMatch(T, P) << '\n'; // Output: 10
    clock_t end_BruteForce = clock();
    double time_BruteForce = double(end_BruteForce - start_BruteForce);
    
    // KMP Failure function: O(m)
    // Knuth-Morris-Pratt's algorithm: O(m+n) is faster than the brute-force algorithm
    clock_t start_KMP = clock();
    cout << "Where is matched? " << KMPMatch(T, P) << '\n';
    clock_t end_KMP = clock();
    double time_KMP = double(end_KMP - start_KMP);

    // last-occurrence function can be computed in time O(m+s)
    // Brute-Force Pattern Matching: O(nm + s) is significiantly faster than the brute-force algorithm
    //  1. Looking-glass heuristic
    //  2. Character-jump hueristic
    // Example of worst case: T = aaa ... a, P = baaa
    clock_t start_BM = clock();
    cout << "Where is matched? " << BoyerMooreMatch(T, P) << '\n';
    clock_t end_BM = clock();
    double time_BM = double(end_BM - start_BM);

    cout << "Time for BruteForceMatch: " << time_BruteForce << " ms \n";
    cout << "Time for KMP: " << time_KMP << " ms \n";
    cout << "Time for BM: " << time_BM << " ms \n";

    // Performance of KMP and BM will be different according to a given text and pattern
    // In my experiment, results show that the BM algorithm is a little bit faster than the KMP algorithm

    return 0;
}