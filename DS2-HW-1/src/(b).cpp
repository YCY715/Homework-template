#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(NULL), right(NULL) {}
};

class BST {
private:
    Node* root;

    void insert(Node*& node, int key) {
        if (!node) {
            node = new Node(key);
            return;
        }

        if (key < node->key)
            insert(node->left, key);
        else
            insert(node->right, key);
    }

    void remove(Node*& node, int key) {
        if (!node) return;

        if (key < node->key)
            remove(node->left, key);
        else if (key > node->key)
            remove(node->right, key);
        else {
            // 找到節點

            //沒有左子樹
            if (!node->left) {
                Node* temp = node;
                node = node->right;
                delete temp;
            }
           //沒有右子樹
            else if (!node->right) {
                Node* temp = node;
                node = node->left;
                delete temp;
            }
            //有兩個子節點
            else {
                Node* temp = node->right;
                while (temp->left)
                    temp = temp->left;
                node->key = temp->key;// 用該值取代目前節點
                remove(node->right, temp->key);// 刪除原本的 successor
            }
        }
    }

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

public:
    BST() { root = NULL; }

    void insert(int key) {
        insert(root, key);
    }

    void remove(int key) {
        remove(root, key);
    }

    void print() {
        inorder(root);
        cout << endl;
    }
};

int main() {
    BST tree;
    int x;

    cout << "input number（input -1 end）：\n";
    while (cin >> x && x != -1) {
        tree.insert(x);
    }

    cout << "now BST (inorder): ";
    tree.print();

    int k;
    cout << "input deleted numbers: ";
    cin >> k;

    tree.remove(k);

    cout << "After deletion BST (inorder): ";
    tree.print();

    return 0;
}