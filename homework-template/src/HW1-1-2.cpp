#include <iostream>
#include <cmath>  
using namespace std;

int akm(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (m == 1)
        return n + 2;
    else if (m == 2)
        return 2 * n + 3;
    else if (m == 3)
        return pow(2, n + 3) - 3;
    else
        return -1;  
}

int main() {
    int m, n;
    cout << " m and n¡G" << endl;
    cin >> m >> n;
    cout << "A(" << m << ", " << n << ") = " << akm(m, n) << endl;
    return 0;
}
