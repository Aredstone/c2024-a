//
// Created by a1823 on 24-12-17.
//


#include <format>
#include <iostream>
#include "LongRealNum.h"

int main() {
    pi_task::EXPS = 1000;

    long long start = clock();

    pi_task::LongRealNum num_1("1");
    pi_task::LongRealNum num_2("2");
    pi_task::LongRealNum num_4("4");
    pi_task::LongRealNum num_5("5");
    pi_task::LongRealNum num_6("6");
    pi_task::LongRealNum num_8("8");
    pi_task::LongRealNum num_16_k("1");
    pi_task::LongRealNum num_tmp("0");
    pi_task::LongRealNum num_ans("0");

    for (int i = 0; i <= 900; i++) {
        num_tmp = num_8 * i;
        num_ans = num_ans + (num_1 / num_16_k) *
            (num_4 / (num_tmp + num_1) -
                num_2 / (num_tmp + num_4) -
                num_1 / (num_tmp + num_5) -
                num_1 / (num_tmp + num_6));
        num_16_k = num_16_k * 16;
        std::cout << std::format("k = {}, now = ", i);
        std::cout << num_ans << std::endl;
    }
    long long end = clock();
    std::cout << num_ans << std::endl;
    std::cout << "Time elapsed: " << std::format("{} seconds\n", static_cast<double>(end - start) / CLOCKS_PER_SEC);

    return 0;
}

