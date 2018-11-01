#include <iostream>
#include <string>

using namespace std;

const int MAX = 100;

double OBST(const double *a, const double *b, int n, double m[MAX][MAX], int s[MAX][MAX], double w[MAX][MAX]) {
    for (int i = 0; i <= n; i++) {
        w[i + 1][i] = a[i];
        m[i + 1][i] = 0;
    }
    for (int r = 0; r < n; r++)
        for (int i = 1; i <= n - r; i++) {
            int j = i + r;
            w[i][j] = w[i][j - 1] + a[j] + b[j - 1];
            m[i][j] = 1e300;
//            s[i][j] = i;
            for (int k = i; k <= j; k++) {
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
    return 0;
}