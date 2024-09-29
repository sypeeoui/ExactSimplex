#include <iostream>
#include <tcb/rational.hpp>
#include <Eigen/Eigen>
#include <set>
// #include <unordered_set>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
#include <gmpxx.h>
#include <chrono>

using namespace std;
using namespace tcb::rational_literals;
using namespace tcb;
using namespace Eigen;
using namespace __gnu_pbds; 
using namespace std::chrono;

#ifdef USE_DOUBLE
    using BaseType = double;
#elif USE_FLOAT
    using BaseType = float;
#elif USE_RATIONAL_INT
    using BaseType = rational<int>;
#elif USE_MPQ
    using BaseType = mpq_class;
#elif USE_MPF
    using BaseType = mpf_class;
#else
    using BaseType = rational<long long>;
#endif

/* TYPES  */
#define ll long long
#define ull unsigned long long
#define ul unsigned long
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

#define ordered_set tree<ll, null_type,less<ll>, rb_tree_tag,tree_order_statistics_node_update> 
#define osl ordered_set
typedef Matrix<BaseType, Dynamic, Dynamic, RowMajor> Mat;
typedef Vector<BaseType, Dynamic> Vec;

#define pSimplex_tuple tuple<BaseType, Vec, osl, Vec, ll, ll>


// Funzione Simplesso primale
pSimplex_tuple
pSimplex(Vec c, Mat A, Vec b, osl base, ll max_iter);

// Funzione Simplesso primale ausiliario
pair<osl, ll> pSimplexAux(Mat A, Vec b, ll max_iter);

// Funzione inversa con gauss
Mat inverse(Mat A);

// Shermon-Morrison inverse update
Mat inverse_update(Mat Ainv, Vec u, Vec v);

// Funzione di utilità per il calcolo del segno in base al tipo
int get_sign(BaseType x);