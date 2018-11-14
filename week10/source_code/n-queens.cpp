#include <iostream>
using std::cout;
using std::endl;

const int n = 18;
bool x[n][n];

void output(bool x[n][n]) {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            if(x[i][j])
                cout << 1;
            else cout << 0;
            if(j + 1 == n)
                cout << endl;
            else cout << ' ';
        }
    for(int i = 0; i < n * 2; i++)
        cout << "-";
    cout << endl;
}

bool complete() {
    for(int i = 0; i < n; i++) {
        int count = 0;
        for(int j = 0; j < n; j++) {
            if(x[i][j])
                count++;
        }
        if(count != 1)
            return false;
    }
    return true;
}

bool constraint(int x_, int y_) {
    if(!x[x_][y_]) {
        for(int i = 0; i < x_; i++) {
            int count = 0;
            for(int j = 0; j < n; j++) {
                if(x[i][j])
                    count++;
            }
            if(count != 1)
                return false;
        }
        return true;
    }
    for(int i = 0; i < y_; i++)
        if(x[x_][i])
            return false;
    for(int i = 0; i < x_; i++)
        if(x[i][y_])
            return false;
    int i = x_;
    int j = y_;
    while(i > 0 && j > 0)
        if(x[--i][--j])
            return false;
    i = x_;
    j = y_;
    while(i > 0 && j < n - 1)
        if(x[--i][++j])
            return false;
    return true;
}

void backtrack(int x_, int y_) {
    if(x_ == n - 1 && y_ == n - 1 && complete())
        output(x);
    else {
        bool binary[] = {true, false};
        for(int i = 0; i < 2; i++) {
            x[x_][y_] = binary[i];
            if(constraint(x_, y_)) {
                if(y_ + 1 < n)
                    backtrack(x_, y_ + 1);
                else if(x_ + 1 < n)
                    backtrack(x_ + 1, (y_ + 1) % n);
            }
        }
    }
}
int main() {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            x[i][j] = false;
    backtrack(0, 0);
    return 0;
}