
# 41343137

作業2
## 解題說明

實作四種基礎排序演算法（Insertion Sort, Quick Sort, Merge Sort, Heap Sort），並透過嚴格的最壞情況（Worst-case）測試資料與高精度計時系統，量測其在不同資料量 $n$ 下的執行時間。最終，依據實驗量測結果，設計出一個在各種 $n$ 值下皆能達到最佳效能的「複合式排序函數

1.計時精確度控制（精確度 $\le 1\%$）：由於小資料量下排序速度極快，單次量測易因硬體時脈不準而產生 $0\text{ ms}$ 的誤差。因此系統採用智慧型防秒殺機制，當 $n$ 較小時會自動重複執行高達 100 至 500 次（repetitions），拉長總時間後再取平均值，確保將誤差控制在 $1\%$ 以內。

2.最壞情況測試資料生成：Insertion Sort：直接生成完全逆序的數列（如 $n, n-1, \dots, 1$），強迫每次插入都需要移動最大次數。

Merge Sort：使用「逆向構造法（Working backward）」，透過奇偶項交錯拆分，構造出能讓迭代版合併排序在每次 Merge 時都發生最高比較次數的極端數列。

Quick Sort & Heap Sort：遵照投影片規範，對於同一個 $n$ 值，隨機打亂並測試至少 10 次（本實驗採 15 次）不同的排列，並從中抽取出「最大執行時間（Max time）」作為最壞情況的代表。

3.複合式排序設計（Composite Sort）：綜合演算法理論，在 $n$ 極小時（如 $n < 20$），Insertion Sort 由於常數極小且不需額外記憶體，速度最快；當 $n$ 變大時，應果斷切換至具備穩定 $O(n \log n)$ 且常數較小的 Heap Sort，以防止 Quick/Insertion 退化至 $O(n^2)$。


## 程式實作

以下為主要程式碼：
1. 迭代版合併排序 (Iterative Merge Sort)
為符合投影片中「不使用遞迴（Non-recursive）」的要求，採用由底向上的迭代設計，利用雙重迴圈逐層加倍合併區間（size *= 2）。
```cpp
void MergeSort(vector<int>& a) {
    int n = a.size();
    if (n <= 1) return;
    vector<int> b(n); // 配置與原陣列等大的暫存輔助空間
    
    // size 代表每次合併的子陣列長度 (以 1, 2, 4, 8... 的倍數增長)
    for (int size = 1; size < n; size *= 2) {
        for (int l = 0; l < n - 1; l += 2 * size) {
            int m = min(l + size - 1, n - 1);
            int r = min(l + 2 * size - 1, n - 1);
            Merge(a, b, l, m, r);
        }
    }
}

```

2. 隨機序列與最大時間獲取 (Quick & Heap Worst-case Testing)
此片段展示了如何透過重複打亂 15 次，並抓取最高耗時（maxTime）來逼近最壞情況


```cpp
if (sortType == "Quick" || sortType == "Heap") {
    double maxTime = 0.0;
    int permutationsCount = 15; // 跑 15 次隨機序列取最大值

    for (int p = 0; p < permutationsCount; p++) {
        vector<int> baseData(n);
        for (int i = 0; i < n; i++) baseData[i] = i;

        Permute(baseData, n); // 呼叫投影片 Program 7.20 的隨機打亂

        auto start = chrono::high_resolution_clock::now();
        for (int r = 0; r < repetitions; r++) {
            vector<int> testData = baseData; // 還原資料
            if (sortType == "Quick") QuickSort(testData);
            else HeapSort(testData);
        }
        auto end = chrono::high_resolution_clock::now();
        double currentAvgTime = chrono::duration<double, milli>(end - start).count() / repetitions;

        if (currentAvgTime > maxTime) maxTime = currentAvgTime; // 取最大值
    }
    return maxTime;
}

```

3. 最終複合式排序函數 (Composite Sort)
臨界點（Threshold）設為 20，兼顧小資料的低常數優勢與大資料的穩定邊界。

```cpp
void CompositeSort(vector<int>& a) {
    int n = a.size();
    if (n < 20) {
        InsertionSort(a); // 小資料集下，Insertion Sort 常數小、效率最佳
    }
    else {
        HeapSort(a);      // 大資料集下，穩定維持 O(n log n) 防止退化
    }
}

```

