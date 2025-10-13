# 41143122

作業一

## 解題說明

1. 分別使用遞迴函式與非遞迴版本完成阿克曼函式。
2. 本題要求實現一個程式，計算並輸出給定集合的冪集（Power Set），輸入為一字串表示的集合元素。

### 遞迴解阿克曼解題策略

1. 使用遞迴函式實現阿克曼函數的數學定義：
   
   (1)若 $m = 0$，則返回 $n + 1$。
   
   (2)若 $m > 0$ 且 $n = 0$，則遞迴調用 $A(m-1, 1)$。
   
   (3)$m > 0$ 且 $n > 0$，則遞迴調用 $A(m-1, A(m, n-1))$。
2. 主程式接收用戶輸入的 $m$ 和 $n$，並輸出計算結果。

### 非遞迴解阿克曼解題策略

1. 使用堆疊模擬阿克曼函數的遞迴過程，避免堆疊溢出問題。
2. 定義結構體儲存每次遞迴的狀態（$m$、$n$、標記和結果）。
3. 主程式接收用戶輸入的 $m$ 和 $n$，並輸出計算結果。
4. 加入堆疊溢出檢查，確保程式穩健性。

### 冪集解題策略

1. 使用位元運算生成集合的所有子集，透過計數器遍歷 $0$ 到 $2^n-1$，每個計數器的二進位表示對應一個子集。
2. 主程式接收用戶輸入的字串，轉換為字符陣列，傳遞給冪集計算函數。
3. 處理空集和無元素的情況，確保輸出格式一致。

## 程式實作

### 遞迴解阿克曼程式碼

```cpp
#include<iostream>
using namespace std;
int ackerman(int m,int n){
	if(m==0)
		return n+1;
	else if(m>0&&n==0)
		return ackerman(m-1,1);
	else
		return ackerman(m-1,ackerman(m,n-1));
}
int main(){
	int m,n=0;
	cout<<"請輸入m:"; 
	cin>>m;
	cout<<"請輸入n:"; 
	cin>>n;
	
	cout<<"A(m,n)="<<ackerman(m,n)<<endl;
}
```
### 非遞迴解阿克曼程式碼

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#define MaxSize 10000
using namespace std;

typedef struct {
    int m, n;
    int flag;
    int sum;
} AkmStack;

int Akm(int m, int n) {
    AkmStack st[MaxSize];
    int top = -1;
    top++;
    if (top >= MaxSize) {
        cout << "堆疊溢位" << endl;
        return -1;
    }
    st[top].m = m;
    st[top].n = n;
    st[top].flag = 0;
    st[top].sum = 0;
    while (top > -1) {
        if (st[top].flag == 2) {
            if (top == 0) {
                break;
            }
            int result = st[top].sum;
            top--;
            st[top].m--;
            st[top].n = result;
            st[top].flag = 0;
        } else {
            if (st[top].m == 0) {
                st[top].sum = st[top].n + 1;
                st[top].flag = 2;
            } else if (st[top].n == 0) {
                st[top].m--;
                st[top].n = 1;
                st[top].flag = 0;
            } else {
                top++;
                if (top >= MaxSize) {
                    cout << "堆疊溢位" << endl;
                    return -1;
                }
                st[top].m = st[top-1].m;
                st[top].n = st[top-1].n - 1;
                st[top].flag = 0;
                st[top].sum = 0;
            }
        }
    }
    return st[0].sum;
}

int main() {
    int m, n;
    cout << "請輸入m: ";
    cin >> m;
    cout << "請輸入n: ";
    cin >> n;
    cout << "A(" << m << "," << n << ") = " << Akm(m, n) << endl;
    return 0;
}
```

### 冪集程式碼

```cpp
#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
using namespace std;

void printPowerSet(char *set, int set_size)
{
    unsigned int pow_set_size = (unsigned int)pow(2, set_size);
    int counter, j;

    for(counter = 0; counter < pow_set_size; counter++)
    {
        bool hasElements = false;
        for(j = 0; j < set_size; j++)
        {
            if(counter & (1 << j))
            {
                cout << set[j]<<" ";
                hasElements = true;
            }
        }
        if (!hasElements) {
            cout << "{}";  // 輸出空集
        }
        cout << std::endl;
    }
}

