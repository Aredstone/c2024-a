//
// Created by a1823 on 24-12-17.
//

#include "LongRealNum.h"
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <format>
#include <sstream>

// #define DEBUG_MODE

namespace pi_task {
    LongRealNum::LongRealNum(): sign(false){};

    LongRealNum::LongRealNum(const std::string &s) {
        if (s[0] == '-') {
            sign = true;
        }
        std::string tmp;
        size_t save = s.length() - 1;
        size_t l = s.length();
        for (size_t i = 0; i < l; i++) {
            if (s[i] == '.') {
                save = i;
                if (l - save - 1 > EXPS)
                    l = EXPS + 1 + save;
            } else if (isdigit(s[i])) {
                tmp += s[i];
            } else if (s[i] != '-') {
                std::cerr << std::format("Illegal character '{}' in pos {}. The complete string is {}\n", s[i], i, s);
                throw std::invalid_argument("Invalid");
            }
        }
        for (size_t i = s.length() - save; i <= EXPS; i++) {
            tmp += '0';
        }
        std::ranges::reverse(tmp);
        num = tmp;
        fix();
    }

    LongRealNum::LongRealNum(long long l1, long long l2) {
        // std::swap(l1, l2);
        std::string tmp(std::to_string(l1 / l2));
        if ((l1 < 0 && l2 > 0) || (l1 > 0 && l2 < 0)) {
            tmp = '-' + tmp;
            l1 = std::abs(l1);
            l2 = std::abs(l2);
        }
        tmp += '.';
        l1 %= l2;
        for (int i = 0; i < EXPS; i++) {
            l1 *= 10;
            tmp += (l1 / l2) + '0';
            l1 %= l2;
        }
        *this = LongRealNum(tmp);
    }

    LongRealNum::LongRealNum(const int i) {
        *this = LongRealNum(std::to_string(i));
    }

    LongRealNum::LongRealNum(const long long l) {
        *this = LongRealNum(std::to_string(l));
    }

    std::ostream &operator<<(std::ostream &os, const LongRealNum &lr) {
        const std::string &tmp = lr.num;
        if (lr.sign)
            os << '-';
        if (tmp.length() == EXPS)
            os << '0';
        for (int i = tmp.length() - 1; i >= 0; i--) {
            if (EXPS == i + 1)
                os << '.';
            os << tmp[i];
        }
        return os;
    }

    void LongRealNum::fix() {
#ifdef DEBUG_MODE
        if (num.length() < EXPS) {
            std::cerr << std::format("Invalid number size: {}. Begin to traceback.\n", num.length());
            throw std::runtime_error("Invalid number size");
        }
        for (char &i: num) {
            if (!isdigit(i)) {
                std::cerr << std::format("Invalid char: {}. The complete num is {}. Begin to traceback.\n", i, num);
                throw std::runtime_error("Invalid char");
            }
        }
#endif

        while (num.length() > EXPS && num[num.length() - 1] == '0') {
            num.pop_back();
        }
    }

    LongRealNum LongRealNum::operator-() const {
        LongRealNum tmp = *this;
        tmp.sign = !sign;
        return tmp;
    }

    LongRealNum operator+(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.num.length() < lr2.num.length()) {
            return lr2 + lr1;
        }
        if (lr1.sign) {
            return lr2 - (-lr1);
        }
        if (lr2.sign) {
            return lr1 - (-lr2);
        }

        const std::string y(lr2.num);
        std::string x(lr1.num);

        size_t l = y.length();
        for (size_t i = 0; i < l; i++) {
            x[i] = x[i] - '0' + y[i] - '0';
        }
        for (size_t i = l; i < x.length(); i++) {
            x[i] = x[i] - '0';
        }
        l = x.length() - 1;
        for (size_t i = 0; i < l; i++) {
            if (x[i] > 9) {
                x[i + 1] += x[i] / 10;
                x[i] = x[i] % 10;
            }
            x[i] += '0';
        }
        if (x[l] > 9) {
            x += static_cast<char>('0' + x[l] / 10);
            x[l] %= 10;
        }
        x[l] += '0';
        LongRealNum tmp;
#ifdef DEBUG_MODE
        try {
#endif
            tmp.num = x;
            tmp.fix();
#ifdef DEBUG_MODE
        } catch (...) {
            std::cerr << std::format("Something bad happened in LongRealNum::operator+ when try to fix result.\n"
                                     "Between {}\n and {}.\n ", lr1.num, lr2.num);
            throw std::runtime_error("Something bad happened");
        }
#endif

