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
