#include<iostream>
#include<stdlib.h>
#include<math.h>
using namespace std;
int main() {
    int n = 0;
    int d = 0, x = 0;
    int i = 0;
    int* a;
    cin >> n;
    a = (int*)malloc(n * sizeof(int));
    for (i = 0;i < n;i++) {
        cin >> *(a + i);
        if (*(a + i) > *(a + d))
            d = i;
        else if (*(a + i) < *(a + x))
            x = i;
    }
    free(a);
    int d0 = x - 0,     //最大值移动到左侧
        d1 = n - 1 - x, //最大值移动到右侧
        d2 = d - 0,     //最小值移动到左侧
        d3 = n - 1 - d; //最小值移动到右侧
    int result = max(max(d0, d1), max(d2, d3));
    cout << result << endl;
    system("pause");
    return 0;
}