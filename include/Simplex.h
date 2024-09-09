#include <iostream>
#include <tcb/rational.hpp>
#include <Eigen/Eigen>
#include <set>
// #include <unordered_set>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
#include <gmpxx.h>

using namespace std;
using namespace tcb::rational_literals;
using namespace tcb;
using namespace Eigen;

#ifdef USE_DOUBLE
    using BaseType = double;
#elif USE_FLOAT
    using BaseType = float;
#elif USE_RATIONAL_INT
    using BaseType = rational<int>;
#elif USE_MPQ
    using BaseType = mpq_class;
#else
    using BaseType = rational<long long>;
#endif

using namespace __gnu_pbds; 
  
#define ordered_set tree<long long, null_type,less<long long>, rb_tree_tag,tree_order_statistics_node_update> 
#define osl ordered_set
typedef Matrix<BaseType, Dynamic, Dynamic, RowMajor> Mat;
typedef Vector<BaseType, Dynamic> Vec;
/* TYPES  */
#define ll long long
#define pii pair<int, int>
#define pll pair<long long, long long>
#define vi vector<int>
#define vll vector<long long>
#define mii map<int, int>
#define si set<int>
#define sc set<char>
#define sll set<long long>
// #define usi unordered_set<int>
// #define usl unordered_set<long long>

#define pSimplex_tuple tuple<BaseType, Vec, osl, Vec, ll, ll>
// void test();
pSimplex_tuple
pSimplex(Vec c, Mat A, Vec b, osl base, ll max_iter);

pair<osl, ll> pSimplexAux(Mat A, Vec b);