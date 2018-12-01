#include <iostream>
#include <chrono>

using namespace std;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::hours;

#define NoEdge 0x7fffffff
#define SIZE 20 + 1

int cities[SIZE][SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 2, NoEdge, NoEdge, 6, NoEdge, NoEdge, NoEdge, 1, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, 5, NoEdge, 3, NoEdge, 2, NoEdge,
                          0, 2, 0, NoEdge, 2, NoEdge, NoEdge, 4, NoEdge, NoEdge, NoEdge, 3, NoEdge, NoEdge, 7, NoEdge, 1, NoEdge, NoEdge, NoEdge, 2,
                          0, NoEdge, NoEdge, 0, NoEdge, NoEdge, 2, NoEdge, NoEdge, 1, NoEdge, NoEdge, 1, NoEdge, 9, NoEdge, NoEdge, 4, NoEdge, 6, NoEdge,
                          0, NoEdge, 2, NoEdge, 0, 1, NoEdge, NoEdge, 4, NoEdge, 2, NoEdge, NoEdge, 4, NoEdge, NoEdge, 5, NoEdge, NoEdge, NoEdge, 8,
                          0, 6, NoEdge, NoEdge, 1, 0, NoEdge, 2, NoEdge, 6, NoEdge, NoEdge, 5, NoEdge, NoEdge, 8, NoEdge, 5, NoEdge, NoEdge, 10,
                          0, NoEdge, NoEdge, 2, NoEdge, NoEdge, 0, NoEdge, 1, NoEdge, NoEdge, 1, NoEdge, NoEdge, 5, NoEdge, 4, NoEdge, 8, NoEdge, NoEdge,
                          0, NoEdge, 4, NoEdge, NoEdge, 2, NoEdge, 0, 1, NoEdge, NoEdge, 2, NoEdge, 7, NoEdge, 5, NoEdge, NoEdge, NoEdge, 6, NoEdge,
                          0, NoEdge, NoEdge, NoEdge, 4, NoEdge, 1, 1, 0, NoEdge, 2, NoEdge, NoEdge, 3, NoEdge, NoEdge, 2, NoEdge, 10, NoEdge, NoEdge,
                          0, 1, NoEdge, 1, NoEdge, 6, NoEdge, NoEdge, NoEdge, 0, NoEdge, 3, NoEdge, NoEdge, 4, NoEdge, NoEdge, NoEdge, 6, 7, NoEdge,
                          0, NoEdge, NoEdge, NoEdge, 2, NoEdge, NoEdge, NoEdge, 2, NoEdge, 0, 1, NoEdge, 5, NoEdge, NoEdge, 3, NoEdge, NoEdge, NoEdge, 2,
                          0, NoEdge, 3, NoEdge, NoEdge, NoEdge, 1, 2, NoEdge, 3, 1, 0, 2, NoEdge, NoEdge, 8, NoEdge, NoEdge, 1, NoEdge, 3,
                          0, NoEdge, NoEdge, 1, NoEdge, 5, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, 2, 0, 1, NoEdge, NoEdge, 4, NoEdge, 5, NoEdge, 2,
                          0, NoEdge, NoEdge, NoEdge, 4, NoEdge, NoEdge, 7, 3, NoEdge, 5, NoEdge, 1, 0, 5, NoEdge, 1, NoEdge, 9, NoEdge, 8,
                          0, NoEdge, 7, 9, NoEdge, NoEdge, 5, NoEdge, NoEdge, 4, NoEdge, NoEdge, NoEdge, 5, 0, 1, NoEdge, 3, NoEdge, NoEdge, 2,
                          0, 5, NoEdge, NoEdge, NoEdge, 8, NoEdge, 5, NoEdge, NoEdge, NoEdge, 8, NoEdge, NoEdge, 1, 0, 5, NoEdge, 1, NoEdge, 6,
                          0, NoEdge, 1, NoEdge, 5, NoEdge, 4, NoEdge, 2, NoEdge, 3, NoEdge, 4, 1, NoEdge, 5, 0, 7, NoEdge, 2, NoEdge,
                          0, 3, NoEdge, 4, NoEdge, 5, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, 3, NoEdge, 7, 0, NoEdge, 1, NoEdge,
                          0, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, 8, NoEdge, 10, 6, NoEdge, 1, 5, 9, NoEdge, 1, NoEdge, NoEdge, 0, NoEdge, NoEdge,
                          0, 2, NoEdge, 6, NoEdge, NoEdge, NoEdge, 6, NoEdge, 7, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, NoEdge, 2, 1, NoEdge, 0, 4,
                          0, NoEdge, 2, NoEdge, 8, 10, NoEdge, NoEdge, NoEdge, NoEdge, 2, 3, 2, 8, 2, 6, NoEdge, NoEdge, NoEdge, 4, 0
};

