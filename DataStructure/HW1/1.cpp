#include <iostream>
#include <stdlib.h>
#include <time.h>

void typo(char * sen, int n) {
    // srand(time(NULL));
    int idx[8], cnt = 0;

    for(int i = 0; i < 8; i++) {
        int num = rand() % (n - 1); // 랜덤하게 index 뽑기
        while(1) {
            for(int j = 0; j <= cnt; j++) {
                if(idx[j] == num)
                    num = rand() % (n - 1); // 같은 숫자를 뽑았으면 다시 index 뽑기
            }
            break;
        }
        idx[cnt++] = num;
    } // array of idx 에 typo를 넣을 idx들을 랜덤하게 저장
    
    char sentence[100];
    for(int i = 0; i < n; i++)
        sentence[i] = sen[i]; // 문장 복사

    for(int i = 0; i < 8; i++) {
        char ch = rand() % 26 + 'a'; // 랜덤 문자 생성
        if(sentence[idx[i]] == ch) // 바꾸려는 곳이 우연히 같으면 안 된다.
            sentence[idx[i]] = ch + 1;
        else
            sentence[idx[i]] = ch; // typo
    }

    for(int i = 0; i < n; i++)
        std::cout << sentence[i];\
    std::cout << '\n';
}

void print(char * sen, int n) {
    using namespace std;

    for(int i = 0; i < n; i++)
        cout << sen[i];
    cout << '\n';
}

int main() {
    using namespace std;
    char sentence[100] = "I will always use object-oriented design.";
    int n = 42;

    srand(time(NULL));
    for(int i = 0; i < 100; i++) {
        cout << i + 1 << ". ";
        int num = rand() % 100;
        if(i % 12 == 0) // 8번의 typo
            typo(sentence, n);
        else
            print(sentence, n);
    }
    
    return 0;
}