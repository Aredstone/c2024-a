#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 差不多是凯撒密码加密，但是为了保证输出的字符是可见的就用了一些奇奇怪怪的字面量
 * 来限定凯撒密码得到的东西的范围。
 */

char s[1000];

void encoder() {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        s[i] = (char)((s[i] - 32 + 60) % 95 + 32);
    }
}

void decoder() {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        s[i] = (char)((s[i] - 32 - 60 + 95) % 95 + 32);
    }
}

int main() {

    printf("Please enter a string:");
    scanf("%s", s);
    encoder();
    printf("encoder string:%s\n", s);
    decoder();
    printf("decoder string:%s\n", s);

    return 0;
}