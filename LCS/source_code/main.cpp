#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX = 1000;

void LCSLength(int m, int n, const char *x, const char *y, int c[MAX][MAX], int s[MAX][MAX]) {
    int i, j;
    for (i = 1; i <= m; i++)
        c[i][0] = 0;
    for (i = 1; i <= n; i++)
        c[0][i] = 0;
    for (i = 1; i <= m; i++)
        for (j = 1; j <= n; j++) {
            if (x[i - 1] == y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                s[i][j] = 1;
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                s[i][j] = 2;
            } else {
                c[i][j] = c[i][j - 1];
                s[i][j] = 3;
            }
        }
}

string trace_back(int m, int n, const char* x, int s[MAX][MAX]) {
    string res;
    while(m != 0 && n != 0) {
        if(s[m][n] == 1) {
            res += x[m - 1];
            m--;
            n--;
        }
        else if(s[m][n] == 2)
            m--;
        else if(s[m][n] == 3)
            n--;
    }
    reverse(res.begin(), res.end());
    return res;
}

//void LCSLength(int m, int n, char *x,char *y,int c[MAX][MAX]) {
//    int i,j;
//    for(i=1;i<=m;i++)
//        c[i][0]= 0;
//    for(i=1;i<=n;i++)
//        c[0][i]= 0;
//    for(i=1;i<=m;i++)
//        for(j=1;j<=n;j++)  {
//            if (x[i - 1]==y[j - 1]) {
//                c[i][j]= c[i-1][j-1]+1;
//            }
//            else   if (c[i-1][ j]>=c[i][j-1]) {
//                c[i][j]= c[i-1][j];
//            }
//            else {
//                c[i][j]= c[i][j-1];
//            }
//        }
//}

string trace_back(int m, int n, const char* x, const char* y, int c[MAX][MAX]) {
    string res;
    while(c[m][n] != 0) {
        if(x[m - 1] == y[n - 1]) {
            res += x[m - 1];
            m--;
            n--;
        } else if(c[m - 1][n] == c[m][n])
            m--;
        else if(c[m][n - 1] == c[m][n])
            n--;
    }
    reverse(res.begin(), res.end());
    return res;
}


int main() {
    char x[MAX];
    char y[MAX];
    int memo[MAX][MAX];
    int s[MAX][MAX];
    string strx;
    string stry;
    cin >> strx >> stry;
    strcpy(x, strx.c_str());
    strcpy(y, stry.c_str());

    LCSLength(strx.size(), stry.size(), x, y, memo, s);
    cout << "Length of longest common sequence: " << memo[strx.size()][stry.size()] << endl;
    cout << "The sequence get by s is " << trace_back(strx.size(), stry.size(), x, s) << endl;
    cout << "The sequence get by c is " << trace_back(strx.size(), stry.size(), x, y, memo) << endl;
    return 0;
}
