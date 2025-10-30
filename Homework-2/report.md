
# 41343137

作業2

## 解題說明

多項式的建立與輸入
多項式的加法（Add）
多項式的乘法（Mult）
多項式的輸出（含正負號判斷）
多項式的代入運算（Eval）
採用「物件導向」方式設計，建立 Term 類別儲存單項式（係數與指數），再由 Polynomial 類別管理所有非零項。
多項式的運算（加法、乘法、代入）皆以項目為單位計算，並使用動態陣列 (new / delete[]) 來管理項數。
### 舉例

若
P1(x) = 2x² + 3x + 1
P2(x) = x² + 4

則
P1 + P2 = 3x² + 3x + 5
P1 × P2 = 2x⁴ + 3x³ + 9x² + 12x + 4
## 程式實作

以下為主要程式碼：

```cpp
// Example main function
int main() {
    Polynomial p1, p2;

    cout << "Enter first polynomial:\n";
    cin >> p1;
    cout << "Enter second polynomial:\n";
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    Polynomial prod = p1.Mult(p2);

    cout << "\nP1 + P2 = " << sum << endl;
    cout << "P1 * P2 = " << prod << endl;

    float x;
    cout << "\nEnter a value of x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
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

Enter first polynomial:
Enter number of terms: 3
Enter coefficient and exponent: 2 2
Enter coefficient and exponent: 3 1
Enter coefficient and exponent: 1 0
Enter second polynomial:
Enter number of terms: 2
Enter coefficient and exponent: 1 2
Enter coefficient and exponent: 4 0
輸出
P1(x) = 2x^2 + 3x + 1
P2(x) = 1x^2 + 4
P1 + P2 = 3x^2 + 3x + 5
P1 * P2 = 2x^4 + 3x^3 + 9x^2 + 12x + 4
Enter a value of x to evaluate P1: 2
P1(2) = 15

```
```

### 結論


   

## 申論及開發報告

### 選擇遞迴的原因

1. **程式結構簡潔、可讀性高**  

2. **遞迴的語意**  


3. **易於理解與實現**  
   
   以本程式為例：  
 ```cpp
  
   ```



















------------------------------------------------------------------------------------------------------------------------------------------------------------------


