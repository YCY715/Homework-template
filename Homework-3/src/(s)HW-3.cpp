#include <iostream>
#include <cmath>
using namespace std;

/*
 * 單向鏈結串列的節點
 * data : 儲存資料
 * link : 指向下一個節點
 */
template<class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;

    // 建構子，預設資料為 T()，link 為 nullptr
    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {}
};

/*
 * 可用節點串列，用來回收刪除後的節點
 * 以 static 指標 avail 管理所有可重複使用的節點
 */
template<class T>
class AvailableList {
public:
    static ChainNode<T>* avail;   // 指向可用節點串列的開頭

    // 取得節點：優先從 available list 取，否則 new
    static ChainNode<T>* GetNode(const T& x, ChainNode<T>* link) {
        if (avail) {
            ChainNode<T>* n = avail;
            avail = avail->link;
            n->data = x;
            n->link = link;
            return n;
        }
        return new ChainNode<T>(x, link);
    }

    // 回收節點：不 delete，丟回 available list
    static void ReturnNode(ChainNode<T>* node) {
        node->link = avail;
        avail = node;
    }
};

// static 成員初始化
template<class T>
ChainNode<T>* AvailableList<T>::avail = nullptr;

/*
 * Term
 * 多項式中的一項 (coefficient * x^exponent)
 */
class Term {
public:
    int exp;        
    double coef;    

    Term(double c = 0, int e = 0) : exp(e), coef(c) {}
};

/*
 * 使用 circular linked list 表示多項式
 * head 為 header node，head->link 指向第一項
 */
class Polynomial {
    // I/O 運算子宣告為 friend，才能存取 private 成員
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);

private:
    ChainNode<Term>* head; // circular list 的 header node

    // 清空多項式，並將節點回收到 available list
    void Clear() {
        ChainNode<Term>* cur = head->link;
        while (cur != head) {
            ChainNode<Term>* tmp = cur;
            cur = cur->link;
            AvailableList<Term>::ReturnNode(tmp);
        }
        head->link = head;
    }

public:
    // 預設建構子：建立空的 circular list
    Polynomial() {
        head = new ChainNode<Term>();
        head->link = head;
    }

    // copy constructor（深拷貝）
    Polynomial(const Polynomial& a) : Polynomial() {
        *this = a;
    }

    // 解構子：回收所有節點
    ~Polynomial() {
        Clear();
        delete head;
    }

    // 指派運算子（深拷貝）
    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        Clear();

        ChainNode<Term>* cur = a.head->link;
        ChainNode<Term>* last = head;

        while (cur != a.head) {
            last->link = AvailableList<Term>::GetNode(cur->data, head);
            last = last->link;
            cur = cur->link;
        }
        return *this;
    }

    // 新增一項到多項式（依指數由大到小排序）
    void AddTerm(double c, int e) {
        if (c == 0) return;

        ChainNode<Term>* prev = head;
        ChainNode<Term>* cur = head->link;

        // 找到插入位置
        while (cur != head && cur->data.exp > e) {
            prev = cur;
            cur = cur->link;
        }

        // 指數相同，合併係數
        if (cur != head && cur->data.exp == e) {
            cur->data.coef += c;
            // 若係數變 0，刪除節點
            if (fabs(cur->data.coef) < 1e-9) {
                prev->link = cur->link;
                AvailableList<Term>::ReturnNode(cur);
            }
        }
        // 新指數，新增節點
        else {
            prev->link = AvailableList<Term>::GetNode(Term(c, e), cur);
        }
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        ChainNode<Term>* p = head->link;
        ChainNode<Term>* q = b.head->link;

        while (p != head && q != b.head) {
            if (p->data.exp > q->data.exp) {
                c.AddTerm(p->data.coef, p->data.exp);
                p = p->link;
            }
            else if (p->data.exp < q->data.exp) {
                c.AddTerm(q->data.coef, q->data.exp);
                q = q->link;
            }
            else {
                c.AddTerm(p->data.coef + q->data.coef, p->data.exp);
                p = p->link;
                q = q->link;
            }
        }

        // 剩餘項目直接加入
        while (p != head) {
            c.AddTerm(p->data.coef, p->data.exp);
            p = p->link;
        }
        while (q != b.head) {
            c.AddTerm(q->data.coef, q->data.exp);
            q = q->link;
        }
        return c;
    }

    // 多項式減法
    Polynomial operator-(const Polynomial& b) const {
        Polynomial nb;
        ChainNode<Term>* q = b.head->link;

        // 將 b 的係數取負
        while (q != b.head) {
            nb.AddTerm(-q->data.coef, q->data.exp);
            q = q->link;
        }
        return (*this + nb);
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& b) const {
        Polynomial c;
        for (ChainNode<Term>* p = head->link; p != head; p = p->link)
            for (ChainNode<Term>* q = b.head->link; q != b.head; q = q->link)
                c.AddTerm(p->data.coef * q->data.coef,
                    p->data.exp + q->data.exp);
        return c;
    }

    // Evaluate：計算多項式在 x 時的值
    double Evaluate(double x) const {
        double sum = 0;
        for (ChainNode<Term>* p = head->link; p != head; p = p->link)
            sum += p->data.coef * pow(x, p->data.exp);
        return sum;
    }
};

// 讀入多項式（外部表示法）
istream& operator>>(istream& is, Polynomial& x) {
    int n;
    is >> n;
    x.Clear();
    for (int i = 0; i < n; i++) {
        double c; int e;
        is >> c >> e;
        x.AddTerm(c, e);
    }
    return is;
}

// 輸出多項式（指定格式）
ostream& operator<<(ostream& s, const Polynomial& x) {
    ChainNode<Term>* cur = x.head->link;
    if (cur == x.head) {
        s << 0;
        return s;
    }

    bool f = true;
    while (cur != x.head) {
        double c = cur->data.coef;
        int e = cur->data.exp;

        if (!f && c > 0) s << " + ";
        if (c < 0) s << "-";

        double ac = fabs(c);
        if (!(ac == 1 && e != 0)) s << ac;

        if (e > 0) {
            s << "x";
            if (e > 1) s << "^" << e;
        }

        f = false;
        cur = cur->link;
    }
    return s;
}


int main() {
    Polynomial A, B;

    cin >> A;
    cin >> B;

    cout << "A: " << A << endl;
    cout << "B: " << B << endl;
    cout << "A + B: " << A + B << endl;
    cout << "A - B: " << A - B << endl;
    cout << "A * B: " << A * B << endl;
    cout << "A / B = 0" << endl;
    cout << "A % B = " << A << endl;
    cout << "A(X) = " << A.Evaluate(2) << endl;

    return 0;
}
