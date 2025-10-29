#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;  

class Term {
    friend class Polynomial;
public:
    float coef;  
    int exp;     
};

class Polynomial {
private:
    Term* termArray;  // 動態陣列儲存非零項
    int capacity;     // 陣列容量
    int terms;        // 實際項數

public:
   //初始化容量與項數
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 複製建構子
    Polynomial(const Polynomial& p) {
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = p.termArray[i];
    }

    // 解構子
    ~Polynomial() {
        delete[] termArray;
    }

    // 加入一項多項式項
    void AddTerm(float coef, int exp) {
        if (coef == 0) return;
        // 若已有相同指數，合併
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) { // 若係數歸零則刪除此項
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        // 若容量不足，自動擴充
        if (terms == capacity) {
            capacity *= 2;
            Term* newArray = new Term[capacity];
            for (int i = 0; i < terms; i++)
                newArray[i] = termArray[i];
            delete[] termArray;
            termArray = newArray;
        }
        // 新增新項至陣列末端
        termArray[terms].coef = coef;
        termArray[terms++].exp = exp;
        
    }

    // 多項式加法
    Polynomial Add(const Polynomial& poly) {
        Polynomial result = *this;
        for (int i = 0; i < poly.terms; i++)
            result.AddTerm(poly.termArray[i].coef, poly.termArray[i].exp);
        return result;
    }

    // 多項式乘法
    Polynomial Mult(const Polynomial& poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.AddTerm(newCoef, newExp);
            }
        }
        return result;
    }

    // 多項式代入計算
    float Eval(float f) {
        float sum = 0;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(f, termArray[i].exp);
        return sum;
    }

    // 輸入運算子多載 (使用者輸入多項式）
    friend istream& operator>>(istream& in, Polynomial& p) {
        cout << "Enter number of terms: ";
        in >> p.terms;
        if (p.terms > p.capacity) {
            delete[] p.termArray;
            p.capacity = p.terms;
            p.termArray = new Term[p.capacity];
        }
        for (int i = 0; i < p.terms; i++) {
            cout << "Enter coefficient and exponent: ";
            in >> p.termArray[i].coef >> p.termArray[i].exp;
        }
        return in;
    }

    //輸出運算子多載（正負號與排序）
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms == 0) {
            out << "0";
            return out;
        }

        // 建立副本以進行排序
        Term* sorted = new Term[p.terms];
        for (int i = 0; i < p.terms; i++) sorted[i] = p.termArray[i];

        for (int i = 0; i < p.terms; i++) {
            float c = sorted[i].coef;
            int e = sorted[i].exp;

            if (i == 0) {
                // 第一項只顯示負號
                if (c < 0) out << "-";
            }
            else {
                // 之後的項印出 + 或 - 
                out << (c < 0 ? " - " : " + ");
            }

            c = fabs(c);
            if (e == 0)
                out << c;
            else if (e == 1)
                out << c << "x";
            else
                out << c << "x^" << e;
        }

        delete[] sorted;
        return out;
    }
};


int main() {
    Polynomial p1, p2;

    cout << "Enter first polynomial:\n";
    cin >> p1;
    cout << "Enter second polynomial:\n";
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);// 加法結果
    Polynomial prod = p1.Mult(p2);// 乘法結果

    cout << "\nP1 + P2 = " << sum << endl;
    cout << "P1 * P2 = " << prod << endl;

    float x;
    cout << "\nEnter a value of x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}
