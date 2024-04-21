#include <iostream>
#include <tcb/rational.hpp>

using namespace std;
using namespace tcb::rational_literals;
using namespace tcb;

#ifdef USE_DOUBLE
    using BaseType = double;
#elif USE_FLOAT
    using BaseType = float;
#elif USE_RATIONAL_INT
    using BaseType = rational<int>;
#else
    using BaseType = rational<long long>;
#endif

class Simplex {
public:
    Simplex();
};
