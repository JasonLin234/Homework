#include <iostream>
#include <string>
#include <algorithm>

#define MaxSize 10000
using namespace std;

typedef struct{
	int m,n;
	int flag;
	int sum;
}AkmStack;

int Akm(int m,int n){
	AkmStack st[MaxSize];
	int top=-1;
	top++;
	if(top >= MaxSize){
		cout << "堆疊溢位" << endl;
		return -1;
	}
	st[top].m=m;
	st[top].n=n;
	st[top].flag=0;
	st[top].sum=0;
	while(top > -1){
		if(st[top].flag == 2){
			if(top == 0){
				break;
			}
			int result = st[top].sum;
			top--;
			st[top].m--;
			st[top].n = result;
			st[top].flag = 0;
		}
		else{
			if(st[top].m == 0){
				st[top].sum = st[top].n + 1;
				st[top].flag = 2;
			}
			else if(st[top].n == 0){
				st[top].m--;
				st[top].n = 1;
				st[top].flag = 0;
			}
			else{
				top++;
				if(top >= MaxSize){
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
    cout << "A(" << m << "," << n << ") = " << Akm(m,n) << endl;
    return 0;
}
