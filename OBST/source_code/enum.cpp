#include <iostream>
#include <vector>
#include <string>

using namespace std;

//  list is ascending sorted
vector<string> Btree(vector<int> list) {
    vector<string> res;
    if(list.size() == 1) {
        res.emplace_back(to_string(list.front()));
        return res;
    }
    for(vector<int>::iterator it = list.begin(); it != list.end(); it++) {
        vector<string> left = Btree(vector<int>(list.begin(), it));
        vector<string> right = Btree(vector<int>(it + 1, list.end()));
        if(!left.empty() && !right.empty()) {
            for(const string & l : left)
                for(const string & r : right)
                    res.emplace_back(to_string(*it).append("(" + l + ")").append("(" + r + ")"));
        } else if(left.empty() && !right.empty()) {
            for(const string & r : right)
                res.emplace_back(to_string(*it).append("()").append("(" + r + ")"));
        } else if(!left.empty() && right.empty()) {
            for(const string & l : left)
                    res.emplace_back(to_string(*it).append("(" + l + ")").append("()"));
        }
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    vector<int> list(n);
    for(int i = 0; i < n; i++)
        list[i] = i;
    vector<string> res = Btree(list);
    for(const string & s : res)
        cout << s << endl;
    cout << "total " << res.size() << " BStrees" << endl;

    return 0;
}
