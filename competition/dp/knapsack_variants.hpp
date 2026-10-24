#pragma once
#include "../../dp/knapsack.hpp"

// w/v/cnt从1开始；w[i]>0，cnt[i]>=0，cap>=0。
// 契约沿用DP01：价值与任意可达总价值绝对值<2^60。
inline vector<ll> knapsack_bounded(const vector<int>& w,
                                  const vector<ll>& v,
                                  const vector<int>& cnt, int cap,
                                  bool exact = false) {
    assert(cap >= 0 && !w.empty());
    assert(w.size() == v.size() && w.size() == cnt.size());
    vector<int> a(1);
    vector<ll> b(1);
    for (int i = 1; i < int(w.size()); ++i) {
        assert(w[i] > 0 && cnt[i] >= 0);
        int rest = min(cnt[i], cap / w[i]);
        // 二进制拆成0/1包；最后剩余的一包使0..cnt全部可表示。
        for (ll k = 1; rest > 0; k *= 2) {
            int take = int(min<ll>(k, rest));
            a.push_back(w[i] * take);
            b.push_back(v[i] * take);
            rest -= take;
        }
    }
    return knapsack01(a, b, cap, exact); // 复用已验证DP01
}

// groups[1..n]；每组若干{重量,价值}，最多选一个，也能整组不选。
// 组内空集合合法；必须从old转移，不能读本组刚更新的dp。
inline vector<ll> knapsack_grouped(
        const vector<vector<pair<int, ll>>>& groups, int cap,
        bool exact = false) {
    assert(!groups.empty() && cap >= 0);
    vector<ll> dp(cap + 1, exact ? KNAP_NEG : 0);
    dp[0] = 0;
    for (int i = 1; i < int(groups.size()); ++i) {
        auto old = dp;
        for (auto [w, v] : groups[i]) {
            assert(w > 0);
            for (int j = w; j <= cap; ++j)
                if (old[j - w] != KNAP_NEG)
                    dp[j] = max(dp[j], old[j - w] + v);
        }
    }
    return dp;
}

