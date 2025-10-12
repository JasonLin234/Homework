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
