#include<iostream>
using namespace std;
#include<stdlib.h>
int main() {
    int* a = (int*)malloc(10 * sizeof(int));
    int b[10];
    cout << sizeof(int) << '\t' << sizeof(int*)
        << '\t' << sizeof(a) << '\t' << sizeof(b) << endl;
    free(a);
    return 0;
}