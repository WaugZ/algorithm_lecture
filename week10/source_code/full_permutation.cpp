#include <iostream>
using namespace std;

void output(char *x, int n) {
    for(int i = 0; i < n; i++) {
        cout << x[i];
        if(i + 1 == n)
            cout << endl;
        else cout << ' ';
    }
}

void backtrack(char *x, int n, int t) {
    if (t == n) output(x, n);
    else
        for (int i = t; i < n; i++) {
            swap(x[t], x[i]);
            backtrack(x, n, t + 1);
            swap(x[t], x[i]);
        }
}

int main() {
    int n;
    cin >> n;
    char a = 'A';
    char* x = new char[n];
    for(int i = 0; i < n; i++)
        x[i] = a + i;
    backtrack(x, n, 0);
    delete [] x;
    return 0;
}