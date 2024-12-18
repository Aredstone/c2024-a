//
// Created by a1823 on 24-12-17.
//

#ifndef LONGREALNUM_H
#define LONGREALNUM_H


#include <string>
namespace pi_task {
    inline int EXPS = 1000;
    class LongRealNum {
    public:
        LongRealNum();
        explicit LongRealNum(const std::string& s);
        explicit LongRealNum(long long l1, long long l2);
        explicit LongRealNum(long long l);
        explicit LongRealNum(int i);

        LongRealNum operator-() const;

        friend std::ostream& operator<<(std::ostream& os, const LongRealNum& lr);

        friend LongRealNum operator+(const LongRealNum& lr1, const LongRealNum& lr2);
        friend LongRealNum operator-(const LongRealNum& lr1, const LongRealNum& lr2);
        friend LongRealNum operator*(const LongRealNum& lr1, const LongRealNum& lr2);
        friend LongRealNum operator*(const long long& lr1, const LongRealNum& lr2);
        friend LongRealNum operator*(const LongRealNum& lr2, const long long& lr1);
        friend LongRealNum operator/(LongRealNum lr1, LongRealNum lr2);
        friend bool operator==(const LongRealNum& lr1, const LongRealNum& lr2);
        friend bool operator!=(const LongRealNum& lr1, const LongRealNum& lr2);
        friend bool operator<(const LongRealNum& lr1, const LongRealNum& lr2);
        friend bool operator>(const LongRealNum& lr1, const LongRealNum& lr2);
        friend bool operator<=(const LongRealNum& lr1, const LongRealNum& lr2);
        friend bool operator>=(const LongRealNum& lr1, const LongRealNum& lr2);

    private:
        std::string num;
        bool sign{};
        void fix();
    };
}


#endif //LONGREALNUM_H