        return tmp;
    }

    LongRealNum operator-(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.sign) {
            return -(-lr1 + lr2);
        }
        if (lr2.sign) {
            return lr1 + (-lr2);
        }
        if (lr1 < lr2) {
            return -(lr2 - lr1);
        }

        const std::string y(lr2.num);
        std::string x(lr1.num);

        size_t l = y.length();
        for (size_t i = 0; i < l; i++) {
            if (x[i] < y[i] && i + 1 < l) {
                x[i] = x[i] + 10 - y[i] + '0';
                x[i + 1] -= 1;
            } else {
                x[i] = x[i] - y[i] + '0';
            }
        }
        for (size_t i = l - 1; i < x.length(); i++) {
#ifdef DEBUG_MODE
            if (i == x.length() - 1 && x[i] < '0') {
                std::cerr << std::format("Something bad happened in LongRealNum::operator-, because lr1 < lr2.\n"
                                         "lr1 is {}\n and \nlr2 is {}.\n ", lr1.num, lr2.num);
                throw std::runtime_error("Something bad happened");
            }
#endif
            if (x[i] < '0') {
                x[i] = x[i] + 10;
                x[i + 1] -= 1;
            }
        }
        LongRealNum tmp;
#ifdef DEBUG_MODE
        try {
#endif
            tmp.num = x;
            tmp.fix();
#ifdef DEBUG_MODE
        } catch (...) {
            std::cerr << std::format("Something bad happened in LongRealNum::operator+ when try to fix result.\n"
                                     "Between {}\n and {}\n "
                                     "Begin to traceback.\n", lr1.num, lr2.num);
            throw std::runtime_error("Something bad happened");
        }
#endif

        return tmp;
    }
    LongRealNum operator*(const LongRealNum &lr1, const long long &lr2) {
        return lr2 * lr1;
    }
    LongRealNum operator*(const long long &lr1, const LongRealNum &lr2) {
        if (lr2.sign) {
            return -(lr1 * (-lr2));
        }
        if (lr1 < 0) {
            return -((-lr1) * lr2);
        }

        std::string x = std::to_string(lr1);
        std::ranges::reverse(x);
        const std::string& y = lr2.num;
        int lx = x.length();
        int ly = y.length();
        std::string tmp(lx + ly + 1, '\x00');

        for (size_t i = 0; i < lx; i++) {
            for (size_t j = 0; j < ly; j++) {
                tmp[i + j] += (x[i] - '0') * (y[j] - '0');
                if (tmp[i + j] > 9) {
                    tmp[i + j + 1] += tmp[i + j] / 10;
                    tmp[i + j] %= 10;
                }
            }
        }
        for (size_t i = 0; i < tmp.length(); i++) {
            if (tmp[i] > 9) {
                tmp[i + 1] += tmp[i] / 10;
                tmp[i] %= 10;
            }
            tmp[i] += '0';
        }

        LongRealNum ret;
#ifdef DEBUG_MODE
        try {
#endif
            ret.num = tmp;
            ret.fix();
#ifdef DEBUG_MODE
        } catch (...) {
            std::cerr << std::format("Something bad happened in LongRealNum::operator* when try to fix result.\n"
                                     "Between {}\n and {}\n "
                                     "Begin to traceback.\n", lr1, lr2.num);
            throw std::runtime_error("Something bad happened");
        }
#endif
        return ret;
    }

    LongRealNum operator*(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr2.sign) {
            return -(lr1 * (-lr2));
        }
        if (lr1.sign) {
            return -((-lr1) * lr2);
        }

        const std::string& x = lr1.num;
        const std::string& y = lr2.num;
        int lx = x.length();
        int ly = y.length();
        std::string tmp(lx + ly + 1, '\x00');

        for (size_t i = 0; i < lx; i++) {
            for (size_t j = 0; j < ly; j++) {
                tmp[i + j] += (x[i] - '0') * (y[j] - '0');
                if (tmp[i + j] > 9) {
                    tmp[i + j + 1] += tmp[i + j] / 10;
                    tmp[i + j] %= 10;
                }
            }
        }
        if (tmp[EXPS - 1] > 4) {
            tmp[EXPS] += 1;
        }
        for (size_t i = 0; i < tmp.length(); i++) {
            if (tmp[i] > 9) {
                tmp[i + 1] += tmp[i] / 10;
                tmp[i] %= 10;
            }
            tmp[i] += '0';
        }
        tmp = tmp.substr(EXPS);
        LongRealNum ret;
