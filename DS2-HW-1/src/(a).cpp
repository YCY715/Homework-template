#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(NULL), right(NULL) {}
};

class BST {
private:
    Node* root;
    // 取得節點高度
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        // 遞迴插入左或右子樹
        if (key < node->key)
            node->left = insert(node->left, key);
        else
            node->right = insert(node->right, key);

        //  更新高度
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

        return node;
    }

public:
    BST() { root = NULL; }

    void insert(int key) {
        root = insert(root, key);
    }

    int height() {
        return root ? root->height : 0;
    }
};

int main() {
    srand(time(0));

    cout << "n\tHeight\tRatio\n";
    // 測試不同 n
    for (int n = 100; n <= 10000; n += 100) {
        BST tree;
       // 建立隨機 BST
        for (int i = 0; i < n; i++)
            tree.insert(rand());

        int h = tree.height();
        double ratio = h / log2(n);

        cout << n << "\t" << h << "\t" << ratio << endl;
    }

    return 0;
}