#include <iostream>
#include <tcb/rational.hpp>
#include <Eigen/Dense>
#include "Simplex.h" 

using Eigen::Matrix;

using namespace std;
using namespace tcb::rational_literals;
using namespace tcb;

int main() {
    cout << "Hello " << 10/2_r << endl;
    Matrix<BaseType, 2, 2> m;
    cout << typeid(BaseType).name() << endl;
    m << 1/5_r, 10/2_r, 3, 4;
    cout << m << endl;
    return 0;
}