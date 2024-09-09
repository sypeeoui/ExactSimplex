#include "Simplex.h"
#ifdef USE_DOUBLE
    const BaseType tol = 1e-4;
#elif USE_FLOAT
    const BaseType tol = 1e-2;
#else
    const BaseType tol = 0;
#endif
// void test() {
//     cout << "Hello " << 10/2_r << endl;
//     Mat m(3,3);
//     m << 1/5_r, 10/2_r, 3, 4, 5, 6, 7, 8, 9;
//     m = 2*m;
//     m(0,1) = 1/5_r;

//     cout << m << endl;
// }

Mat inverse(Mat A) {
    // implement the inverse function using gauss-jordan elimination
    // cout << "A = " << endl << A << endl;
    ll n = A.rows();
    Mat B = Mat::Identity(n, n);
    Mat P = Mat::Identity(n, n); // Permutation matrix
    // cout << "B = " << endl << B << endl;
    for (ll i = 0; i < n; i++) {
        BaseType pivot = A(i, i);
        if (pivot == 0) {
            cout << "Sono qua dentro" << endl;
            // find the pivot
            ll pivot_idx = -1;
            for (ll j = i + 1; j < n; j++) {
                if (A(j, i) != 0) {
                    pivot_idx = j;
                    break;
                }
            }
            if (pivot_idx == -1) {
                throw runtime_error("Pivot is zero");
                return Mat::Zero(n, n);
            }

            cout << "pivot_idx = " << pivot_idx << endl;
            // swap rows
            A.row(i).swap(A.row(pivot_idx));
            B.row(i).swap(B.row(pivot_idx));
            pivot = A(i, i);
        }
        A.row(i) /= pivot;
        B.row(i) /= pivot;
        for (ll j = 0; j < n; j++) {
            if (j == i) continue;
            BaseType factor = A(j, i);
            A.row(j) -= factor * A.row(i);
            B.row(j) -= factor * B.row(i);
        }
    }
    return B;
}

