#include<iostream>
#include <string>

using namespace std;

const int MAX = 100;
const string MAT = "A";

void matrixChain(const int *p, int n, int m[MAX][MAX], int s[MAX][MAX]) {
    for (int i = 1; i <= n; i++)
        m[i][i] = 0;
    for (int r = 2; r <= n; r++)
        for (int i = 1; i <= n - r + 1; i++) {
            int j = i + r - 1;
            m[i][j] = m[i + 1][j] + p[i - 1] * p[i] * p[j];  // + m[i][i]
            s[i][j] = i;
            for (int k = i + 1; k < j; k++) {
                int t = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (t < m[i][j]) {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
        }
}

string traceback(int i, int j, int s[MAX][MAX]) {

    if (i == j)
        return MAT;

    string front = traceback(i, s[i][j], s);
    string back = traceback(s[i][j] + 1, j, s);
    return "(" + front + back + ")";
}

int main() {
    int p[MAX + 1], memo[MAX][MAX], path[MAX][MAX];
    int n = 0;
    int rc;
    while(cin >> rc && rc > 0) {
        p[n++] = rc;
    }
    n -= 1;
    matrixChain(p, n, memo, path);

    cout << traceback(1, n, path) << endl;
    cout << memo[1][n] << endl;
    return 0;
}