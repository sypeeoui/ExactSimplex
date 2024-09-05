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
    base.insert(1);
    base.insert(5);
    base.insert(6);

    pSimplex(c, A, b, base, 5);
}