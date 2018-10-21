#include <iostream>
#include <random>
#include <functional>
#include <set>
#include <chrono>

using std::cout;
using std::endl;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

template <typename Type>
void swap(Type& a, Type& b) {
    Type tmp = a;
    a = b;
    b = tmp;
}

template <typename Type>
void sort(Type *list, int len) {
    for(int i = 0; i < len - 1; i++) {
        Type min = list[i];
        for(int j = i + 1; j < len; j++) {
            if(list[j] < min) {
                swap(list[i], list[j]);
                min = list[i];
            }
        }
    }
}

template <typename Type>
int partition(Type *list, int s, int e, Type ind) {
    int i = s - 1;
    int j = e + 1;
    while(true) {
        while(list[++i] < ind && i < e);
        while(list[--j] > ind);
        if (i >= j)
            break;
        swap(list[i], list[j]);
    }
    return j;
}

template <typename Type>
Type select(Type *list, int s, int e, int k) {
//    debug
//    for(int i = 0; i < 75; i++) {
//        cout << list[i] << ' ';
//    }
//    cout << endl;

    if(e - s < 75) {
        sort(list + s, e - s);
        return list[s + k - 1];
    }
    for(int i = 0; i <= (e - s - 4) /5; i++) {
        sort(list + s + 5 * i, 5);
        swap(list[s + i], list[s + 5 * i + 2]);
    }
    Type x = select(list, s, s + (e - s - 4) / 5, (e - s - 4) / 10);
//    cout << "the middle number is " << x;  // debug
//    cout << " k is " << k << endl;  // debug
    int i = partition(list, s, e, x);
    int j = i - s + 1;
    if(k <= j)
        return select(list, s, i, k);
    else
        return select(list, i + 1, e, k - j);
}


int main() {
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    auto dice = std::bind(dis, engine);

    const int list_len = 8000;
    const int k = 83;
    int *list = new int[list_len];
    std::set<int> s;
    cout << "The length of list is " << list_len << " and k is " << k << endl;
    cout << "original: ";
    for(int i = 0; i < list_len; i++) {
        int t = dice();
        while (s.find(t) != s.end())
            t = dice();
        list[i] = t;
        s.insert(t);
        cout << list[i] << ' ';
    }
    cout << endl;

    auto start = system_clock::now();

    int result = select(list, 0, list_len - 1, k);
    cout << "result got by select is " << result << endl;

    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<  "select cost " << double(duration.count()) << "us" << endl;



    cout << "sorted: ";
    sort(list, list_len);
    for(int i = 0; i < list_len; i++) {
        cout << list[i] << ' ';
    }
    cout << endl;
    cout << "The " << k << "th item is " << list[k - 1] << endl;

    delete[] list;
    return 0;
}
