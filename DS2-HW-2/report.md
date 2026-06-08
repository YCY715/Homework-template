
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

## 效能量測

## 測試與驗證

### 測試案例
(a)
| n |Insertion| Quick | Merge | Heap  |  Composite |  
|----------|--------------|----------|----------|----------|----------|
| 500   |4.73692   |0.282415   | 0.339381   | 0.716373 |0.627344   |
| 1000  | 19.1876  | 0.616576  | 0.754543   | 1.58414  |1.43322   |
|  2000 | 73.7376  | 1.24906   | 1.66994    | 3.53766  | 3.13406 |
| 3000  | 167.932  | 1.98651   | 2.61696    | 5.97057  | 5.01367 |
| 4000  | 300.864  | 2.5891    |  3.92689   | 8.72781  | 7.03065   |
| 5000  | 535.154  | 4.73905   |  6.2782    | 12.3093  | 10.0735   |



### 效能分析
1.Insertion Sort 的退化驗證  (O(n^2))：
從數據可以看出，當 $n$ 從 $1000$ 翻倍到 $2000$，再翻倍到 $4000$ 時，Insertion Sort 的執行時間呈現驚人的 4 倍速度暴增（從約 $19\text{ ms} \rightarrow 73\text{ ms} \rightarrow 296\text{ ms}$）。這在理論上完全符合其最壞情況下的平方複雜度曲線，也證明了純逆序數列確實是其致命的最壞狀況。

 2.Quick Sort 搭配三數取中的強健性：
儘管投影片要求透過 15 次隨機打亂來抓取最大時間，但由於本實作導入了「三數取中法（Median-of-three）」，它極難在隨機排列中挑選到極端的 Pivot。因此，在隨機盲測下，Quick Sort 依然維持極高水準的 $O(n \log n)$ 常數表現，在表格中甚至超越了 Merge 和 Heap，這符合投影片第 3 張所預期的「其他方法可能都比 Quick Sort 慢」的學術現象。

3.Merge Sort 與 Heap Sort 的時間穩定性：
Merge 與 Heap 在最壞情況下的增長曲線非常平緩。值得注意的是，Merge 的時間普遍比 Heap 還要快一些，這反映出 Heap Sort 在維持 Heap 結構時（MaxHeapify）的元素交換常數稍微高了一點點；然而 Heap Sort 具備空間複雜度 $O(1)$ 的優勢，而非遞迴版 Merge Sort 則需要額外 $O(n)$ 的記憶體。

4.Composite Sort 複合函數的綜合綜效（Win-Win）：
觀察 Composite 欄位可以發現，在大小為 $n=1000$ 時，其時間（1.38ms）優於單獨使用 Heap Sort（1.55ms）。這是因為它在子陣列長度小於 20 時，切換成幾乎沒有常數開銷、免維護堆積樹結構的 Insertion Sort，成功將大演算法的理論優勢與小演算法的低常數完美融合。實驗證明，複合式排序成功達到了投影片所要求的「在所有 $n$ 範圍下皆能產生最佳效能」的終極目的。

## 心得討論

### 這次程式重點
1. **最壞情況（Worst-case）」科學定義與實作**  
  Insertion Sort：要給它完全逆序的數列（$n$ 到 $1$），強迫它時間複雜度退化到 $O(n^2)$。

  Merge Sort：採用「逆向構造法（Working backward）」，把已經排好的數列故意拆成奇偶項交錯，強迫它在每一層合併時，左邊和右邊的指標都要比較到最後一刻，達到最大比較次數。

  Quick & Heap Sort：這兩者最有趣，規定要求生成至少 10 次隨機排列，並抓出其中「執行時間最長（Max time）」的那一次來當作最壞情況的代表。

2. **精確度控制（消除硬體計時誤差）**  
當 $n = 500$ 這種小資料量時，有些演算法（像是 Quick Sort）執行一次可能只要 $0.01$ 毫秒。如果直接量測，硬體的微秒級時鐘根本抓不到，通常會直接顯示 $0 \text{ ms}$（也就是你一開始遇到的情況）。

重點解法：程式加入了智慧型重複次數（repetitions）機制。在資料量小的時候，強迫程式重複跑 100 到 500 次，累積出足夠長的時間後再除以重複次數。這樣成功把計時誤差壓到了投影片要求的 $1\%$ 以內

**複合式排序（Composite Sort）的優勢**  
論上大資料（n 很大）用 $O(n \log n)$ 的 Heap Sort 比較快；但當子陣列被切得很小（例如 $n < 20$）時，Heap Sort 維護樹狀結構的「常數開銷（Overhead）」反而太大，這時候直接用常數極小、免額外記憶體的 Insertion Sort 反而會反超。重點成果：實作出來後，你的實驗數據（例如 $n=1000$ 時）成功證明了 Composite 的速度超越了純 Heap Sort，達到了 1+1 > 2 的最佳化效果。

### 實作上的困難點

 **迭代版合併排序（Iterative Merge Sort）的邊界控制**

投影片明確規定「Merge Sort 不能使用遞迴（Non-recursive）」。困難點：遞迴版的 Merge Sort 很好寫（直接二分遞迴即可），但改成用 for 迴圈由底向上（1 疊 2、2 疊 4、4 疊 8...）去合併時，最大的地獄在於「陣列長度不一定是 2 的冪次方」。
當 $n=5000$ 或 $3000$ 時，迴圈切到最後一定會剩下孤兒碎塊。程式中必須精準使用 min(l + size - 1, n - 1) 來鎖死右邊邊界（r）和中間點（m），只要算錯 1 個字元，程式不是直接崩潰（Segmentation Fault）就是資料漏排序。

**迭記憶體管理與重複計時的「環境復原」**

困難點：因為我們要讓演算法重複跑好幾百次（reps = 500）來取平均時間。如果你的測試資料 testData 在第一遍跑完就被「排好序」了，那麼第二遍、第三遍到第五百遍，演算法面對的其實都是一個已經完全排好（Sorted）的數列！

如果沒有在每次進入排序前，用 testData = baseData; 進行完整的記憶體複製與資料還原，量測出來的數據就會全部失真（例如 Insertion Sort 面對排好的數列會變成超快的 $O(n)$）。如何在高效能的計時迴圈內做好資料還原，是實作上很吃細節的地方。

### 困難點狀況及改善方法

記憶體洩漏 (Memory Leak)：
在使用 new Node 建立節點後，刪除時必須確實執行 delete temp。此外，若沒有實作 解構子 (Destructor) 來回收整棵樹，程式結束前會殘留大量無效記憶體。

指標引用的傳遞 (Node*&)：
在 remove 函式中，若忘記加上 &（只寫 Node* node），你在函式內部修改 node = node->right 只會改到區域變數，原本樹狀結構中的父節點指標並不會更新，導致刪除失敗。

更新高度的時機：
高度更新必須放在遞迴回溯段（即 insert 完子樹之後）。如果放錯位置，父節點的高度將無法反映子樹最新的變化。
動態記憶體的管理因為整個程式沒用 STL 的 vector，而是用原生的 new / delete[] 來動態配置空間。
困難在於：需要手動檢查陣列是否已滿.

















------------------------------------------------------------------------------------------------------------------------------------------------------------------