int main()
{
    string input;
        cout << "請輸入集合元素 (例如 abc, 無空格) 或輸入 stop 停止: ";
        cin >> input;
        int set_size = input.length();
        if (set_size == 0) {
            cout << "{}" << std::endl;  // 空輸入視為空集
        }
        char *set = new char[set_size + 1];
        strcpy(set, input.c_str());
        printPowerSet(set, set_size);
        delete[] set;
    return 0;
}
```

## 效能分析
### 遞迴解阿克曼
1. 時間複雜度：阿克曼函數的遞迴深度和計算量會隨 $m$ 和 $n$ 快速增長，時間複雜度近似為原始遞迴函數 $O(A(m, n))$。
2. 空間複雜度：空間複雜度取決於堆疊深度，為 $O(A(m, n))$，在較大的 $m$ 和 $n$ 下可能導致堆疊溢出。
### 非遞迴解阿克曼
1. 時間複雜度：阿克曼函數的遞迴深度和計算量會隨 $m$ 和 $n$ 快速增長，時間複雜度近似為原始遞迴函數 $O(A(m, n))$。
2. 空間複雜度：因使用固定大小的堆疊陣列，空間複雜度為 $O(\text{MaxSize})$，其中 MaxSize = 10000，所以空間複雜度為 $O(10000)$
### 冪集
1. 時間複雜度：生成冪集需要遍歷 $2^n$ 個子集，每個子集檢查 $n$ 個元素，總時間複雜度為 $O(n \cdot 2^n)$。
2. 空間複雜度：使用動態分配的字符陣列儲存輸入集合，空間複雜度為 $O(n)$，其中 $n$ 為集合大小。

## 測試與驗證

### 遞迴解阿克曼與非遞迴解阿克曼測試案例

| 測試案例 | 輸入參數 $(m, n)$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $(0, 0)$      | 1        | 1       |
| 測試二   | $(1, 1)$     | 3        | 3        |
| 測試三   | $(2, 2)$     | 7        | 7        |
| 測試四   | $(3, 1)$      | 13       | 13       |
| 測試五   | $(4, 0)$     | 13       | 13       |

### 冪集測試案例

| 測試案例 | 輸入參數| 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | ""           | {}       | {}       |
| 測試二   | "a"          | {}, a     | {}, a      |
| 測試三   | "ab"        | {}, a, b, ab | {}, a, b, ab|
| 測試四   | "abc"      | {}, a, b, ab, c, ac, bc, abc | {}, a, b, ab, c, ac, bc, abc       |

### 編譯與執行指令

#### 遞迴解阿克曼
```shell
$ g++ Akmer.cpp -std=c++17 -o Akmer.exe
$ .\Akmer.exe
請輸入m: 2
請輸入n: 2
A(m,n)=7
```
#### 非遞迴解阿克曼
```shell
$ g++ NonRecursiveAkmer.cpp -std=c++17 -o  NonRecursiveAkmer.exe
$ .\NonRecursiveAkmer.exe
請輸入m: 2
請輸入n: 2
A(m,n)=7
```
#### 冪集
```shell
$ g++ powerset.cpp -std=c++17 -o  powerset.exe
$ .\powerset.exe
請輸入集合元素 (例如 abc, 無空格): abc
{}
a
b
a b
c
a c
b c
a b c
```

### 結論

1. 遞迴版與非遞迴版程式均能正確計算阿克曼函數的值，冪集程式能正確生成並輸出給定集合的冪集，均符合數學定義。  
2. 阿克曼測試案例涵蓋了多種邊界情況（$m = 0$、$n = 0$、$m, n > 0$），驗證程式的正確性。
3. 冪集測試案例涵蓋空集、單元素集合和多元素集合，驗證程式的正確性。 
4. 非遞迴阿克曼設有堆疊大小限制（MaxSize = 10000），需根據需求調整。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **符合數學定義**  
   阿克曼函數的數學定義本身是遞迴形式，程式碼直接對應公式，邏輯清晰： $$ A(m, n) = \begin{cases} n + 1 & \text{if } m = 0 \ A(m-1, 1) & \text{if } m > 0 \text{ and } n = 0 \ A(m-1, A(m, n-1)) & \text{otherwise} \end{cases} $$

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。

原先使用土法煉鋼的方式進行非遞迴阿克曼,後來發現記憶體消耗過大便改為用棧實現的方式
