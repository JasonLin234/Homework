# 41143122

作業二

## 解題說明

本題要求實作 Polynomial 類別，其抽象資料型別（ADT）包含多項式的動態儲存、輸入輸出、加法、乘法與求值運算，並重載 << 與 >> 運算子以支援多項式的格式化輸入與輸出。
```cpp
class Polynomial{
	private:
		Term *termArray;
		int capacity;
		int terms;
	public:
		Polynomial();
		Polynomial Add(Polynomial b);
		Polynomial Mult(Polynomial b);
		float Eval(float x);
};

class Term{
	friend Polynomial;
	private:
		int exp;
		float coef;
}; 
```

### 解題策略

1. Polynomial 類別以動態陣列 termArray 儲存多項式項，支援自動擴容。
2. 維持 降冪排列（由大到小）。
3. 實作複製建構子與賦值運算子（深複製）
4. Add()：合併同冪次項，係數為 0 則不加入
5. Mult()：每項相乘後合併同冪次項
6. Eval(x)：計算多項式在 x 點的值
7. newTerm() 自動擴容（容量倍增）
8. istream 和 ostream

## 程式實作

```cpp
#include<iostream>
#include <algorithm>
#include <cmath>
using namespace std;
class Polynomial;
class Term{
	friend Polynomial;
	friend bool term_greater(const Term&, const Term&);//降幕排列函式 
	friend ostream& operator<<(ostream &output,const Polynomial &Poly);
	private:
		int exp;
		float coef;
}; 
class Polynomial{
	private:
		Term *termArray;
		int capacity;
		int terms;
	public:
		Polynomial():capacity(2),terms(0){
			termArray = new Term[capacity];
		}
		~Polynomial(){
			delete[] termArray;
		}
		Polynomial(const Polynomial& other);
		Polynomial& operator=(const Polynomial& other);
		void sortTerms();
		Polynomial Add(Polynomial b);
		Polynomial Mult(Polynomial b);
		float Eval(float x);
		void newTerm(const float newcoef,const int newexp);
		
		friend istream& operator>>(istream &input,Polynomial &Poly);
		friend ostream& operator<<(ostream &output,const Polynomial &Poly);
};
istream& operator>>(istream &is,Polynomial &poly){
	float coef;
	int exp, n;
	is>>n;
	while(n--){
		is>>coef>>exp;
		poly.newTerm(coef,exp);
	}poly.sortTerms();
	return is;
}
ostream& operator<<(ostream &os,const Polynomial &poly){
	if (poly.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < poly.terms; i++) {
        float coef = poly.termArray[i].coef;
        int exp = poly.termArray[i].exp;

        if (i > 0) {
            if (coef > 0) os << " + ";
            else { os << " - "; coef = -coef; }
        } else if (coef < 0) {
            os << "-";
            coef = -coef;
        }

        if (coef == 0) continue;

        if (exp == 0) os << coef;
        else if (exp == 1) {
            if (coef != 1) os << coef;
            os << "x";
        } else {
            if (coef != 1) os << coef;
            os << "x^" << exp;
        }
    }
    return os;
}
// 複製建構子
Polynomial::Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
}

// 賦值運算子
Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this == &other) return *this;
    delete[] termArray;
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
    return *this;
}

bool term_greater(const Term& a, const Term& b) { return a.exp > b.exp; }

void Polynomial::sortTerms() {
    std::sort(termArray, termArray + terms, term_greater);
}

Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    Polynomial a = *this;  // 複製避免修改原物件
    a.sortTerms();
    b.sortTerms();

    int i = 0, j = 0;
    while (i < a.terms && j < b.terms) {
        if (a.termArray[i].exp == b.termArray[j].exp) {
            float sum = a.termArray[i].coef + b.termArray[j].coef;
            if (sum != 0) c.newTerm(sum, a.termArray[i].exp);
            ++i; ++j;
        }
        else if (a.termArray[i].exp > b.termArray[j].exp) {
            c.newTerm(a.termArray[i].coef, a.termArray[i].exp);
            ++i;
        }
        else {
            c.newTerm(b.termArray[j].coef, b.termArray[j].exp);
            ++j;
        }
    }
    while (i < a.terms) c.newTerm(a.termArray[i].coef, a.termArray[i].exp), ++i;
    while (j < b.terms) c.newTerm(b.termArray[j].coef, b.termArray[j].exp), ++j;

    return c;  // 自動深複製
}
Polynomial Polynomial::Mult(Polynomial b){
    Polynomial c;
    for(int i = 0; i < terms; i++){
        for(int j = 0; j < b.terms; j++){
            float newCoef = termArray[i].coef * b.termArray[j].coef;
            int newExp = termArray[i].exp + b.termArray[j].exp;
            // 需合併同冪次項
            bool found = false;
            for(int k = 0; k < c.terms; k++){
                if(c.termArray[k].exp == newExp){
                    c.termArray[k].coef += newCoef;
                    found = true;
                    break;
                }
            }
            if(!found && newCoef != 0){
                c.newTerm(newCoef, newExp);
            }
        }
    }
    c.sortTerms();
    return c;
}
float Polynomial::Eval(float x){
    float result = 0;
    for(int i = 0; i < terms; i++){
        result += termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}
void Polynomial::newTerm(const float theCoef,const int theExp) {
	if(theCoef==0)
		return;
	if(terms==capacity){
		capacity*=2;
		Term *temp=new Term[capacity];
		copy(termArray,termArray+terms,temp);
		delete []termArray;
		termArray=temp;
	}
	termArray[terms].coef=theCoef;
	termArray[terms++].exp=theExp;
}
int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式 (項數 係數 指數): ";
    cin >> p1;
    cout << "輸入第二個多項式: ";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    cout << "加法: " << p1.Add(p2) << endl;
    cout << "乘法: " << p1.Mult(p2) << endl;
    cout << "p1(2) = " << p1.Eval(2) << endl;
    return 0;
}
```

