#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::vector;
using std::sort;

class n_queens {
public:
    enum optim { standard, local, random_local, SA, GA };
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

    void output(vector<int> &a) {
        for(int i = 0; i < n; i++) {
            cout << a[i];
            if(i + 1 != n)
                cout << ' ';
            else cout << endl;
        }
    }

    bool solution() {
        return conflit() == 0;
    }

    template <typename T>
    void shuffle(T* a, int len) {
        random_device rd;
        mt19937 gen(rd());
        for(int i = 0; i < len; i++) {
            uniform_int_distribution<> dis(0, len - 1);
            int j = dis(gen);
            swap(a[i], a[j]);
        }
    }

    void shuffle(vector<int> &a, int len) {
        random_device rd;
        mt19937 gen(rd());
        for(int i = 0; i < len; i++) {
            uniform_int_distribution<> dis(0, len - 1);
            int j = dis(gen);
            swap(a[i], a[j]);
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

    int conflit(vector<int> a) {
        int count = 0;
        bool* cof = new bool[n];

        for(int i = 0; i < n; i++)
            cof[i] = false;

        for(int i = 0; i < n; i++) {
            for(int r = 1; r < n - i; r++) {
                if(a[i] == a[i + r] + r || a[i] == a[i + r] - r) {
                    cof[i] = true;
                    cof[i + r] = true;
                }
            }
        }
        for(int i = 0; i < n; i++)
            if(cof[i])
                count++;
        delete[] cof;
        return count;
    }

    template <typename T>
    bool in_list(vector<T>& v, T u) {
        for(auto i = v.begin(); i < v.end(); i++)
            if(*i == u) {
//                v.erase(i);
                return true;
            }
        return false;
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
        shuffle(x, n);
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
                shuffle(x, n);
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

    void simulate_anneal() {
        // initial
        shuffle(x, n);
        int k = 0;
        double t0 = pow(n, 3. / 2);

        random_device rd;
        mt19937 gen(rd());
        double t = t0;
        int blance = 0;
        int h = conflit();
        while(h != 0) {
            double t_threshold = 2. ;
            if(blance > t_threshold) {
                k++;
                t *= .99;
                blance = 0;
            } else {
                uniform_int_distribution<> dis(0, n - 1);
                int i = dis(gen);
                int j = dis(gen);
                swap(x[i], x[j]);
                int current_h = conflit();
                double p = exp(-(current_h - h) / t);  //  easy to find if current_h <= h, p >= 1
                uniform_real_distribution<> dice(0, 1);
                if(p > dice(gen)) {
                    h = current_h;
                } else {
                    swap(x[i], x[j]);  // do not accept this state
                }
                blance++;
            }
        }
        cout << "temperature dropped " << k << " times\n";
        output();
    }

    // 保留原排序，只对index排序
    template <typename T>
    vector<size_t> sort_indexes(const vector<T> &v) {

        // 初始化索引向量
        vector<size_t> idx(v.size());
        //使用iota对向量赋0~？的连续值
        iota(idx.begin(), idx.end(), 0);

        // 通过比较v的值对索引idx进行排序
        sort(idx.begin(), idx.end(),
             [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
        return idx;
    }

    void genetic_algorithm() {
        // configure
        int len = n; // length of codes
        int top = n - 1; // the top number according to #of codes
        unsigned max_iter = 0xffff;  // max number of iteration
        unsigned m = 252;  // the number of one gener
        double p_c = 1.;  //  exchange
        double p_m = .01;  // variation

        // init
        random_device rd;
        mt19937 gen(rd());

        int iter = 0;
        bool solution_flag = false;
        vector<vector<int>> group(m);
        vector<int> choose(m);
        vector<int> fs(m);
        vector<double> prob(m);
        // initialize the 0 generation
        double sum_f = 0;
        for(int i = 0; i < m; i++) {
            vector<int> chese(len);
            for(int j = 0; j < len; j++)
                chese[j] = j;
            shuffle(chese, len);
            group[i] = chese;
            fs[i] = len - conflit(chese);
            sum_f += fs[i];
        }
        for(int i = 0; i < m; i++) {
            prob[i] = fs[i] / (sum_f + 1e-20);
        }

        // debug
//        for(int i = 0; i < m; i++)
//            cout << prob[i] << ' ';
//        cout << endl;

        // main algorithm
        while(true) {
            if(iter == max_iter || solution_flag)
                break;

            int left = m;
            for(int i = 0; i < m; i++) {
                choose[i] = int(prob[i] * m);
                prob[i] = prob[i] * m - choose[i];
                left -= choose[i];
            }
            vector<size_t> top_k = sort_indexes(prob);
            while(left > 0) {
                choose[top_k[0]]++;
                top_k.erase(top_k.begin());
                left--;
            }

            //  new general
            vector<vector<int>> new_group(m);
            int choose_ind = 0;
            for(int i = 0; i < m; i++) {
                if(choose[choose_ind] > 0) {
                    new_group[i] = group[choose_ind];
                    choose[choose_ind]--;
                } else {
                    choose_ind++;
                    i--;
                }
            }

            //  chromosome exchange and variation
            uniform_real_distribution<> dis(0, 1);
            uniform_int_distribution<> dice(0, top);
            for(int i = 0; i < m; i += 2) {
                vector<int> child_0 = new_group[i];
                vector<int> child_1 = new_group[i+1];
                //  exchange
                if(dis(gen) < p_c) {
                    int offset = dice(gen);
                    int ex_len = len - offset;
                    vector<int> exchange_0(ex_len);
                    vector<int> exchange_1(ex_len);
                    int j;
                    for(j = 0; j < ex_len; j++) {
                        exchange_0[j] = child_0[offset + j];
                    }
                    j = 0;
                    for(int k = 0; k < len; k++) {
                        if(in_list(exchange_0, child_1[k])) {
                            exchange_1[j] = child_1[k];
                            child_1[k] = exchange_0[j];
                            j++;
                        }
                        if(j == ex_len)
                            break;
                    }
                    for(j = 0; j < ex_len; j++) {
                        child_0[offset + j] = exchange_1[j];
                    }
                }
                // variation
                if(dis(gen) < p_m) {
                    int a = dice(gen);
                    int b = dice(gen);
                    swap(child_0[a], child_0[b]);
                }
                if(dis(gen) < p_m) {
                    int a = dice(gen);
                    int b = dice(gen);
                    swap(child_1[a], child_1[b]);
                }
                group[i] = child_0;
                group[i + 1] = child_1;
            }

            sum_f = 0;
            for(int i = 0; i < m; i++) {
                fs[i] = len - conflit(group[i]);
                if(fs[i] == len) {
                    solution_flag = true;
                    output(group[i]);
                    break;
                }
                sum_f += fs[i];
            }

            for(int i = 0; i < m; i++) {
                prob[i] = fs[i] / sum_f;
            }

            iter++;
        }
        if(!solution_flag)
            cout << "No solution\n";

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
        else if(type == SA)
            simulate_anneal();
        else if(type == GA)
            genetic_algorithm();
        else
            throw "No such type";

        auto end = system_clock::now();
        auto duration = duration_cast<seconds>(end - start);

        cout << n << "-queens in ";
        if(type == local)
            cout << "local search ";
        else if(type == random_local)
            cout << "random local search ";
        else if(type == standard)
            cout << "standard search ";
        else if(type == SA)
            cout << "simulate anneal ";
        else if(type == GA)
            cout << "genetic algorithm ";
        cout <<  "takes " << double(duration.count()) << "s" << endl;

    }
    ~n_queens() {
        delete[] x;
        delete[] conf;
    }
};

int main() {
    n_queens t(100, n_queens::GA);
//    n_queens t4(1000, n_queens::SA);
//    n_queens t3(300, n_queens::random_local);
//    n_queens t2(27, n_queens::standard);
//    n_queens t1(64, n_queens::local);
    return 0;
}
