#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

bool is_prime[1010];
int prime[1010], r;

int main() {
    double start = clock();

    for (int i = 2; i <= 1000; i++) {
        if (!is_prime[i]) {
            printf("%d ", i);
            prime[++r] = i;

        }
        for(int j = 1; j <= r; j++) {
            if (i * prime[j] > 1000) {
                break;
            }

            is_prime[prime[j] * i] = true;

            if (i % prime[j] == 0) {
                break;
            }
        }
    }
    double end = clock();
    printf("\n\nTime taken by CPU: %lf ms\n", end - start);

    return 0;
}