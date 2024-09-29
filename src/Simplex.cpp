#include "Simplex.h"
#ifdef USE_DOUBLE
    const BaseType tol = 1e-5;
#elif USE_FLOAT
    const BaseType tol = 1e-2;
#elif USE_MPQ
    const BaseType tol = 0;
#elif USE_MPF
    string prec = to_string((int)(PRECISION/16));
    string strprec = "1e-" + prec;
    const BaseType tol(strprec);
#else
    const BaseType tol = 0;
#endif


#if defined(USE_MPQ)
    int get_sign(BaseType x) {
        return (x < 0 ? -1 : (x > 0 ? 1 : 0) );
    }
#else
    int get_sign(BaseType x) {
        // if (x == 0) return 0;
        // return -signbit(x);
        return x < -tol ? -1 : x > tol;
        // return (x < 0 ? -1 : (x > 0 ? 1 : 0) );
    }
#endif
// #if defined(USE_RATIONAL) || defined(USE_RATIONAL_LONG_LONG)
Mat inverse(Mat A) {
    // Implementazione della Gaussian Elimination
    // cout << "A = " << endl << A << endl;
    ll n = A.rows();
    Mat B = Mat::Identity(n, n);
    // cout << "B = " << endl << B << endl;
    for (ll i = 0; i < n; i++) {
        BaseType pivot = A(i, i);
        if (pivot == 0) {
            // Trova il pivot
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

            // swap righe
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
// #else
// Mat inverse(Mat A) {
//     return A.inverse();
// }
// #endif

// Sherman-Morrison
Mat inverse_update(Mat Ainv, Vec u, Vec v) {
    // implementazione naive 
    Vec Ainvu = Ainv * u;
    BaseType alpha = 1 / (1 + v.dot(Ainvu));
    Mat vtAinv = v.transpose() * Ainv;
    Mat AinvuvAinv = Ainvu * vtAinv;
    return Ainv - alpha * AinvuvAinv;
}

pSimplex_tuple
pSimplex(Vec c, Mat A, Vec b, osl base, ll max_iter) {
    auto start = high_resolution_clock::now();

    ll iter = 0; // iteration counter
    ll nVar = A.cols(); // numero di variabili
    ll nCon = A.rows(); // numero di vincoli

    Mat B(nVar, nVar); // matrice di base
    // Mat N(nCon - nVar, nVar); // matrice non di base (Non utilizzata)
    Mat Binv(nVar, nVar); // inversa di B
    Mat W(nVar, nVar); // W = -Binv

    Vec bB(nVar); // b di base
    Vec xB(nVar); // x di base
    Vec yB(nCon); // y di base
    
    Vec Wh(nCon); // colonna h di W
    Vec AWh(nCon); // A*Wh

    Vec ratio(nCon - nVar); // rapporti
    Vec keyratio(nCon - nVar); // indici dei rapporti
    ll ridx; // contatore dei rapporti

    // variabili di output fObj,xB,B,yB,flag,iter
    BaseType fObj;
    // xB;
    // base;
    // yB;
    // iter;

    //  -100 generic error,
    //  -5 unbounded, 
    //  -3 linear dependent base,
    //  -2 infeasible,
    //  0 max_iter reached
    //  1 optimal

    ll h; // indice uscente
    ll k = 0; // indice entrante
    ll upindex; // indice1 per l'update dell'inversa
    ll upindex2; // indice2 per la permutazione

    bool feasible_checked = false; // flag per il primo check di feasability
    
    cout << "----- pSimplex -----" << endl;
    // cout << "c = " << endl << c.transpose() << endl;
    // cout << "-----" << endl;
    // cout << "A = " << endl << A << endl;
    // cout << "-----" << endl;
    // cout << "b = " << endl << b.transpose() << endl;
    // cout << "-----" << endl;
    // cout << "base = " << endl;
    // for(auto i : base) cout << i << " ";
    // cout << endl;
    cout << "-----" << endl;
    cout << "max_iter = " << max_iter << endl;
    cout << "-----" << endl;
    cout << "tol = " << tol << endl;
    cout << "-----" << endl;

    // controlli sulle dimentisioni
    if (c.rows() != nVar) {
        cout << "Error: c.rows() != nVar" << endl;
        return pSimplex_tuple
        (xB.dot(c), xB, base, yB, -100, iter);
    }
    if (nCon != b.rows()) {
        cout << "Error: nCon != b.rows()" << endl;
        return pSimplex_tuple
        (xB.dot(c), xB, base, yB, -100, iter);
    }
    if (nVar != (ll)base.size()) {
        // avvia il problema ausiliario
        cout << "Base not setted, using auxiliary problem..." << endl;
        pair<osl, ll> AuxAnswer = pSimplexAux(A, b, max_iter);
        base = AuxAnswer.first;
        iter = AuxAnswer.second;
        cout << "-----" << endl;
        cout << "Auxiliary problem solved in " << ((duration<double>)(high_resolution_clock::now() - start)).count() << " seconds" << endl;
        cout << "-----" << endl;
    }
    

    // iterazione principale
    for (; iter < max_iter; iter++) {
        auto start_iter = high_resolution_clock::now(); // tempo di inizio iterazione

        cout << "---- Iteration " << iter << " ----" << endl;
        
        // ll Nidx = 0;
        ll key;
        
        // update di B e bB
        for (ll i = 0; i < nCon; i++) {
            if (base.find(i) != base.end()) {
                    key = base.order_of_key(i);
                    bB(key) = b(i);
                    B.row(key) = A.row(i);
            } 
            // else {
            //     N.row(Nidx++) = A.row(i);
            // }
        }

        // se e' la prima iterazione calcola l'inversa
        if (!feasible_checked) {
            cout << "INVERTING B" << endl;
            Binv = inverse(B);
            cout << "INVERTED B" << endl;
            cout << "-----" << endl;
        } 

        xB = Binv * bB; 
        yB = c.transpose() * Binv;

        // controlla se la base e' ammissibile
        if (!feasible_checked) {
            feasible_checked = true;
            cout << "Check if it's feasible" << endl;
            cout << "tol = " << tol << endl;
            for (ll i = 0; i < b.rows(); i++) {
                if (get_sign(b(i) - A.row(i).dot(xB)) == -1) {
                    cout << "++++++++++++++++++++++++++++++" << endl;
                    cout << "Infeasible base" << endl;
                    cout << "++++++++++++++++++++++++++++++" << endl;
                    return pSimplex_tuple
                    (xB.dot(c), xB, base, yB, -2, iter);
                }
            }
        }


        // controlla se e' stato raggiunto l'ottimo
        // cout << "Check if yB >= 0" << endl; 
        bool yB_geq_0 = true;
        for (ll i = 0; i < yB.rows(); i++) {
            if (get_sign(yB(i)) == -1) {
                yB_geq_0 = false;
                break;
            }
        }
        if (yB_geq_0) {
            // raggiunto l'ottimo
            cout << "++++++++++++++++++++++++++++++" << endl;
            cout << "Optimal solution found" << endl;
            cout << "++++++++++++++++++++++++++++++" << endl;
            // cout << "xB = " << endl << xB.transpose() << endl;
            // cout << "yB = " << endl << yB.transpose() << endl;
            // cout << "fObj = " << xB.dot(c) << endl;
            // cout << "iter = " << iter << endl;
            // cout << "++++++++++++++++++++++++++++++" << endl;
            cout << "--------------------------" << endl;
            cout << "Completed in " << ((duration<double>)(high_resolution_clock::now() - start)).count() << " seconds" << endl;
            cout << "--------------------------" << endl;
            return pSimplex_tuple
            (xB.dot(c), xB, base, yB, 1, iter);
        }

        // Trova l'indice uscente
        for (ll i = 0; i < yB.rows(); i++) {
            if (get_sign(yB(i)) == -1) {
                h = *base.find_by_order(i);
                break;
            }
        }


        cout << "-------" << endl;
        cout << "h = " << h << endl;
        cout << "-------" << endl;

        W = -Binv;
        // cout << "W = " << endl << W << endl;
        // cout << "-----" << endl;
        Wh = W.col(base.order_of_key(h));
        // cout << "Wh = " << endl << Wh.transpose() << endl;
        // cout << "-----" << endl;
        AWh = A * Wh;
        // cout << "AWh = " << endl << AWh.transpose() << endl;
        // cout << "-----" << endl;

        cout << "Check if unbounded" << endl;
        bool unbounded = true;
        for (ll i = 0; i < AWh.rows(); i++) {
            if (get_sign(AWh(i)) == 1) {
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
        // rapporti
        // trova l'indice entrante
        BaseType theta = -1;
        ridx = 0;
        for (int i = 0; i < nCon; i++) {
            if (get_sign(AWh(i)) == 1) {
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
        // cout << "-------" << endl;
        // cout << "ratio = " << endl << ratio.transpose() << endl;
        // cout << "-------" << endl;
        // cout << "theta = " << theta << endl;
        // cout << "-------" << endl;

        // cout << "-----" << endl;
        // cout << "xB = " << endl << xB.transpose() << endl;
        // cout << "-----" << endl;
        // cout << "yB = " << endl << yB.transpose() << endl;

        cout << "k = " << k << endl;
        cout << "-------" << endl;

        // Update dell'inversa
        cout << "Updating Inverse" << endl;
        cout << "-------" << endl;
        upindex = base.order_of_key(h);
        Vec e = Vec::Zero(nVar);
        e(upindex) = 1;
        Vec u = e;
        Vec v = A.row(k) - A.row(h);
        Binv = inverse_update(Binv, e, v);

        base.erase(h);
        base.insert(k);

        cout << "-----" << endl;
        cout << "base = " << endl;
        for(auto i : base) cout << i << " ";
        cout << endl;
        cout << "-----" << endl;

        // Permutation matrix
        upindex2 = base.order_of_key(k);
        // if (upindex > upindex2) {
        //     // swap values
        //     ll temp = upindex;
        //     upindex = upindex2;
        //     upindex2 = temp;
        // }
        // cout << "upindex = " << upindex << endl;
        // cout << "upindex2 = " << upindex2 << endl;
        // cout << "Permuting Binv" << endl;
        // cout << "-------" << endl;
        
        // soluzione naive
        // Mat I = Mat::Identity(nVar, nVar);
        // Mat P = I;
        // // shift the columns from upindex to upindex2 to the left
        // if (upindex2 > upindex) {
        //     P.block(upindex, upindex + 1, upindex2 - upindex, upindex2 - upindex) = I.block(upindex, upindex, upindex2 - upindex, upindex2 - upindex);
        // }
        // else {
        //     P.block(upindex2 + 1, upindex2, upindex - upindex2, upindex - upindex2) = I.block(upindex2, upindex2, upindex - upindex2, upindex - upindex2);
        // }
        // P.row(upindex2) = I.row(upindex);
        // P.col(upindex) = I.col(upindex2);

        // // cout << "P = " << endl << P.transpose() << endl;
        // // cout << "P = " << endl << P << endl;

        // cout << "-----" << endl;
        // Binv = Binv * P.transpose();
        // cout << "upindex = " << upindex << endl;
        // cout << "upindex2 = " << upindex2 << endl;
        // cout << "Binv = " << endl << Binv << endl;


        // portare upindex in upindex2 shiftando a destra o sinistra in base a se e' maggiore o minore
        if (upindex > upindex2) {
            Vec temp = Binv.col(upindex);
            Mat temp2 = Binv.block(0, upindex2, Binv.rows(), upindex - upindex2);
            Binv.block(0, upindex2 + 1, Binv.rows(), upindex - upindex2) = temp2;
            Binv.col(upindex2) = temp;
        }
        else {
            Vec temp = Binv.col(upindex);
            Binv.block(0, upindex, Binv.rows(), upindex2 - upindex) = Binv.block(0, upindex + 1, Binv.rows(), upindex2 - upindex);
            Binv.col(upindex2) = temp;
        }
        // cout << "Binv = " << endl << Binv << endl;

        cout << "fObj = " << xB.dot(c) << endl;
        #if defined(USE_MPQ) || defined(USE_MPF)
            cout << "double = " << xB.dot(c).get_d() << endl;
        #else
            cout << "double = " << (double)xB.dot(c) << endl;
        #endif
        cout << "-----" << endl;
        cout << "Iteration " << iter << " completed in " << ((duration<double>)(high_resolution_clock::now() - start_iter)).count() << " seconds" << endl;
        cout << "-----" << endl;

    }
    cout << "++++++++++++++++++++++++++++++" << endl;
    cout << "Max iter reached" << endl;
    cout << "++++++++++++++++++++++++++++++" << endl;
    cout << "--------------------------" << endl;
    cout << "Completed in " << ((duration<double>)(high_resolution_clock::now() - start)).count() << " seconds" << endl;
    cout << "--------------------------" << endl;
    return pSimplex_tuple
    (xB.dot(c), xB, base, yB, 0, iter);

}

pair<osl, ll> pSimplexAux(Mat A, Vec b, ll max_iter) {
    ll iter = 0;
    ll nVar = A.cols();
    ll nCon = A.rows();
    Mat B(nVar, nVar);
    Mat Binv(nVar, nVar);

    Vec bB(nVar);
    Vec xB(nVar);
    Vec yB(nCon);
    
    osl base;
    ll bIdx = 0;
    for(ll i = 0; i < nVar; i++) {
        base.insert(i);
        B.row(i) = A.row(i);
        bB(bIdx++) = b(i);
    }
    cout << "INVERTING B" << endl;
    Binv = inverse(B);
    cout << "INVERTED B" << endl;
    
    xB = Binv * bB;

    cout << "----- pSimplexAux -----" << endl;
    // cout << "A = " << endl << A << endl;
    // cout << "-----" << endl;
    // cout << "b = " << endl << b.transpose() << endl;
    // cout << "-----" << endl;
    // cout << "base = " << endl;
    // for(auto i : base) cout << i << " ";
    // cout << endl;
    // cout << "-----" << endl;
    // cout << "B = " << endl << B << endl;
    // cout << "-----" << endl;
    // cout << "xB = " << endl << xB.transpose() << endl;
    // cout << "-----" << endl;

    bool feasible = true;
    for (ll i = 0; i < b.rows(); i++) {
        if (get_sign(A.row(i).dot(xB)-b(i))  == 1) {
            // cout << "A.row(" << i << ").dot(xB) = " << A.row(i).dot(xB) << endl;
            // cout << "b(" << i << ") = " << b(i) << endl;
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
        // cout << A.row(i).dot(xB) - b(i) << endl;
        // cout << get_sign(A.row(i).dot(xB) - b(i)) << endl;
        if (get_sign(A.row(i).dot(xB) - b(i)) == 1) {
            V.insert(i);
        } else {
            U.insert(i);
        }
    }
    // cout << "-----" << endl;
    // cout << "U = " << endl;
    // for(auto i : U) cout << i << " ";
    // cout << endl;
    // cout << "-----" << endl;
    // cout << "V = " << endl;
    // for(auto i : V) cout << i << " ";
    // cout << endl;
    // cout << "-----" << endl;

    ll nConAux = nCon + (ll)V.size();
    ll nVarAux = nVar + (ll)V.size();

    Mat Aaux(nConAux, nVarAux);

    Aaux.block(0, 0, nCon, nVar) = A;

    ll Idx = 0;
    for (ll i: V) {
        Aaux(i, nVar + Idx) = -1;
        Aaux(nCon + Idx, nVar + Idx) = -1;
        Idx++;
    }

    Vec baux(nConAux);
    baux.head(nCon) = b;

    Vec cAux(nVarAux);
    cAux.tail(V.size()) = -Vec::Ones(V.size());

    osl baseAux;
    baseAux.join(base);
    baseAux.join(V);

    // cout << "-----" << endl;
    // cout << "U = " << endl;
    // for(auto i : U) cout << i << " ";
    // cout << endl;
    // cout << "-----" << endl;
    // cout << "V = " << endl;
    // for(auto i : V) cout << i << " ";
    // cout << endl;
    // cout << "-----" << endl;
    // cout << "Aaux = " << endl << Aaux << endl;
    // cout << "-----" << endl;
    // cout << "baux = " << endl << baux.transpose() << endl;
    // cout << "-----" << endl;
    // cout << "cAux = " << endl << cAux.transpose() << endl;
    // cout << "-----" << endl;
    // cout << "baseAux = " << endl;
    // for(auto i : baseAux) cout << i << " ";
    // cout << endl;
    pSimplex_tuple answer = pSimplex(cAux, Aaux, baux, baseAux, max_iter);

    if (get<4>(answer) == 1) {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "Auxiliary problem solved" << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        // rimozione delle variabili artificiali
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
            if ((ll)base.size() == nVar) {
                break;
            }
        }
        return make_pair(base, iter);
    } else {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "Auxiliary problem failed. flag = " << get<4>(answer) << endl;
        cout << "++++++++++++++++++++++++++++++" << endl;
        return make_pair(base, iter);
    }
}