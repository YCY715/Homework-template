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
    Term* termArray;  // �ʺA�}�C�x�s�D�s��
    int capacity;     // �}�C�e�q
    int terms;        // ��ڶ���

public:
   //��l�Ʈe�q�P����
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // �ƻs�غc�l
    Polynomial(const Polynomial& p) {
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = p.termArray[i];
    }

    // �Ѻc�l
    ~Polynomial() {
        delete[] termArray;
    }

    // �[�J�@���h������
    void AddTerm(float coef, int exp) {
        if (coef == 0) return;
        // �Y�w���ۦP���ơA�X��
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) { // �Y�Y���k�s�h�R������
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        // �Y�e�q�����A�۰��X�R
        if (terms == capacity) {
            capacity *= 2;
            Term* newArray = new Term[capacity];
            for (int i = 0; i < terms; i++)
                newArray[i] = termArray[i];
            delete[] termArray;
            termArray = newArray;
        }
        // �s�W�s���ܰ}�C����
        termArray[terms].coef = coef;
        termArray[terms++].exp = exp;
        
    }

    // �h�����[�k
    Polynomial Add(const Polynomial& poly) {
        Polynomial result = *this;
        for (int i = 0; i < poly.terms; i++)
            result.AddTerm(poly.termArray[i].coef, poly.termArray[i].exp);
        return result;
    }

    // �h�������k
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

    // �h�����N�J�p��
    float Eval(float f) {
        float sum = 0;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(f, termArray[i].exp);
        return sum;
    }

    // ��J�B��l�h�� (�ϥΪ̿�J�h�����^
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

    //��X�B��l�h���]���t���P�Ƨǡ^
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms == 0) {
            out << "0";
            return out;
        }

        // �إ߰ƥ��H�i��Ƨ�
        Term* sorted = new Term[p.terms];
        for (int i = 0; i < p.terms; i++) sorted[i] = p.termArray[i];

        for (int i = 0; i < p.terms; i++) {
            float c = sorted[i].coef;
            int e = sorted[i].exp;

            if (i == 0) {
                // �Ĥ@���u��ܭt��
                if (c < 0) out << "-";
            }
            else {
                // ���᪺���L�X + �� - 
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

    Polynomial sum = p1.Add(p2);// �[�k���G
    Polynomial prod = p1.Mult(p2);// ���k���G

    cout << "\nP1 + P2 = " << sum << endl;
    cout << "P1 * P2 = " << prod << endl;

    float x;
    cout << "\nEnter a value of x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}
