#include "Simplex.h"

// void test() {
//     cout << "Hello " << 10/2_r << endl;
//     Mat m(3,3);
//     m << 1/5_r, 10/2_r, 3, 4, 5, 6, 7, 8, 9;
//     m = 2*m;
//     m(0,1) = 1/5_r;

//     cout << m << endl;
// }

void pSimplex(Vec c, Mat A, Vec b, osl base, ll max_iter) {
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
    ll theta;
    
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
        return;
    }
    if (A.rows() != b.size()) {
        cout << "Error: A.rows() != b.size()" << endl;
        return;
    }
    if (A.cols() != base.size()) {
        cout << "Error: A.cols() != base.size()" << endl;
        return;
    }


    

    for (ll i = 0; i < max_iter; i++) {
        
        cout << "---- Iteration " << iter << " ----" << endl;
        iter++;

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

        cout << "-----" << endl;
        cout << "B = " << endl << B << endl;
        cout << "-----" << endl;
        cout << "N = " << endl << N << endl;
        cout << "-----" << endl;
        cout << "bB = " << endl << bB.transpose() << endl;
        cout << "-----" << endl;

        if (B.determinant() == 0) {
            cout << "Error: Linear dependent base" << endl;
            return;
        }
        Binv = B.inverse();
        xB = Binv * bB; 
        yB = c.transpose() * Binv;


        cout << "B = " << endl << B << endl;
        cout << "-----" << endl;
        cout << "N = " << endl << N << endl;
        cout << "-----" << endl;
        cout << "bB = " << endl << bB.transpose() << endl;
        cout << "-----" << endl;
        cout << "xB = " << endl << xB.transpose() << endl;
        cout << "-----" << endl;
        cout << "yB = " << endl << yB.transpose() << endl;
        cout << "-----" << endl;

        cout << "Check if yB >= 0" << endl;
        bool yB_geq_0 = true;
        for (ll i = 0; i < yB.size(); i++) {
            if (yB(i) < 0) {
                yB_geq_0 = false;
                break;
            }
        }
        if (yB_geq_0) {
            cout << "Optimal solution found" << endl;
            cout << "xB = " << endl << xB.transpose() << endl;
            cout << "yB = " << endl << yB.transpose() << endl;
            return;
        }

        // Find the outgoing index
        ll h;
        for (ll i = 0; i < yB.size(); i++) {
            if (yB(i) < 0) {
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
            if (AWh(i) > 0) {
                unbounded = false;
                break;
            }
        }

        if (unbounded) {
            cout << "Unbounded solution" << endl;
            return;
        }

        // ratios
        // find the incoming index
        ll k = 0;
        ridx = 0;
        theta = -1;

        for (ll i = 0; i < A.rows(); i++) {
            if (AWh(i) > 0) {
                // cout << b(i) << " - " << A.row(i).dot(xB) << " / " << AWh(i) << " = " << (b(i) - A.row(i).dot(xB)) / AWh(i) << endl;
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
        cout << "k = " << k << endl;
        cout << "-------" << endl;

        base.erase(h);
        base.insert(k);

        cout << "base = " << endl;
        for(auto i : base) cout << i << " ";
        cout << endl;

    }

}