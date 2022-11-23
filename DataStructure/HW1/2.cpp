#include <iostream>

class Line { // represented by y = ax + b
private:
    double a, b;
public:
    Line(double a, double b) : a(a), b(b) {}
    double Intersect(Line * l) {
        try {
            if(a == l->a)
                throw a;
            return (l->b - b) / (a - l->a); // 교차 x좌표 리턴
        } catch(double exception) {
            std::cerr << "Parallel!" << '\n';
        }
    }

    void print() {
        std::cout << "y = " << a << "x + " << b << '\n';
    }
};

int main() {
    using namespace std;

    Line x(2, 3), y(2, 4), z(3, 4);

    cout << "There are lines :" << '\n';
    x.print();
    y.print();
    z.print();
    cout << "--------------------------\n";
    cout << "x coordinates of Intersect(x, y) : ";
    cout << x.Intersect(&y) << '\n';
    cout << "x coordinates of Intersect(x, z) : ";
    cout << x.Intersect(&z) << '\n';
    cout << "x coordinates of Intersect(y, z) : ";
    cout << y.Intersect(&z) << '\n';

    return 0;
}