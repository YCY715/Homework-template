#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int n;
    cout << "�ӼơG";
    cin >> n;

    vector<string> set(n);
    cout << "�����G" << endl;
    for (int i = 0; i < n; i++) cin >> set[i];

    cout << "���X�������G" << endl;
    int total = 1 << n;  // 2^n �Ӥl���X

    for (int mask = 0; mask < total; mask++) {
        cout << "{";
        bool first = true;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) { // �Y�� i �� bit �� 1�A�N��o�Ӥ���
                if (!first) cout << ",";
                cout << set[i];
                first = false;
            }
        }
        cout << "}" << endl;
    }

    return 0;
}
