#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// a 为1-indexed非负石子堆。每次只能合并相邻两堆，代价为两堆之和。
// 求全部合成一堆的最小总代价；n=0/1返回0；不是环形合并。
// 要求 n*sum(a)<2^60，使前缀和、候选代价及无穷大不混淆。
inline ll interval_merge(const vector<ll>& a) {
    assert(!a.empty());
    int n = int(a.size()) - 1;
    if (n <= 1) return 0;
    vector<ll> sum(n + 1);
    for (int i = 1; i <= n; ++i) {
        assert(a[i] >= 0);
        sum[i] = sum[i - 1] + a[i];
    }
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1));
    // dp[l][r]：合并闭区间[l,r]；先枚举长度，保证子区间已算出。
    for (int len = 2; len <= n; ++len)
        for (int l = 1, r = len; r <= n; ++l, ++r) {
            dp[l][r] = 1LL << 60;
            for (int k = l; k < r; ++k)
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]
                               + sum[r] - sum[l - 1]);
        }
    return dp[1][n];
}
// 时间O(n^3)，空间O(n^2)；n达到千级时应重新评估是否需要优化。
// vector<ll> a(n+1); 读a[1..n]；cout << interval_merge(a);
