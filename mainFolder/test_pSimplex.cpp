#include <iostream>
#include "Simplex.h" // Include the Simplex header file

int main() {
    Mat c = Vec(3);
    c << 100, 80, 60;
    Mat A = Mat(8, 3);
    A <<     
    6, 5, 4,
    4, 2, 10,
    4, 5, 3,
    2, -1, 0,
    -4, 6, -4,
    -1, 0, 0,
    0, -1, 0,
    0, 0, -1;

    Mat b = Vec(8);
    b << 1000, 800, 500, 0, 0, 0, 0, 0;
    osl base;
    // base.insert(2);
    // base.insert(3);
    // base.insert(4);

    pSimplex_tuple answer = pSimplex(c, A, b, base, 3);
    cout << "---- pSimplex ----" << endl;
    cout << "fObj = " << get<0>(answer) << endl;
    cout << "-----" << endl;
    cout << "xB = " << endl << get<1>(answer).transpose() << endl;
    cout << "-----" << endl;
    cout << "base = " << endl;
    for(auto i : get<2>(answer)) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;
    cout << "yB = " << endl << get<3>(answer).transpose() << endl;
    cout << "-----" << endl;
    cout << "flag = " << get<4>(answer) << endl;
    cout << "-----" << endl;
    cout << "iter = " << get<5>(answer) << endl;
    cout << "-----" << endl;
    return 0;
}