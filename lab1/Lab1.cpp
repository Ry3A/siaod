#include <iostream>

using namespace std;
int cnst = 0xFFA2187;
void coutp(unsigned int x);


void createFirstMask(){
    coutp(cnst);
    cout << endl;
    int mask = 0b111100000;
    coutp(cnst | mask);
    cout << endl;
}

void createSecondMask(int x){
    int mask = ~(0b10100000);
    coutp(x);
    cout << endl;

    coutp(mask);
    cout << endl;

    coutp(x & mask);
    cout << endl;
}

void multiplication(int x){
    coutp(x << 5);
}

void del(int x){
    coutp(x >> 5);
}

void last(int x){
    int mask = 0b1;
    mask = ~(mask << x);
    coutp(cnst);
    cout << endl;
    coutp(cnst & mask);
}

void coutp(unsigned int x) {
    int n = sizeof(int) * 8;
    unsigned maska = (1 << (n - 1));
    for (int i = 1; i <= n; i++) {
        cout << ((x & maska) >> (n - i));
        maska = maska >> 1;
    }
}
/*
int main()
{
    ///createFirstMask();
    int x;
    cin >> x;
    createSecondMask(x);
    ///multiplication(x);
    ///del(x);
    ///last(x);
    return 0;
}
*/