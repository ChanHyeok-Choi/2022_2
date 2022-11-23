#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {

    vector<pair<string, int>> words;

    string sentence;
    getline(cin, sentence); // 한 줄에 한 문장 입력 받기

    int index = 0;
    string word;

    while(1) {
        if(sentence[index] == ' ' || sentence[index] == '\0') {
            bool signal = false;
            for(int i = 0; i < words.size(); i++) {
                if(words[i].first == word) { // words 벡터 안에 기존 단어가 존재하는지 확인
                    words[i].second++; // 있다면, 카운트 증가
                    signal = true;
                    break;
                }
            }

            if(signal == false) { // for문 돌았는데 기존 단어랑 매치되는게 없다면, 맨 끝에 단어랑 카운트 추가
                words.push_back(make_pair(word, 1));
            }

            word = ""; // 단어 초기화

            if(sentence[index] == '\0') // 문장 끝에 도달했으면 while 종료
                break;
        } else {
            char x = sentence[index]; // 한 문자씩 string word에 추가
            word += x;
        }
        index++; // 인덱스 하나씩 증가
    }

    cout << "---< 단어 : 갯수 >---" << '\n';
    for(int i = 0; i < words.size(); i++) {
        cout << i + 1 << ". " << words[i].first << " : " << words[i].second << '\n';
    }

    return 0;
}