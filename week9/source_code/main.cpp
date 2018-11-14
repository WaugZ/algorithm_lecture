#include <iostream>
#include <string>

using namespace std;

const int MAX = 100;

template <typename T>
void print(T mat[MAX][MAX], int n) {
    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= n; j++) {
//            cout << "[" << i << "][" << j << "] = " << mat[i][j];
            cout << mat[i][j];
            if(j + 1 > n)
                cout << endl;
            else cout << '\t';
        }
    }
}

double OBST(const double *a, const double *b, int n, double m[MAX][MAX], int s[MAX][MAX], double w[MAX][MAX]) {
    for (int i = 0; i <= n; i++) {
        w[i + 1][i] = a[i];
        m[i + 1][i] = 0;
    }
    for (int r = 0; r < n; r++)
        for (int i = 1; i <= n - r; i++) {
            int j = i + r;
            w[i][j] = w[i][j - 1] + a[j] + b[j - 1];
            int i1 = s[i][j - 1] > i ? s[i][j - 1] : i;
            int j1 = s[i + 1][j] < j ? s[i + 1][j] : j;
                     // init: the first split is in i'
            m[i][j] = m[i][i1 - 1] + m[i1 + 1][j];
            s[i][j] = i1;
            for (int k = i1 + 1; k <= j1; k++) {
                double t = m[i][k - 1] + m[k + 1][j];
                if (t < m[i][j]) {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
            m[i][j] += w[i][j];
        }
    return m[1][n];
}

string trace_back(int s[MAX][MAX], int p, int r) {
    string res(to_string(s[p][r]));
    if(p == r)
        return res;
    if(p > r)
        return "";
    res += "(" + trace_back(s, p, s[p][r] - 1) + ")";
    res += "(" + trace_back(s, s[p][r] + 1, r) + ")";
    return res;

}

int main() {
    double m[MAX][MAX];
    int s[MAX][MAX];
    double w[MAX][MAX];
//    int n = 5;
//    double a[] = {.05, .1, .05, .05, .05, .1};
//    double b[] = {.15, .1, .05, .1, .2};
    int n = 15;
    double b[] = {.02, .03, .045, .04, .045, .025, .06, .035, .025, .045, .05, .04, .045, .055, .035};
    double a[] = {.025, .03, .035, .01, .015, .02, .025, .03, .015, .01, .02, .045, .03, .035, .02, .04};
    cout << "Average length of path: " << OBST(a, b, n, m, s, w) << endl;
    cout << "OBST: " << trace_back(s, 1, n) << endl;

    cout << "w:\n";
    print(w, n);
    cout << "m:\n";
    print(m, n);
    cout << "s:\n";
    print(s, n);

    for(int i = 1; i <= n; i++) {
        for(int j = i + 1; j <= n; j++) {
            for(int i1 = i; i1 < j; i1++) {
                for(int j1 = j; j1 <= n; j1++) {
                    cout << "w(" << i << "," << j << ")=" << w[i][j] << '\t';
                    cout << "w(" << i1 << "," << j << ")=" << w[i1][j] << '\t';
                    cout << "w(" << i << "," << j1 << ")=" << w[i][j1] << '\t';
                    cout << "w(" << i1 << "," << j1 << ")=" << w[i1][j1] << '\t';
                    if(w[i][j] + w[i1][j1] <= w[i1][j] + w[i][j1]) cout << "w quadrangle pass";
                    else cout << "w quadrangle no pass";
                    cout << '\t';
                    if(w[i1][j] <= w[i][j1]) cout << "w monotonicity pass";
                    else cout << "w monotonicity no pass";
                    cout << endl;
                    cout << "s(" << i << "," << j << ")=" << s[i][j] << '\t';
                    cout << "s(" << i1 << "," << j << ")=" << s[i1][j] << '\t';
                    cout << "s(" << i << "," << j1 << ")=" << s[i][j1] << '\t';
                    cout << "s(" << i1 << "," << j1 << ")=" << s[i1][j1] << '\t';
                    if(s[i1][j] <= s[i][j1]) cout << "s monotonicity pass";
                    else cout << "s monotonicity no pass";
                    cout << endl;
                }
            }
        }
    }
    return 0;
}