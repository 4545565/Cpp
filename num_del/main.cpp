#include<iostream>
using namespace std;
int main() {
    int t[5] = { 0 };
    int count[2][5] = { 0 };
    int i = 0, j = 0, cmax = 0, sum = 0;
    for (i = 0;i < 5;i++) {
        cin >> t[i];
    }
    for (i = 0;i < 5;i++) {
        for (j = 0;j < 5;++j) {
            if (t[i] == count[0][j]) {
                count[1][j]++;
                break;
            }
            else if (count[0][j] == 0) {
                count[0][j] = t[i];
                count[1][j] = 1;
                break;
            }
        }
    }
    for (j = 0;j < 5;++j) {
        if ((count[0][j] * count[1][j] > cmax) && count[1][j] > 1)
            cmax = count[0][j] * count[1][j];
    }
    for (i = 0;i < 5;++i) {
        sum += t[i];
    }
    sum -= cmax;
    cout << sum << endl;
    system("pause");
    return 0;
}