#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char MAT = 'A';

void mul_mat(int len) {
    vector<vector<string>> mem(len);
    vector<string> init;
    string first_char = "";
    first_char  += MAT;
    init.emplace_back(first_char);
    mem[0] = init;
    for(int i = 1; i < len; i++) {
        for(int j = 0; j < i; j++) {
            for(string front : mem[j]) {
                for (string back : mem[i - j - 1])
                    mem[i].emplace_back("(" + front + back + ")");
            }
        }
    }
    for (string m : mem[len - 1])
        cout << m << endl;
    cout << "total " << mem[len - 1].size() << " probable plan(s)" << endl;
}



int main() {
    int len;
    cin >> len;
    mul_mat(len);
    return 0;
}
