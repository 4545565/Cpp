#include<stdio.h>
int main() {
    printf("hello\n");
    FILE* fp = fopen("sources.txt", "r");
    char c = fgetc(fp);
    int i = 0;
    while (c != -1) {
        printf("%c", c);
        c = fgetc(fp);
    }
    fclose(fp);
    return 0;
}