#ifdef DEBUG_MODE
        try {
#endif
            ret.num = tmp;
            ret.fix();
#ifdef DEBUG_MODE
        } catch (...) {
            std::cerr << std::format("Something bad happened in LongRealNum::operator* when try to fix result.\n"
                                     "Between {}\n and {}\n "
                                     "Begin to traceback.\n", lr1.num, lr2.num);
            throw std::runtime_error("Something bad happened");
        }
#endif
        return ret;
    }

    bool operator==(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.sign != lr2.sign)
            return false;
        return lr1.num == lr2.num;
    }
    bool operator!=(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.sign != lr2.sign)
            return true;
        return lr1.num != lr2.num;
    }
    bool operator<(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.sign != lr2.sign) {
            return lr1.sign > lr2.sign;
        }
        if (lr1.num.length() != lr2.num.length()) {
            return (lr1.num.length() < lr2.num.length()) ^ lr1.sign;
        }
        for (int i = lr1.num.length() - 1; i >= 0; i--) {
            if (lr1.num[i] != lr2.num[i]) {
                return (lr1.num[i] < lr2.num[i]) ^ lr1.sign;
            }
        }
        return false;
    }
    bool operator>(const LongRealNum &lr1, const LongRealNum &lr2) {
        if (lr1.sign != lr2.sign) {
            return lr1.sign < lr2.sign;
        }
        if (lr1.num.length() != lr2.num.length()) {
            return lr1.num.length() > lr2.num.length() ^ lr1.sign;
        }
        for (int i = lr1.num.length() - 1; i >= 0; i--) {
            if (lr1.num[i] != lr2.num[i]) {
                return lr1.num[i] > lr2.num[i] ^ lr1.sign;
            }
        }
        return false;
    }

    bool operator<=(const LongRealNum &lr1, const LongRealNum &lr2) {
        return !(lr1 > lr2);
    }
    bool operator>=(const LongRealNum &lr1, const LongRealNum &lr2) {
        return !(lr1 < lr2);
    }

    LongRealNum operator/(LongRealNum lr1, LongRealNum lr2) {
        if (lr2.sign) {
            return -(lr1 / (-lr2));
        }
        if (lr1.sign) {
            return -((-lr1) / lr2);
        }

        int cnt1 = 0;
        while(lr1.num[lr1.num.length() - 1] == '0')
            cnt1 += 1;
        lr1.num = lr1.num.substr(0, lr1.num.length() - cnt1);

        int cnt2 = 0;
        while(lr2.num[lr2.num.length() - 1] == '0')
            cnt2 += 1;
        lr2.num = lr2.num.substr(0, lr2.num.length() - cnt2);

        int save = lr1.num.length() - lr2.num.length();

        if (lr1.num.length() > lr2.num.length()) {
            lr2.num = std::string(lr1.num.length() + EXPS + 1 - lr2.num.length(), '0') + lr2.num;
            lr1.num = std::string(EXPS + 1, '0') + lr1.num;
        } else {
            lr1.num = std::string(lr2.num.length() + EXPS + 1 - lr1.num.length(), '0') + lr1.num;
            lr2.num = std::string(EXPS + 1, '0') + lr2.num;
        }
        std::string ans;
        for (int i = save + EXPS; i >= 0; i --) {
            char tmp = '0';
            while (lr1 >= lr2) {
                lr1 = lr1 - lr2;
                tmp += 1;
            }
            ans += tmp;
            lr2.num = lr2.num.substr(1, lr2.num.length());
        }
        if (ans.length() > EXPS) {
            return LongRealNum(ans.insert(ans.length() - EXPS, "."));
        }
        ans = "0." + std::string(EXPS - ans.length(), '0') + ans;
        return LongRealNum(ans);
    }
}