template <typename T>
class TravellingSale {
public:
    enum Strategy {Nopruning, Pruning, Sort};
protected:
    int n; //  n cities
    T** a;  //  a map, edges between cities
    T* x;  // the permutation of traveler
    T bestc;  // the best cost
    T* bestx;  // the best traveling path
    T cc;  // current cost
    Strategy strategy;

    void Swap(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    T bestCost() {
        return bestc;
    }

    void bestPath() {
        for(int i = 1; i <= n; i++) {
            cout << bestx[i];
            if(i + 1 <= n)
                cout << " -> ";
            else cout << " -> " << bestx[1] << endl;
        }
    }
public:
    TravellingSale(T cities[SIZE][SIZE], Strategy s = Pruning) {
        n = SIZE - 1;
        a = new T*[n + 1];
        for(int i = 0; i <= n; i++)
            a[i] = new T[n];
        for(int i = 0; i <= n; i++)
            for(int j = 0; j <= n; j++)
                a[i][j] = cities[i][j];

        x = new T[n + 1];
        for(int i = 0; i <= n; i++)
            x[i] = i;

        strategy = s;
        if(strategy == Sort) {  // let the city with fewest edges in the front
            int* count = new int[n + 1];
            for(int i = 1; i <= n; i++)
                count[i] = 0;
            for(int i = 1; i <= n; i++) {
                for(int j = 1; j <= n; j++) {
                    if(a[i][j] != NoEdge)
                        count[i]++;
                }
            }
            for(int i = 1; i <= n; i++) {
                int min = count[i];
                int min_ind = i;
                for(int j = i + 1; j <= n; j++) {
                    if(count[j] < min) {
                        min = count[j];
                        min_ind = j;
                    }
                }
                Swap(count[i], count[min_ind]);
                Swap(x[i], x[min_ind]);
            }
            delete[] count;
        }

//        for(int i = 0; i <= n; i++)
//            cout << x[i] << ' ';
//            cout << endl;

        bestx = new T[n + 1];
        cc = 0;
        bestc = NoEdge;

        auto start = system_clock::now();

        Backtrack(1);

        auto end = system_clock::now();
//        auto duration = duration_cast<seconds>(end - start);
        if(strategy == Pruning) {
            cout << "finish with pruning ";
            auto duration = duration_cast<seconds>(end - start);
            cout <<  "costs " << double(duration.count()) << "s" << endl;
        } else if(strategy == Sort) {
            cout << "finish with soreted pruning ";
            auto duration = duration_cast<seconds>(end - start);
            cout <<  "costs " << double(duration.count()) << "s" << endl;
        } else if(strategy == Nopruning) {
            cout << "finish without pruning ";
            auto duration = duration_cast<seconds>(end - start);
            cout <<  "costs " << double(duration.count()) << "s" << endl;
        }

        cout << "The best cost is " << bestCost() << endl;
        cout << "The best path is: ";
        bestPath();

    }
    ~TravellingSale() {
        for(int i = 0; i <= n; i++)
            delete[] a[i];
        delete[] a;
        delete[] x;
        delete[] bestx;
    }
    void Backtrack(int i) {
        if (i == n) {
            if (a[x[n-1]][x[n]] != NoEdge && a[x[n]][x[1]] != NoEdge &&
                (cc + a[x[n-1]][x[n]] + a[x[n]][x[1]] < bestc || bestc == NoEdge)) {
                for (int j = 1; j <= n; j++)
                    bestx[j] = x[j];
                bestc = cc + a[x[n-1]][x[n]] + a[x[n]][x[1]];
            }
        }
        else {
            for (int j = i; j <= n; j++)
                // 是否可进入x[j]子树?
                if (a[x[i-1]][x[j]] != NoEdge) {
                    if((strategy == Pruning || strategy == Sort ) &&
                    !(cc + a[x[i-1]][x[i]] < bestc || bestc == NoEdge))
                        continue;
                    // 搜索子树
                    Swap(x[i], x[j]);
                    cc += a[x[i-1]][x[i]];
                    Backtrack(i+1);
                    cc -= a[x[i-1]][x[i]];
                    Swap(x[i], x[j]);}
        }
    }
};


int main() {
    TravellingSale<int> t(cities, TravellingSale<int>::Pruning);
//    TravellingSale<int> t_s(cities, TravellingSale<int>::Sort);
    TravellingSale<int> t_n(cities, TravellingSale<int>::Nopruning);
    return 0;
}