## 效能分析
### 遞迴解阿克曼
1. 時間複雜度：阿克曼函數的遞迴深度和計算量會隨 $m$ 和 $n$ 快速增長，時間複雜度近似為原始遞迴函數 $O(A(m, n))$。
2. 空間複雜度：空間複雜度取決於堆疊深度，為 $O(A(m, n))$，在較大的 $m$ 和 $n$ 下可能導致堆疊溢出。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | 2 | p1 = 3x^2 + 1 | p1 = 3x^2 + 1 |
| |2 3 | p2 = 2x | p2 = 2x |
| |1 0 | 加法: 3x^2 + 2x + 1 | 加法: 3x^2 + 2x + 1 |
| |1 | 乘法: 6x^3 + 2x | 乘法: 6x^3 + 2x |
| |2 1 | p1(2) = 13 | p1(2) = 13 |
| 測試二   | 3 | p1 = x^3 - 2x + 4 | p1 = x^3 - 2x + 4 |
| | 1 3 | p2 = x + 1 | p2 = x + 1 |
| | -2 1 | 加法: x^3 - x + 5 | 加法: x^3 - x + 5 |
| |4 0 | 乘法: x^4 + x^3 - x^2 - x + 4 | 乘法: x^4 + x^3 - x^2 - x + 4 |
| | 2  | p1(2) = 13 | p1(2) = 8 |
| | 1 1 | | |
| | 1 0 | | |

### 編譯與執行指令

```shell
$ g++ Polynomial.cpp -std=c++17 -o Polynomial.exe
$ .\Polynomial.exe
輸入第一個多項式 (項數 係數 指數): 2
3 2
1 0
輸入第二個多項式: 1
2 1
p1 = 3x^2 + 1
p2 = 2x
加法: 3x^2 + 2x + 1
乘法: 6x^3 + 2x
p1(2) = 13
```

### 結論

1. 遞迴版與非遞迴版程式均能正確計算阿克曼函數的值，冪集程式能正確生成並輸出給定集合的冪集，均符合數學定義。  
2. 阿克曼測試案例涵蓋了多種邊界情況（ $m = 0$ 、 $n = 0$ 、 $m, n > 0$ ），驗證程式的正確性。
3. 冪集測試案例涵蓋空集、單元素集合和多元素集合，驗證程式的正確性。 
4. 非遞迴阿克曼設有堆疊大小限制（MaxSize = 10000），需根據需求調整。

## 申論及開發報告

### 非遞迴阿克曼使用堆疊(stack)的原因

1. **模擬遞迴邏輯**  
   在不使用遞迴的情況下，我選擇的處理方法就是堆疊，用堆疊模擬遞迴，將遞迴狀態儲存在陣列中來管理記憶體。程式使用結構體 AkmStack 記錄每次遞迴的狀態，清晰模擬了阿克曼函數的遞迴過程。例如：
```cpp
if (st[top].m == 0) {
    st[top].sum = st[top].n + 1;
    st[top].flag = 2;
} else if (st[top].n == 0) {
    st[top].m--;
    st[top].n = 1;
    st[top].flag = 0;
} else {
    top++;
    st[top].m = st[top-1].m;
    st[top].n = st[top-1].n - 1;
    st[top].flag = 0;
    st[top].sum = 0;
}
```
阿克曼函數的遞迴深度極高，傳統遞迴實現容易導致堆疊溢出，非遞迴實現雖然程式碼較複雜，但能處理更大的輸入範圍，適合需要穩定性和高效記憶體管理的場景。
### 冪集使用位元運算的原因

１. **高效生成子集** 

   透過計數器 $counter$ 從 $0$ 到 $2^n-1$，每個 $counter$ 的二進位表示對應一個子集。例如，對於集合 ${a, b}$，$counter = 2$（二進位 10）表示子集 ${b}$。這種方法簡單且高效：   

```cpp
for (counter = 0; counter < pow_set_size; counter++) {
    bool hasElements = false;
    for (j = 0; j < set_size; j++) {
        if (counter & (1 << j)) {
            cout << set[j] << " ";
            hasElements = true;
        }
    }
    if (!hasElements) {
        cout << "{}";
    }
    cout << endl;
}
```

2. **程式碼簡潔**
   
	位元運算避免了顯式遞迴或複雜的組合邏輯，程式碼易於理解和實現。

位元運算實現冪集生成簡單高效，適合展示離散數學中的集合論概念。然而，當集合大小 $n$ 較大時，$2^n$ 的子集數量會導致輸出時間增加，未來可考慮優化輸出格式或支援分批處理。
