#include <iostream>
using namespace std;

int Ak(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return Ak(m - 1, 1);
    else
        return Ak(m - 1, Ak(m, n - 1));
}

int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;

    cout << "A(" << m << "," << n << ") = " << Ak(m, n) << endl;

    return 0;
}
