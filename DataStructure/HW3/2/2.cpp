// Writer: 20181257 ChanHyeok Choi
/* 2. (35 points) Implement the map ADT with a hashFunction table with separate-chaining
collision handling. Your main() should include code for put, erase and other
operation examples and visualizing your hashFunction table data structures before and after
the operations.
*/

#include <iostream>
#include <string>
#include "vector.h"

using namespace std;

int hashFunction(int k, int n) {
    return (int)k % n; // h(k) = k mod n
}

template <typename K, typename V>
class Entry {
private:
    K key;
    V value;
public:
    Entry(K k, V v) : key(k), value(v) {}
    K getKey() const { return key; }
    V getValue() const { return value; }
    void setKey(K k) { key = k; }
    void setValue(V v) { value = v; }
    void show() { cout << "(" << key << ", " << value << ")"; }

    template <typename X, typename Y> friend class Map;
};

template <typename K, typename V>
class Map {
private:
    vector<vector<Entry<K, V>>>* Array; // simple vector-based map
    vector<Entry<K, V>>* end;
    int size;
    int n;
public:
    Map(int _size) : size(_size), n(0) {
        Array = new vector<vector<Entry<K, V>>>(_size + 1);
        for (int i = 0; i < _size; i++) {
            vector<Entry<K, V>>* v = new vector<Entry<K, V>>(_size);
            Array->push_back(v);
        }
        end = new vector<Entry<K, V>>(0);
        Array->push_back(end);
    }
    int getN() { return n; }
    bool isEmpty() { return n == 0; }
    Entry<K, V>* find(K k) {
        if (Array->at(hashFunction(k, size))->empty())
            return end->at(0);
        return Array->at(hashFunction(k, size))->at(0);
    }
    Entry<K, V>* put(K k, V v) {
        Entry<K, V>* p = new Entry<K, V>(k, v);
        Array->at(hashFunction(k, size))->push_back(p);
        n++;
        return p;
    }
    void erase(K k) {
        Array->at(hashFunction(k, size))->pop_back();
        n--;
    }

    void Draw() {
        cout << "[Index] [Value]\n";
        for (int i = 0; i < size; i++) {
            cout << i << ". ";
            for (int j = 0; j < Array->at(i)->sizet(); j++) {
                cout << Array->at(i)->at(j)->getValue() << ' ';
            }
            cout << '\n';
        }
    }
};

int main() {
    // h(x) = x mod n
    int size = 13;
    Map<int, int> customed_map(size);

    // put operation
    customed_map.put(54, 54);
    customed_map.put(28, 28);
    customed_map.put(41, 41);
    customed_map.put(18, 18);
    customed_map.put(10, 10);
    customed_map.put(36, 36);
    customed_map.put(25, 25);
    customed_map.put(38, 38);
    customed_map.put(12, 12);
    customed_map.put(90, 90);

    // Draw map
    cout << "<put and then Draw operation>\n";
    customed_map.Draw();

    // find operation
    cout << "\n<find operation>\n";
    cout << customed_map.find(2) << "->"; customed_map.find(2)->show(); cout << '\n';
    cout << customed_map.find(10) << "->"; customed_map.find(10)->show(); cout << '\n';
    cout << customed_map.find(12) << "->"; customed_map.find(12)->show(); cout << '\n';
    cout << customed_map.find(0) << "->"; 
    // customed_map.find(0)->show(); cout << '\n'; // this will give error (because of Segmant Fault)

    // erase operation
    customed_map.erase(2);
    customed_map.erase(10);
    customed_map.erase(12);

    // Draw map
    cout << "\n\n<erase and then Draw operatoin>\n";
    customed_map.Draw();

    return 0;
}