// Writer: 20181257 Chanhyeok Choi
/* 2. (30 points) Implement the LCS algorithm and use it to compute the best
sequence alignment between some DNA strings (each has 10 characters) that you
choose from the attached DNA_seq_sample.txt. Your code should print both the
length of LCS and the subsequence itself in your main().
*/

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

typedef int (*Arr)[11]; // Customize return type

Arr LCS(string X, string Y) {
    // Dynamic Programming Approach
    int n = X.length();
    int m = Y.length();
    string subsequence = "";
    static int L[11][11] = { 0, }; // initialize elements of (n+1)*(m+1) array to 0
    for(int i = 1; i < n+1; i++) {
        for(int j = 1; j < m+1; j++) {
            if(X[i-1] == Y[j-1]) {
                L[i][j] = L[i-1][j-1] + 1; // Diagonally up left thing plus one
                subsequence += X[i-1];
            }
            else {
                L[i][j] = max(L[i-1][j], L[i][j-1]); // maximum between left and up
            }
        }
    }

    return L;
}

void printArray(string arr[][6], int row, int col) {
    cout << "<---------------------------DNA_sequnce----------------------------->\n";
    cout << "i\\j ";
    for(int j = 0; j < col; j++)
        cout << "     " << j << "     ";
    cout << '\n';
    for(int i = 0; i < row; i++) {
        cout.width(4);
        cout << std::left << i;
        for(int j = 0; j < col; j++) {
            cout << arr[i][j] << ' ';
        }
        cout << '\n';
    }
}

int main() {
    string DNA[84][6];
    string line;
    ifstream file("DNA_seq_sample.txt"); // open the sample file
    if(file.is_open()){
        int line_cnt = 0;
		while(getline(file, line)) {
            if(line == "ORIGIN      ") continue;
            else if(line == "//") continue;
            istringstream iss(line);
            string word;
            int cnt = 0;
            while(iss >> word) {
                if(word.length() == 10)
                    DNA[line_cnt][cnt++] = word;
                    // cout << word << '\n';
            }
            line_cnt++;
			// cout << line << endl;
		}
		file.close(); // close the opened file
	} else {
		cout << "Unable to open file";
		return 1;
	}

    // Print DNA_seq
    printArray(DNA, 84, 6);
    cout << '\n';

    // Choose two DNA sequences in DNA_seq
    int DNA1_x, DNA1_y, DNA2_x, DNA2_y;
    cout << "Choose two DNA sequences for implementing LCS Algorithm.\n";
    cout << "DNA 1. row index: "; cin >> DNA1_x;
    cout << "DNA 1. column index: "; cin >> DNA1_y;
    cout << "DNA 2. row index: "; cin >> DNA2_x;
    cout << "DNA 2. column index: "; cin >> DNA2_y;
    cout << '\n';

    // Show your two DNA sequences
    string DNA1 = DNA[DNA1_x][DNA1_y];
    string DNA2 = DNA[DNA2_x][DNA2_y];
    cout << "<Your DNA sequences>\n";
    cout << "DNA 1. " << DNA1 << '\n';
    cout << "DNA 2. " << DNA2 << '\n';
    cout << '\n';

    // Implement LCS Algorithm
    Arr LCS_arr;
    LCS_arr = LCS(DNA1, DNA2);
    cout << '\n';

    // Print LCS array
    cout << "<----- LCS array ----->\n";
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 11; j++) {
            cout << LCS_arr[i][j] << ' ';
        }
        cout << '\n';
    }

    // print both the length of LCS and the subsequence
    int i = 10, j = 10;
    string subsequence = "";
    while(i != 0 and j != 0) {
        // cout << "check " << i << ' ' << j << ' ';
        if(LCS_arr[i][j] == LCS_arr[i][j-1]) {
            j--;
            // cout << "and move " << i << ' ' << j << ' ' << "(" << LCS_arr[i][j] << ")" << '\n';
        } else {
            subsequence = DNA2[j-1] + subsequence;
            i--; j--;
            // cout << "and record and up " << i << ' ' << j << ' ' << "(" << LCS_arr[i][j] << ")" << '\n';
        }
    }
    cout << "Length of LCS: " << LCS_arr[10][10] << '\n';
    cout << "Subsequence: " << subsequence << '\n';


    return 0;
}