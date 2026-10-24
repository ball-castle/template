#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// w、v 为 1-indexed；w[i]>0，cap>=0，价值及可达总和绝对值<2^60。
// exact=false：总重量<=j；exact=true：总重量恰好为j。
// 不可达为 KNAP_NEG；不能直接对不可达状态加价值。
inline constexpr ll KNAP_NEG = -(1LL << 60);
inline vector<ll> knapsack01(const vector<int>& w,
                             const vector<ll>& v, int cap,
                             bool exact = false) {
    assert(cap >= 0 && !w.empty() && w.size() == v.size());
    vector<ll> dp(cap + 1, exact ? KNAP_NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < int(w.size()); ++i) {
        assert(w[i] > 0);
        // 倒序：dp[j-w[i]] 仍属于上一轮，物品最多取一次。
        for (int j = cap; j >= w[i]; --j)
            if (dp[j - w[i]] != KNAP_NEG)
                dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
    return dp;
}

inline vector<ll> knapsack_complete(const vector<int>& w,
                                    const vector<ll>& v, int cap,
                                    bool exact = false) {
    assert(cap >= 0 && !w.empty() && w.size() == v.size());
    vector<ll> dp(cap + 1, exact ? KNAP_NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < int(w.size()); ++i) {
        assert(w[i] > 0);
        // 正序：dp[j-w[i]] 可以已经取过本物品，允许继续取。
        for (int j = w[i]; j <= cap; ++j)
            if (dp[j - w[i]] != KNAP_NEG)
                dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
    return dp;
}
// auto best = knapsack01(w, v, capacity, true);
// best[capacity]==KNAP_NEG 表示不能恰好装满；否则就是最大价值。