## 效能分析

(a)

height / log₂(n) 約為常數（約 2）
表示 BST 高度成長為：
O(log n)

 即隨機插入下，BST 平均高度接近平衡樹。

(b)

刪除操作的時間複雜度取決於樹高 h：

平均情況（隨機 BST）：
 O(log n)
最壞情況（退化為鏈狀）：
 O(n)
## 測試與驗證

### 測試案例
(a)
| 測試案例 |n| Height | Ratio |
|----------|--------------|----------|----------|
| 測試一   | 100      | 14        | 2.10      |
| 測試二   | 500      | 18        | 2.00       |
| 測試三   | 1000      | 20        | 2.00        |
| 測試四   | 3000      | 25       | 2.16       |

結果說明：

Ratio 大致維持在 2 左右
符合理論預期

### 測試案例
(b)
```
輸入:
插入：50 30 70 20 40 60 80
刪除：50

輸出:
原始 BST: 20 30 40 50 60 70 80
刪除後:   20 30 40 60 70 80
```
說明：

刪除節點 50（兩個子節點）
使用右子樹最小值 60 取代


### 效能量測
(a)

針對不同 n（100 ~ 10000），量測：

BST 高度（height）
比值：height / log₂(n)

並觀察其趨勢是否接近常數。

(b)

針對刪除操作，分析其時間複雜度（不需實際量測）：

平均情況：O(log n)
最壞情況：O(n)
   

## 心得討論

### 這次程式重點

1. **隨機性對平衡的貢獻**  
在實作 (a) 部分，雖然 BST 在最差情況下會退化成 $O(n)$ 的鏈狀結構（例如依序輸入 1, 2, 3...），但實驗數據顯示，當輸入資料具備隨機性時，比值 height / log2(n) 穩定在 2 左右。這證明了在現實應用中，只要資料分布相對均勻，BST 就能維持極佳的搜尋效率（$O(\log n)$）。
2. **遞迴與指標引用的優雅實作**  
在實作 (b) 的刪除操作中，使用 「指標的引用 (Node*&)」 能大幅簡化程式碼。

傳統做法：需要手動處理父節點與子節點的重新連接，邏輯較為破碎。

引用做法：直接在遞迴中修改指標本身，讓程式碼更直觀且不易出錯。
這讓我理解到，適當利用程式語言的特性（如 C++ 的 Reference），可以讓複雜的演算法實作變得更加優雅且易於維護。

**自己覺得的困難點**

 **雙子節點刪除**
當要刪除的節點同時擁有左、右子樹時，不能隨便抓一個子節點遞補，否則會破壞 BST 「左小右大」的規則。

策略選擇：必須從 右子樹找最小值 (Inorder Successor) 或 左子樹找最大值 (Inorder Predecessor)。

為什麼選 Successor？：因為它是「剛好大於目標值」的那個節點，把它換到根部後，原本左子樹的所有值依然比它小，右子樹剩餘的值依然比它大。

遞迴刪除的陷阱：
在將 Successor 的值複製到目標節點後，必須再次呼叫 remove 去刪除那個位於下方的原始 Successor 節點。這是一個「遞迴內包含遞迴」的過程，邏輯上較為繞口。

**容易出錯的點**

記憶體洩漏 (Memory Leak)：
在使用 new Node 建立節點後，刪除時必須確實執行 delete temp。此外，若沒有實作 解構子 (Destructor) 來回收整棵樹，程式結束前會殘留大量無效記憶體。

指標引用的傳遞 (Node*&)：
在 remove 函式中，若忘記加上 &（只寫 Node* node），你在函式內部修改 node = node->right 只會改到區域變數，原本樹狀結構中的父節點指標並不會更新，導致刪除失敗。

更新高度的時機：
高度更新必須放在遞迴回溯段（即 insert 完子樹之後）。如果放錯位置，父節點的高度將無法反映子樹最新的變化。
動態記憶體的管理因為整個程式沒用 STL 的 vector，而是用原生的 new / delete[] 來動態配置空間。
困難在於：需要手動檢查陣列是否已滿.

















------------------------------------------------------------------------------------------------------------------------------------------------------------------

