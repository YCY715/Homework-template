
# 41343137

作業1

## 解題說明

(a) 高度實驗分析
本題要求建立一棵初始為空的二元搜尋樹（BST），並進行 n 次隨機插入。
透過亂數產生器產生插入值，使樹的結構具有隨機性。
接著計算該 BST 的高度（height），並計算比值：
height / log₂(n)
藉此觀察 BST 的高度是否符合 O(log n) 的成長趨勢。

(b) 刪除節點操作
實作一個函式，從 BST 中刪除指定鍵值 k 的節點。
刪除時需考慮三種情況：
1.葉節點（無子節點） → 直接刪除
2.只有一個子節點 → 用子節點取代
3.有兩個子節點 → 使用「右子樹最小值（inorder successor）」取代
### 舉例
a部分
計算高度
高度定義為：
👉 從根節點到最深葉節點的層數
最長路徑：50 → 30 → 20
👉 高度 = 3
計算比值

假設 n = 5：
log₂(5) ≈ 2.32
height = 3
此例中：height / log₂(n) ≈ 3 / 2.32 ≈ 1.29

目的

透過大量隨機資料（n = 100 ~ 10000）觀察：
👉
height / log₂(n) 是否接近常數（約 2）
若成立，表示 BST 高度 ≈ O(log n)

b部分
情況 1：葉節點（無子節點）
    
         50
        /
      30

刪除30

50
直接刪除即可
情況 2：只有一個子節點


       50
      /
    30
     \
      40

 刪除 30：     

       50
      /
    40


用子節點（40）取代


情況 3：有兩個子節點

           50
          /  \
        30    70
       / \   / \
      20 40 60 80

找右子樹最小值 → 60
用 60 取代 50
刪除原本的 60

結果

          60
         /  \
       30    70
      / \     \
     20 40     80


## 程式實作

以下為主要程式碼：
(a) 插入與高度維護
```cpp
Node* insert(Node* node, int key) {
    if (!node) return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    // 更新高度（避免重算整棵樹）
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    return node;
}

```
說明：
此方法將高度儲存在節點中，使得查詢高度時間複雜度從 O(n) 降為 O(1)。

(b) 刪除節點核心
```cpp
void remove(Node*& node, int key) {
    if (!node) return;

    if (key < node->key)
        remove(node->left, key);
    else if (key > node->key)
        remove(node->right, key);
    else {
        if (!node->left) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else if (!node->right) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            Node* temp = node->right;
            while (temp->left)
                temp = temp->left;

            node->key = temp->key;
            remove(node->right, temp->key);
        }
    }
}

```
## 效能分析

| 操作          | 時間複雜度    | 空間複雜度        | 說明          |
| ----------- | -------- | ------------ | ----------- |
| `AddTerm()` | O(n)     | O(1)         | 需檢查是否有相同指數  |
| `Add()`     | O(n + m) | O(max(n, m)) | 對兩多項式逐項相加   |
| `Mult()`    | O(n × m) | O(n + m)     | 每項相乘再合併同類項  |
| `Eval()`    | O(n)     | O(1)         | 依次計算每項次方和加總 |


## 測試與驗證

### 測試案例

| 輸入 |  |  |  |
|----------|--------------|----------|----------|
|   Enter first polynomial:
Enter number of terms: 3
Enter coefficient and exponent: 2 2
Enter coefficient and exponent: 3 1
Enter coefficient and exponent: 1 0
Enter second polynomial:
Enter number of terms: 2
Enter coefficient and exponent: 1 2
Enter coefficient and exponent: 4 0
      | 輸出        |      
|            P1(x) = 2x^2 + 3x + 1
P2(x) = 1x^2 + 4

P1 + P2 = 3x^2 + 3x + 5
P1 * P2 = 2x^4 + 3x^3 + 9x^2 + 12x + 4

Enter a value of x to evaluate P1: 2
P1(2) = 15
      |              

```
```

### 效能量測
以輸入項數從 10、100、1000 測試

加法時間約隨項數線性增加
乘法時間為平方級增長（因每項需與另一多項式全部項相乘）
記憶體使用量主要取決於 termArray 大小


   

## 心得討論

### 這次程式重點

1. **物件導向的封裝與模組化設計**  
每個多項式都用 Polynomial 物件表示，內部包含動態陣列 termArray 來存放項目，並透過 AddTerm()、Add()、Mult() 等成員函式完成多項式的加法與乘法。這種封裝方式能有效管理資料，減少程式間的耦合度，讓主程式 main() 部分變得非常簡潔。
2. **動態記憶體配置與陣列擴充**  
程式使用 new 和 delete[] 來管理記憶體，並在項數超出容量時自動擴充。

3. **運算子多載實作**  
   這份程式中，利用 friend 函式多載了 >> 和 <<，讓輸入與輸出多項式時更自然。特別是輸出函式中，透過判斷正負號與排序（降冪輸出），輸出格式比較符合我們正常寫的方式，也讓我學會如何在程式中處理符號與格式問題。
  
**自己覺得的困難點**
動態記憶體的管理因為整個程式沒用 STL 的 vector，而是用原生的 new / delete[] 來動態配置空間。
困難在於：需要手動檢查陣列是否已滿；

複製多項式時要避免淺層複製版本例如:只會複製指標 termArray 的位址（不是內容）。結果 p1 和 p2 都指向同一塊記憶體。

這樣當程式結束時：
p1 解構 → delete[] termArray;
p2 解構 → 又 delete[] termArray;
 重複釋放記憶體 → 發生 Segmentation Fault

每次擴充容量要確保舊資料正確搬移，並釋放舊空間。

如果沒在適當時機 delete[] 舊的 termArray，就會造成「記憶體洩漏」，雖然程式表面沒當機，電腦記憶體也會越來越吃重。

















------------------------------------------------------------------------------------------------------------------------------------------------------------------
