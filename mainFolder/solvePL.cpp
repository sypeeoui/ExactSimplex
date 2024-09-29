#include <iostream>
#include "Simplex.h" // Include the Simplex header file
#include <cstdio>
// int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
    //     return 1;
    // }
    // freopen(argv[1], "r", stdin);

int main() {
    cout << "Precision = " << PRECISION << endl;

    ll csize;
    cout << "csize = ";
    cin >> csize;
    ll bsize;
    cout << "bsize = ";
    cin >> bsize;

    Vec c = Vec(csize);
    Mat A = Mat(bsize, csize);
    Vec b = Vec(bsize);
    #ifdef USE_MPQ
        mpq_class aux;
    #elif USE_MPF
        mpf_class aux;
        mpf_set_default_prec(PRECISION);
    #else
        ll aux;
    #endif
    cout << "c = ";
    for(ll i = 0; i < csize; i++) {
        cin >> aux;
        c(i) = aux;
        
    }
    cout << "A = " << endl;
    for(ll i = 0; i < bsize; i++) {
        for(ll j = 0; j < csize; j++) {
            cin >> aux;
            A(i, j) = aux;

        }
    }
    cout << "b = ";
    for(ll i = 0; i < bsize; i++) {
        cin >> aux;
        b(i) = aux;
    }
    osl base;
    ll max_iter;
    cin >> max_iter;
    pSimplex_tuple answer = pSimplex(c, A, b, base, max_iter);
    cout << "---- pSimplex ----" << endl;
    cout << "-----" << endl;
    cout << "flag = " << get<4>(answer) << endl;
    cout << "-----" << endl;
    cout << "xB = " << endl << get<1>(answer).transpose() << endl;
    cout << "-----" << endl;
    cout << "yB = " << endl << get<3>(answer).transpose() << endl;
    cout << "-----" << endl;
    cout << "base = " << endl;
    for(auto i : get<2>(answer)) cout << i << " ";
    cout << endl;
    cout << "fObj = " << get<0>(answer) << endl;
    cout << "-----" << endl;
    cout << "iter = " << get<5>(answer) << endl;
    cout << "-----" << endl;


    return 0;
}