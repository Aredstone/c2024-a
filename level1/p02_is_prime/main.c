#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int n;

int main() {
    printf("Enter a number:");
    scanf("%d", &n);
    if (n < 2) {
        printf("Illegal input.");
        return -1;
    }
    int len = (int)sqrt(n) + 1;
    for (int i = 2; i <= len; i++) {
        if (n % i == 0) {
            printf("it's composite number.");
            return 0;
        }
    }
    printf("it's prime number.");

    return 0;
}