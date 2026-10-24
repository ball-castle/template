#pragma once
#include "../../core/basics.hpp"

// cin先读字符串；s为合法十进制整数，可带+/-，数值在有符号i128范围。
// 普通ll仍直接使用原read()，这里只补i128的字符串转换。
inline __int128 parse_i128(const string& s) {
    int i = 0;
    bool neg = s[0] == '-';
    if (s[0] == '+' || s[0] == '-') ++i;
    __uint128_t value = 0;
    for (; i < int(s.size()); ++i) value = value * 10 + (s[i] - '0');
    if (neg && value) return -(__int128)(value - 1) - 1;
    return (__int128)value;
}
