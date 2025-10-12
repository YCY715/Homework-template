#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int n;
    cout << "個數：";
    cin >> n;

    vector<string> set(n);
    cout << "元素：" << endl;
    for (int i = 0; i < n; i++) cin >> set[i];

    cout << "集合的冪集：" << endl;
    int total = 1 << n;  // 2^n 個子集合

    for (int mask = 0; mask < total; mask++) {
        cout << "{";
        bool first = true;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) { // 若第 i 個 bit 為 1，就選這個元素
                if (!first) cout << ",";
                cout << set[i];
                first = false;
            }
        }
        cout << "}" << endl;
    }

    return 0;
}
