#include <stdio.h>
#include <stdlib.h>

void hanoi(int n, char from, char to, char tmp) {
    if (n == 1) {
        printf("%c -> %c\n", from, to);
        return;
    }
    hanoi(n - 1, from, tmp, to);
    printf("%c -> %c\n", from, to);
    hanoi(n - 1, tmp, to, from);
}

int main() {

    hanoi(4, 'A', 'B', 'C');

    return 0;
}