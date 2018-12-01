#include <iostream>
#include <chrono>
#include <random>
#include <vector>
using std::cout;
using std::endl;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;
using std::vector;

class n_queens {
public:
    enum optim { standard, local, random_local };
private:
    int n;
    int* x;
    optim type;
    bool* conf;
    int threshold;

    template <typename T>
    void swap(T&a, T&b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    void output() {
        for(int i = 0; i < n; i++) {
            cout << x[i];
            if(i + 1 != n)
                cout << ' ';
            else cout << endl;
        }
    }

    bool solution() {
        return conflit() == 0;
    }

    void shuffle() {
        random_device rd;
        mt19937 gen(rd());
        for(int i = 0; i < n; i++) {
            std::uniform_int_distribution<> dis(0, n - 1);
            int j = dis(gen);
            swap(x[i], x[j]);
        }
    }

    int conflit() {
        int count = 0;

        for(int i = 0; i < n; i++)
            conf[i] = false;

        for(int i = 0; i < n; i++) {
            for(int r = 1; r < n - i; r++) {
                if(x[i] == x[i + r] + r || x[i] == x[i + r] - r) {
                    conf[i] = true;
                    conf[i + r] = true;
                }
            }
        }
        for(int i = 0; i < n; i++)
            if(conf[i])
                count++;
        return count;
    }

    void local_backtrack() {
        int f = 0;
        int s = 0;
        int pref = -1;
        int pres = -1;
        int min = conflit();
        int counter = 0;
        bool solution_flag = true;
        while(!solution()) {
            bool nochange_flag = true;
            if(counter > threshold) {
                cout << "Fail\n";
                solution_flag = false;
                break;
            }
            for(int i = 0; i < n; i++) {
                for(int j = i + 1; j < n; j++) {
                    swap(x[i], x[j]);
                    int h = conflit();
                    if(h <= min && pref != i && pres != j) {
                        if(h < min)
                            nochange_flag = false;
                        min = h;
                        f = i;
                        s = j;
                        pref = f;
                        pres = s;
                    }
                    swap(x[i], x[j]);
                }
            }
            if(nochange_flag)
                counter++;
            else
                counter = 0;
            swap(x[f], x[s]);
        }
        if(solution_flag)
            output();
    }

    void random_backtrack() {
        shuffle();
        int current_h = conflit();
        while(current_h != 0) {
            bool terminal_flag = false;
            for(int i = 0; i < n; i++) {
                for(int j = i + 1; j < n; j++) {
                    swap(x[i], x[j]);
                    int h = conflit();
                    if(h < current_h) {
                        terminal_flag = true;
                        current_h = h;
                        break;
                    }
                    swap(x[i], x[j]);
                }
                if(terminal_flag)
                    break;
            }
            if(!terminal_flag) { //  trapped into local minimal
                shuffle();
                current_h = conflit();
            }
        }
        output();
    }

    bool constraint(int t) {
        for(int i = 0; i < t; i++)
            if(x[i] == x[t] || x[i] == x[t] + (t - i) || x[i] == x[t] - (t - i))
                return false;
        return true;
    }

    void standard_backtrack() {
        int t = 0;
        auto *q = new vector<int>[n];
        for(int i = 0; i < n; i++)
            q[t].emplace_back(i);
        while(t < n) {
            if(!q[t].empty()) {
                int h = q[t].front();
                q[t].erase(q[t].begin());
                x[t] = h;
                if(constraint(t)) {
                    t++;
                    if(t < n)
                        for(int i = 0; i < n; i++)
                            q[t].emplace_back(i);
                }
            }else {
                t--;
            }
        }
        output();
        delete[] q;
    }

public:
    n_queens(int n_, optim t = local) {
        n = n_;
        x = new int[n];
        conf = new bool[n];
        threshold = 100;
        type = t;
        for(int i = 0; i < n; i++)
            x[i] = i;

        auto start = system_clock::now();

        if(type == local)
            local_backtrack();
        else if(type == random_local)
            random_backtrack();
        else if(type == standard)
            standard_backtrack();

        auto end = system_clock::now();
        auto duration = duration_cast<seconds>(end - start);

        cout << n << "-queens in ";
        if(type == local)
            cout << "local search ";
        else if(type == random_local)
            cout << "random local search ";
        else if(type == standard)
            cout << "standard search ";
        cout <<  "takes " << double(duration.count()) << "s" << endl;

    }
    ~n_queens() {
        delete[] x;
        delete[] conf;
    }
};

int main() {
    n_queens t(150, n_queens::random_local);
    n_queens t2(27, n_queens::standard);
    n_queens t1(64, n_queens::local);
    return 0;
}
