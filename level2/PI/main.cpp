#include <format>
#include <iostream>
#include "LongRealNum.h"

int main() {
    pi_task::EXPS = 10010;

    long long start = clock();
    pi_task::LongRealNum ans(0);
    pi_task::LongRealNum num_16(1);

    for (int i = 0; i <= 8700; i++) {
        ans = ans * 16 + (
            pi_task::LongRealNum(4, 8 * i + 1) -
            pi_task::LongRealNum(2, 8 * i + 4) -
            pi_task::LongRealNum(1, 8 * i + 5) -
            pi_task::LongRealNum(1, 8 * i + 6));
        num_16 = num_16 * 16;
    }

    std::cout << ans * 16 / num_16 << std::endl;
    std::cout << "Time elapsed: " << std::format("{} seconds\n", static_cast<double>(clock() - start) / CLOCKS_PER_SEC);

    // 1w 61.364s
    return 0;
}