pSimplex_tuple
pSimplex(Vec c, Mat A, Vec b, osl base, ll max_iter) {
    ll iter = 0;
    Mat B(A.cols(), A.cols());
    Mat N(A.rows() - A.cols(), A.cols());
    Mat Binv(A.cols(), A.cols());
    Mat W(A.cols(), A.cols());

    Vec bB(A.cols());
    Vec xB(A.cols());
    Vec yB(A.rows());
    
    Vec Wh(A.rows());
    Vec AWh(A.rows());

    Vec ratio(A.rows() - A.cols());
    Vec keyratio(A.rows() - A.cols());
    ll ridx;

    // output variables fObj,xB,B,yB,flag,iter
    BaseType fObj;
    // xB;
    // base;
    // yB;
    ll flag; 
    //  -100 generic error,
    //  -5 unbounded, 
    //  -3 linear dependent base,
    //  -2 infeasible,
    //  0 max_iter reached
    //  1 optimal
    // iter;

    bool feasible_checked = false;
    
    cout << "----- pSimplex -----" << endl;
    cout << "c = " << endl << c.transpose() << endl;
    cout << "-----" << endl;
    cout << "A = " << endl << A << endl;
    cout << "-----" << endl;
    cout << "b = " << endl << b.transpose() << endl;
    cout << "-----" << endl;
    cout << "base = " << endl;
    for(auto i : base) cout << i << " ";
    cout << endl;

    if (c.size() != A.cols()) {
        cout << "Error: c.size() != A.cols()" << endl;
        return pSimplex_tuple
        (xB.dot(c), xB, base, yB, -100, iter);
    }
    if (A.rows() != b.size()) {
        cout << "Error: A.rows() != b.size()" << endl;
        return pSimplex_tuple
        (xB.dot(c), xB, base, yB, -100, iter);
    }
    if (A.cols() != base.size()) {
        cout << "Base not setted, using auxiliary problem..." << endl;
        pair<osl, ll> AuxAnswer = pSimplexAux(A, b);
        base = AuxAnswer.first;
        iter = AuxAnswer.second;
    }
    

    for (iter; iter < max_iter; iter++) {
        
        cout << "---- Iteration " << iter << " ----" << endl;

        ll Nidx = 0;
        ll key;

        for (ll i = 0; i < A.rows(); i++) {
            if (base.find(i) != base.end()) {
                    key = base.order_of_key(i);
                    bB(key) = b(i);
                    B.row(key) = A.row(i);
            } else {
                N.row(Nidx++) = A.row(i);
            }
        }
        cout << "B = " << endl << B << endl;
        cout << "-----" << endl;

        // if (B.determinant() == 0) {
        //     cout << "B = " << endl << B << endl;
        //     cout << "Error: Linear dependent base" << endl;
        //     return pSimplex_tuple
        //     (xB.dot(c), xB, base, yB, -3, iter);
        // }
        Binv = inverse(B);
        xB = Binv * bB; 
        yB = c.transpose() * Binv;


        cout << "Binv = " << endl << Binv << endl;
        cout << "-----" << endl;
        // cout << "B.inverse()" << endl << B.inverse() << endl;
        // cout << "-----" << endl;
        // cout << "B.inverse()*B" << endl << B.inverse()*B << endl;
        cout << "-----" << endl;
        cout << "N = " << endl << N << endl;
        cout << "-----" << endl;
        cout << "bB = " << endl << bB.transpose() << endl;
        cout << "-----" << endl;
        cout << "xB = " << endl << xB.transpose() << endl;
        cout << "-----" << endl;
        cout << "yB = " << endl << yB.transpose() << endl;
        cout << "-----" << endl;

        // check if it's feasible
        if (!feasible_checked) {
            bool feasible = true;
            feasible_checked = true;
            cout << "Check if it's feasible" << endl;
            cout << "tol = " << tol << endl;
            for (ll i = 0; i < b.size(); i++) {
                if (b(i) - A.row(i).dot(xB) < -tol) {
                    cout << "++++++++++++++++++++++++++++++" << endl;
                    cout << "Infeasible base" << endl;
                    cout << "++++++++++++++++++++++++++++++" << endl;
                    feasible = false;
                    return pSimplex_tuple
                    (xB.dot(c), xB, base, yB, -2, iter);
                }
            }
        }


        cout << "Check if yB >= 0" << endl;
        bool yB_geq_0 = true;
        for (ll i = 0; i < yB.size(); i++) {
            if (yB(i) < -tol) {
                yB_geq_0 = false;
                break;
            }
        }
        if (yB_geq_0) {
            cout << "++++++++++++++++++++++++++++++" << endl;
            cout << "Optimal solution found" << endl;
            cout << "++++++++++++++++++++++++++++++" << endl;
            cout << "xB = " << endl << xB.transpose() << endl;
            cout << "yB = " << endl << yB.transpose() << endl;
            cout << "fObj = " << xB.dot(c) << endl;
            cout << "iter = " << iter << endl;
            cout << "++++++++++++++++++++++++++++++" << endl;
            return pSimplex_tuple
            (xB.dot(c), xB, base, yB, 1, iter);
        }

        // Find the outgoing index
        ll h;
        for (ll i = 0; i < yB.size(); i++) {
            if (yB(i) < -tol) {
                h = *base.find_by_order(i);
                break;
            }
        }

        cout << "-------" << endl;
        cout << "h = " << h << endl;
        cout << "-------" << endl;

        W = -Binv;
        cout << "W = " << endl << W << endl;
        cout << "-----" << endl;

        Wh = W.col(base.order_of_key(h));

        cout << "Wh = " << endl << Wh.transpose() << endl;
        cout << "-----" << endl;


        AWh = A * Wh;

        cout << "AWh = " << endl << AWh.transpose() << endl;
        cout << "-----" << endl;

        cout << "Check if unbounded" << endl;
        bool unbounded = true;
        for (ll i = 0; i < AWh.size(); i++) {
            if (AWh(i) > tol) {
                unbounded = false;
                break;
            }
        }

        if (unbounded) {
            cout << "Unbounded solution" << endl;
            return pSimplex_tuple
            (xB.dot(c), xB, base, yB, -5, iter);
        }

        cout << "calculating ratios..." << endl;
        // ratios
        // find the incoming index
        ll k = 0;
        BaseType theta = -1;
        ridx = 0;

        for (ll i = 0; i < A.rows(); i++) {
            if (AWh(i) > tol) {
                // cout << (b(i) - A.row(i).dot(xB)) / AWh(i)<< endl;
                BaseType rvalue = (b(i) - A.row(i).dot(xB)) / AWh(i);
                ratio(ridx) = rvalue;
                keyratio(ridx++) = i;
                if (theta == -1 || rvalue < theta) {
                    theta = rvalue;
                    k = i;
                }
            } 
        }
        cout << "-------" << endl;
        cout << "ratio = " << endl << ratio.transpose() << endl;
        cout << "-------" << endl;
        cout << "theta = " << theta << endl;
        cout << "-------" << endl;
        cout << "k = " << k << endl;
        cout << "-------" << endl;

        base.erase(h);
        base.insert(k);

        cout << "base = " << endl;
        for(auto i : base) cout << i << " ";
        cout << endl;

    }

    return pSimplex_tuple
    (xB.dot(c), xB, base, yB, 0, iter);

}

