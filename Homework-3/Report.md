
# 41343137

作業3

## 解題說明

多項式的建立與輸入
多項式的加法（Add）
多項式的乘法（Mult）
多項式的輸出（含正負號判斷）
多項式的代入運算（Eval）
用物件導向方式設計，建立 Term 類別儲存單項式（coef與exp），再由 Polynomial 類別管理所有非零項。
多項式的運算（+、*、代入）皆以項目為單位計算，並使用動態陣列 (new / delete[]) 來管理項數。
### 定義 Term 類別，表示一項多項式

使用 ChainNode<Term> 建立 circular linked list
實作 AddTerm()，確保：
串列依指數排序
同次方自動合併
所有運算（+ − *）都基於 AddTerm()
利用 AvailableList 回收節點，減少記憶體配置成本


## 程式實作

以下為主要程式碼片段：
1.Polynomial 的表示方式     
```cpp
class Polynomial {
private:
    ChainNode<Term>* head;   // header node（不存資料）
};
```
```cpp
Polynomial::Polynomial() {
    head = new ChainNode<Term>();
    head->link = head;       // circular list
}
```
說明:
head 是 錨點（sentinel）
串列永遠是環狀，不會有 nullptr
插入、刪除邏輯一致，避免特例處理

2.節點回收機制（Available List)
```cpp
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
```
說明:
減少 new / delete
回收節點供下次使用
在多項式乘法裡面能有效提升效能

3.核心函式：AddTerm()
```cpp
void AddTerm(double c, int e) {
    if (c == 0) return;

    ChainNode<Term>* prev = head;
    ChainNode<Term>* cur = head->link;

    while (cur != head && cur->data.exp > e) {
        prev = cur;
        cur = cur->link;
    }

    if (cur != head && cur->data.exp == e) {
        cur->data.coef += c;
        if (fabs(cur->data.coef) < 1e-9) {
            prev->link = cur->link;
            AvailableList<Term>::ReturnNode(cur);
        }
    } else {
        prev->link = AvailableList<Term>::GetNode(Term(c, e), cur);
    }
}
```
說明:
維持 指數遞減排序
自動合併同次方
係數為 0 → 刪除節點
其他運算全部建立在這個函式上

## 效能分析
多項式 A 有 m 項
多項式 B 有 n 項
| 操作          | 時間複雜度    | 空間複雜度        | 說明          |
| ----------- | -------- | ------------ | ----------- |
| `AddTerm(c, e)` | O(k)    | O(1)         | 最壞情況：走完整條串列 |
| `多項式加法 A + B`     | O(n + m) | O(m + n)| 每一項最多被走一次 |
| `多項式減法 A - B`    | O(m + n)|  O(1)   | 先將 B 的係數取負，再進行加法  |
| `多項式乘法 A * B`    | O(m × n)     |   O(1)          | 為多項式運算中最昂貴的部分 |
| `Evaluate(x)`    | O(m)     |   O(1)      | 走訪所有項目    |
| `Available List`    | 節點回收與取得：O(1))     |   減少 new/delete 次數      | 在大量節點新增/刪除時效能明顯提升    |
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



