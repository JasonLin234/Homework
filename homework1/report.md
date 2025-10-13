# 41143122

作業一

## 解題說明

1. 分別使用遞迴函式與非遞迴版本完成阿克曼函式。
2. 如果S是一個包含n個元素的集合，則S的幂集是S所有可能子集的集合，寫一個遞迴函式來計算powerset(S)。

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

## 效能分析

1. 時間複雜度：阿克曼函數的遞迴深度和計算量會隨 $m$ 和 $n$ 快速增長，時間複雜度近似為原始遞迴函數 $O(A(m, n))$。
2. 空間複雜度：空間複雜度取決於堆疊深度，為 $O(A(m, n))$，在較大的 $m$ 和 $n$ 下可能導致堆疊溢出。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $(m, n)$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $(0, 0)$      | 1        | 1       |
| 測試二   | $(1, 1)$     | 3        | 3        |
| 測試三   | $(2, 2)$     | 7        | 7        |
| 測試四   | $(3, 1)$      | 13       | 13       |
| 測試五   | $(4, 0)$     | 13       | 13       |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
請輸入m: 2
請輸入n: 2
A(m,n)=7
```

### 結論

1. 程式能正確計算阿克曼函數的值，符合數學定義。  
2. 測試案例涵蓋了多種邊界情況（$m = 0$、$n = 0$、$m, n > 0$），驗證程式的正確性。  
3. 由於遞迴深度過高，當 $m \geq 4$ 時可能因堆疊溢出而失敗，需考慮非遞迴實現。

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