pair<osl, ll> pSimplexAux(Mat A, Vec b) {
    ll iter = 0;
    ll nVar = A.cols();
    ll nCon = A.rows();
    Mat B(A.cols(), A.cols());
    Mat Binv(A.cols(), A.cols());

    Vec bB(A.cols());
    Vec xB(A.cols());
    Vec yB(A.rows());
    
    osl base;
    ll bIdx = 0;
    for(ll i = 0; i < nVar; i++) {
        base.insert(i);
        B.row(i) = A.row(i);
        bB(bIdx++) = b(i);
    }
    Binv = inverse(B);
    xB = Binv * bB;

    cout << "----- pSimplexAux -----" << endl;
    cout << "A = " << endl << A << endl;
    cout << "-----" << endl;
    cout << "b = " << endl << b.transpose() << endl;
    cout << "-----" << endl;
    cout << "base = " << endl;
    for(auto i : base) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;
    cout << "B = " << endl << B << endl;
    cout << "-----" << endl;
    cout << "xB = " << endl << xB.transpose() << endl;
    cout << "-----" << endl;

    bool feasible = true;
    for (ll i = 0; i < b.size(); i++) {
        if (A.row(i).dot(xB) > b(i) + tol) {
            cout << "A.row(" << i << ").dot(xB) = " << A.row(i).dot(xB) << endl;
            cout << "b(" << i << ") = " << b(i) << endl;
            feasible = false;
            break;
        }
    }
    if (feasible) {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "base is feasible" << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        return make_pair(base, iter); 
    }

    cout << "------" << endl;
    cout << "base is not feasible. Starting auxiliary problem..." << endl;
    cout << "------" << endl;

    osl U, V;
    for (ll i = nVar; i < nCon; i++) {
        if (A.row(i).dot(xB) > b(i) + tol) {
            V.insert(i);
        } else {
            U.insert(i);
        }
    }
    cout << "-----" << endl;
    cout << "U = " << endl;
    for(auto i : U) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;
    cout << "V = " << endl;
    for(auto i : V) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;

    ll nConAux = nCon + V.size();
    ll nVarAux = nVar + V.size();

    Mat Aaux(nConAux, nVarAux);

    Aaux.block(0, 0, nCon, nVar) = A;

    ll Idx = 0;
    for (ll i: V) {
        Aaux(i, nVar + Idx) = -1;
        // cout << "Aaux(" << i << "," << nVar + Idx << ") = -1" << endl;
        Aaux(nCon + Idx, nVar + Idx) = -1;
        // cout << "Aaux(" << nCon + Idx << "," << nVar + Idx << ") = -1" << endl;
        // cout << "Aaux_" << Idx <<  " = " << endl << Aaux << endl;
        Idx++;
    }

    Vec baux(nConAux);
    baux.head(nCon) = b;

    Vec cAux(nVarAux);
    cAux.tail(V.size()) = -Vec::Ones(V.size());

    osl baseAux;
    baseAux.join(base);
    baseAux.join(V);

    cout << "-----" << endl;
    cout << "U = " << endl;
    for(auto i : U) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;
    cout << "V = " << endl;
    for(auto i : V) cout << i << " ";
    cout << endl;
    cout << "-----" << endl;
    cout << "Aaux = " << endl << Aaux << endl;
    cout << "-----" << endl;
    cout << "baux = " << endl << baux.transpose() << endl;
    cout << "-----" << endl;
    cout << "cAux = " << endl << cAux.transpose() << endl;
    cout << "-----" << endl;
    cout << "baseAux = " << endl;
    for(auto i : baseAux) cout << i << " ";
    cout << endl;
    pSimplex_tuple answer = pSimplex(cAux, Aaux, baux, baseAux, 100);

    if (get<4>(answer) == 1) {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "Auxiliary problem solved" << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        // remove the artificial variables
        osl baseAux = get<2>(answer);
        cout << "baseAux = " << endl;
        for(auto i : baseAux) cout << i << " ";
        cout << endl;
        cout << "-----" << endl;
        osl base;
        iter = get<5>(answer) + 1;
        for (auto i : baseAux) {
            if (i < nCon) {
                base.insert(i);
            }
            if (base.size() == nVar) {
                break;
            }
        }
        return make_pair(base, iter);
    } else {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "Auxiliary problem failed" << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        return make_pair(base, iter);
    }
}