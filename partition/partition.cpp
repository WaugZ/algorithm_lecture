#include<iostream>

using namespace std;

void print(int men[], int len) {
    for (int i = 0; i < len; i++) {
        cout << men[i];
        if (i + 1 < len)
            cout << '+';
    }
    cout << endl;
}

void q_partition(int n, int m, int len, int mem[]) {
    // 当m=1时 只有一种划分方法
    if(m == 1) {
        if(n > 0) { // 当n >= 1，继续划分（以m=1的方式）
            mem[len] = 1;
            q_partition(n - 1, m, len + 1, mem);
        } else  // 当n = 0，输出
            print(mem, len); // 没有新的划分 因此len不需要+1
    } else if(m > n) // m > n时，可以转化成q(n,n)的问题
        q_partition(n, n, len, mem);
    else if(m == n) {         // 当m=n时，有将n划分成m以及q(n, n - 1)两种问题，前者划分完成需要输出
        mem[len] = m;
        print(mem, len + 1);

        q_partition(n, m - 1, len, mem); // 需要覆盖原先的划分 因此len不+1
    } else if(n > m) {        // 当n > m时， 划分成q(n, m - 1)和q(n - m, m)两个问题
        mem[len] = m;
        q_partition(n - m, m, len + 1, mem);

        q_partition(n, m - 1, len, mem); // 需要覆盖原先的划分 因此len不+1
    }

}
void partition(int n, int mem[]) {
    q_partition(n, n, 0, mem);
}

int main()
{
    int n;
    while(cin >> n && n > 0) {
        const int MAX_MEMORY = n + 1;
        // 借助一个全局数组（对于该函数）使得栈之间可以共享内存
        int *mem = new int[MAX_MEMORY];
        partition(n, mem);
        delete[] mem;
    }
    return 0;
}
