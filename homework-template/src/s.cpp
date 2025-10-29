#include <iostream>
#include <vector>
using namespace std;

struct Term {
    float coef;
    int exp;
};

class Polynomial {
private:
    vector<Term> terms;

public:
    void newTerm(float c, int e) {
        Term t{ c, e };
        terms.push_back(t);
    }

    Polynomial Add(const Polynomial& b) {
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms.size() && j < b.terms.size()) {
            if (terms[i].exp == b.terms[j].exp) {
                float sum = terms[i].coef + b.terms[j].coef;
                if (sum != 0) result.newTerm(sum, terms[i].exp);
                i++; j++;
            }
            else if (terms[i].exp > b.terms[j].exp) {
                result.newTerm(terms[i].coef, terms[i].exp);
                i++;
            }
            else {
                result.newTerm(b.terms[j].coef, b.terms[j].exp);
                j++;
            }
        }
        while (i < terms.size()) result.newTerm(terms[i].coef, terms[i].exp), i++;
        while (j < b.terms.size()) result.newTerm(b.terms[j].coef, b.terms[j].exp), j++;
        return result;
    }

    friend istream& operator>>(istream& in, Polynomial& p) {
        int n; in >> n;
        while (n--) {
            float c; int e;
            in >> c >> e;
            p.newTerm(c, e);
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        for (int i = 0; i < p.terms.size(); ++i) {
            if (i > 0) out << " + ";
            out << p.terms[i].coef << "X^" << p.terms[i].exp;
        }
        return out;
    }
};

int main() {
    Polynomial A, B, C;
    cin >> A >> B;
    C = A.Add(B);
    cout << C << endl;
}
