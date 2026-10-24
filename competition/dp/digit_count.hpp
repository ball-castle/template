#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 原date/8/1/2_digit_dp.cpp的全局s/dp与递归lambda保持。
// 数0也算；最多3个非零数位。每次上界变化都清缓存。
inline string s;
inline ll dp[20][4];
inline ll digit_calc(ll x) {
    if (x < 0) return 0;
    s = to_string(x);
    memset(dp, -1, sizeof(dp));
    auto dfs = [&](auto&& self, int pos, int cnt, bool limit) -> ll {
        if (cnt > 3) return 0;
        if (pos == int(s.size())) return 1;
        if (!limit && dp[pos][cnt] != -1) return dp[pos][cnt];
        int up = limit ? s[pos] - '0' : 9;
        ll ans = 0;
        for (int d = 0; d <= up; ++d)
            ans += self(self, pos + 1, cnt + (d != 0),
                        limit && d == up);
        if (!limit) dp[pos][cnt] = ans;
        return ans;
    };
    return dfs(dfs, 0, 0, true);
}
inline ll digit_range(ll l, ll r) {
    assert(0 <= l && l <= r); // 0<=l<=r<=LLONG_MAX，l-1不会溢出。
    return digit_calc(r) - digit_calc(l - 1);
}

// 长度n的01串，不允许相邻两个1；空串计1种，模mod>=1。
// dp[0/1]：当前前缀末位为0/1的方案数，空前缀放入状态0。
inline int binary_no_adjacent(int n, int mod = 1000000007) {
    assert(n >= 0 && mod >= 1);
    if (mod == 1) return 0;
    array<ll, 2> f = {1 % mod, 0};
    for (int i = 0; i < n; ++i) {
        array<ll, 2> ndp = {(f[0] + f[1]) % mod, f[0]};
        f = ndp;
    }
    return int((f[0] + f[1]) % mod);
}

