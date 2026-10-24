#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// 绝对值用无符号保存，避免 -LLONG_MIN 的有符号溢出。
inline ull unsigned_abs(ll x) {
    return x < 0 ? 0ULL - ull(x) : ull(x);
}
inline ull gcd_abs(ll a, ll b) {
    ull x = unsigned_abs(a), y = unsigned_abs(b);
    while (y) { ull r = x % y; x = y; y = r; }
    return x;  // gcd(0,0)=0；结果可能为2^63，不能总转回ll。
}
inline optional<ll> lcm_checked(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ull x = unsigned_abs(a), y = unsigned_abs(b);
    __int128 z = (__int128)(x / gcd_abs(a, b)) * y;
    if (z > LLONG_MAX) return nullopt;
    return ll(z);
}

// 位编号从0开始；无符号运算可以安全处理第63位。
inline ull lowbit(ull x) { return x & (0ULL - x); }
inline int bit_count(ull x) { return popcount(x); }
inline int lowest_bit_index(ull x) {
    return x == 0 ? -1 : int(countr_zero(x));
}
// 第k位：1ULL<<k，要求0<=k<64；不可写1<<k处理高位。
// 判断位：(mask>>k)&1ULL；设置位：mask|=(1ULL<<k)。
// 枚举全部子集(含0)，顺序从大到小，O(2^popcount(mask))：
// for (ull sub=mask;; sub=(sub-1)&mask) {
//     ... 使用sub ...
//     if (sub==0) break;  // 必须先终止，否则又回到mask。
// }
