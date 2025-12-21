
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
| `多項式加法 A + B`     | O(n + m) | O(1)| 每一項最多被走一次 |
| `多項式減法 A - B`    | O(m + n)|  O(1)   | 先將 B 的係數取負，再進行加法  |
| `多項式乘法 A * B`    | O(m × n)     |   O(1)          | 為多項式運算中最昂貴的部分 |
| `Evaluate(x)`    | O(m)     |   O(1)      | 走訪所有項目    |
| `Available List`    | 節點回收與取得：O(1))     |   減少 new/delete 次數      | 在大量節點新增/刪除時效能明顯提升    |
## 測試與驗證

### 測試案例

| 測試案例 | 輸入(格式上到下) |  | 實際輸出(格式上到下) |
|----------|--------------|----------|----------|
|    | 3      |         |   A: x^2 + 4x + 3|
|    | 1 2 4 1 3 0      |  | B: x^5 + 2x^2   |
|    | 2    |        | A + B: x^5 + 3x^2 + 4x + 3|
|    | 1 5 2 2    |       |A - B: -x^5-x^2 + 4x + 3   |
|    |       |       |A * B: x^7 + 4x^6 + 3x^5 + 2x^4 + 8x^3 + 6x^2   |
|    |       |       |A / B = 0   |
|    |       |       | A % B = x^2 + 4x + 3  |
|    |       |       |A(X) = 15  |
```
```

### 效能量測

| 功能 | 實際操作次數 |  | 成本評估 |
|----------|--------------|----------|----------|
|  輸入  |    5 次 AddTerm   |         |   極低|
|  加法  |    5 次比較   |  | 極低   |
|  減法  |  7 次操作    |      |極低|
|   乘法 | 6 次乘法   |       |中等   |
|  Evaluate  | 3 次計算      |      |極低   |
```
```
   

## 心得討論

### 這次程式重點

1. **Circular Linked List + Header Node**  
避免空指標判斷,
插入與刪除流程一致,
程式更穩定。

2. **AddTerm() 作為核心函式**  
確保多項式始終保持排序,
合併同次方,
自動刪除係數為 0 的項目,
 降低其他運算的複雜度。

3. **Operator Overloading**  
使用方式貼近數學表示
提高程式可讀性與易用性。

4. **深拷貝（Rule of Three）**
copy constructor,
assignment operator,
destructor,
避免記憶體共用錯誤。

5. **Available List**
手動管理節點回收,
提升效能。

**自己覺得的困難點**

是實作AddTerm的部分:這個部分為整個程式的核心,AddTerm 同時要處理：依指數遞減插入,合併同次方,係數變 0 要刪節點,維持環狀串列
沒處理好就很容易發生插入順序錯 → 輸出亂掉,沒刪掉係數為 0 的項,prev / cur 指標錯位

多項式乘法的「合併問題」:因為乘法會產生大量同次方項,若不即時合併，會產生重複節點,要避免的話每一項結果直接呼叫 AddTerm,不留中間結構,邊產生、邊合併
```cpp
c.AddTerm(p->data.coef * q->data.coef,
          p->data.exp + q->data.exp);
```
再來是記憶體管理（Available List):點不是馬上 delete,要確保不重複回收不存取已回收節點
會忘記回收刪掉的節點,回收後還使用指標
解決方式
```cpp
AvailableList<Term>::ReturnNode(cur);
```
統一回收機制,Clear() 與刪節點都使用它




















------------------------------------------------------------------------------------------------------------------------------------------------------------------



