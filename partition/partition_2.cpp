#include <iostream>

using namespace std;

void print(int mem[], int len) {
	int i = 0;
	for (i = 0; i < len - 1; i++)
		cout << mem[i] << '+';
	cout << mem[i] << endl;
}
void q_partition(int n, int prev, int mem[], int sum=0, int len=0) {
	if(n == sum) {
		print(mem, len);
	} else {
		for (int i = prev; i > 0; i--) {
			mem[len] = i;
			sum += i;
			if(sum <= n)
				q_partition(n, i, mem, sum, len + 1);
			sum -= i;
		}
	}
}

void partition(int n, int mem[]) {
	q_partition(n, n, mem);
}

int main() {
	int n;
	while (cin >> n && n > 0) {
		int* mem = new int[n];
		partition(n, mem);
		delete[] mem;
	}
	return 0;
}
