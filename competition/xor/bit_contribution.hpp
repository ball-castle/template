#pragma once
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
using u128 = __uint128_t;

// a[1..n]，返回所有 i<j 的 XOR和、OR和、AND和。
// n <= 1e9，元素为 uint64；答案使用 GNU unsigned __int128。
array<u128, 3> pair_bit_sums(const vector<ull>& a) {
    u128 n = a.size() - 1;
    array<u128, 3> ans{};
    for(int k = 0; k < 64; k++) {
        u128 one = 0;
        for(size_t i = 1; i < a.size(); i++) one += (a[i] >> k) & 1;
        u128 zero = n - one, bit = u128(1) << k;
        u128 both = 0;
        if(one >= 2) both = one * (one - 1) / 2;
        ans[0] += one * zero * bit;
        ans[1] += (one * zero + both) * bit;
        ans[2] += both * bit;
    }
    return ans;
}
// 所有子数组 XOR 值的普通加法和；统计 n+1 个前缀中不同位数对。
u128 subarray_xor_sum(const vector<ull>& a) {
    ull cnt[64][2]{};
    for(int k = 0; k < 64; k++) cnt[k][0] = 1;
    ull p = 0;
    for(size_t i = 1; i < a.size(); i++) {
        p ^= a[i];
        for(int k = 0; k < 64; k++) cnt[k][(p >> k) & 1]++;
    }
    u128 ans = 0;
    for(int k = 0; k < 64; k++)
        ans += u128(cnt[k][0]) * cnt[k][1] * (u128(1) << k);
    return ans;
}
// 将所有子数组 XOR 值再 XOR；第 i 个元素出现 i*(n-i+1) 次。
ull all_subarray_xor(const vector<ull>& a) {
    ull ans = 0;
    size_t n = a.size() - 1;
    for(size_t i = 1; i <= n; i++)
        if((i & 1) && ((n - i + 1) & 1)) ans ^= a[i];
    return ans;
}
string to_string_u128(u128 x) {
    string s;
    do { s += char('0' + x % 10); x /= 10; } while(x);
    reverse(s.begin(), s.end());
    return s;
}
