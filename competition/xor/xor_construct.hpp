#pragma once
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

// known[1..n] 是已知元素，总 XOR 为 target，求唯一未知元素。
ull xor_missing(const vector<ull>& known, ull target) {
    for(size_t i = 1; i < known.size(); i++) target ^= known[i];
    return target;
}
ull xor_1_to_n(ull n) {
    if(n % 4 == 0) return n;
    if(n % 4 == 1) return 1;
    if(n % 4 == 2) return n + 1;
    return 0;
}
// 1<=n<=1e6，target<2^60；构造互异非负数，全部小于2^62。
// 不承诺正数、较小上界或排列；这些限制必须另证。
bool xor_distinct(int n, ull target, vector<ull>& a) {
    a.clear();
    if(n < 1 || n > 1000000 || target >= (1ULL << 60)) return false;
    if(n == 2 && target == 0) return false;
    a.assign(n + 1, 0);
    if(n <= 2) { a[n] = target; return true; }
    ull last = target;
    for(int i = 1; i <= n - 3; i++) a[i] = ull(i), last ^= a[i];
    a[n - 2] = 1ULL << 60;
    a[n - 1] = 1ULL << 61;
    a[n] = last ^ a[n - 2] ^ a[n - 1];
    return true;
}
// 继承 cf/2/1129/B.cpp 的 mx^mn + 低位全1掩码；0<=l<=r。
ull interval_or(ull l, ull r) {
    ull d = l ^ r, mask = 0;
    while(d) {
        mask = (mask << 1) | 1;
        d >>= 1;
    }
    return r | mask;
}
