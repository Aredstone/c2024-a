#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

bool is_prime[101];
int prime[101], r;

int main() {
    double start = clock();

    for (int i = 2; i <= 100; i++) {
        if (!is_prime[i]) {
            prime[++r] = i;
        }
        for(int j = 1; j <= r; j++) {
            if (i * prime[j] > 100) {
                break;
            }

            is_prime[prime[j] * i] = true;

            if (i % prime[j] == 0) {
                break;
            }
        }
    }
    for (int i = 4; i <= 100; i += 2) {
        for (int j = 1; j <= r; j++) {
            if (!is_prime[i - prime[j]]) {
                printf("The even number %d can be represented as the sum of prime numbers %d and %d\n", i, prime[j], i - prime[j]);
                break;
            }
        }
    }

    return 0;
}