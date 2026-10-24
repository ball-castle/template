#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// w 为 1-indexed 方阵；有向图，缺边用 HAMILTON_INF。
// 从1出发，到n结束，每个点恰好一次。n<=18；n=0/1 约定答案0。
// 可以有负边；任意路径的权值总和须在 (-2^60,2^60) 内。
inline constexpr ll HAMILTON_INF = 1LL << 60;
inline ll hamilton_path(const vector<vector<ll>>& w) {
    assert(!w.empty());
    int n = int(w.size()) - 1;
    assert(n <= 18);
    for (const auto& row : w) assert(row.size() == w.size());
    if (n <= 1) return 0;
    int states = 1 << n;
    // 点 i 对应第 i-1 位；dp[mask*n+u] 中 u 是0-indexed。
    // 状态表示已经访问mask、当前停在u+1的最短路。
    vector<ll> dp(size_t(states) * n, HAMILTON_INF);
    dp[n] = 0;  // mask=1，停在点1。
    for (int mask = 1; mask < states; mask += 2) {
        for (int u = 0; u < n; ++u) {
            ll cur = dp[size_t(mask) * n + u];
            if (cur == HAMILTON_INF) continue;
            for (int v = 0; v < n; ++v) {
                if ((mask >> v & 1) || w[u + 1][v + 1] == HAMILTON_INF)
                    continue;
                int next = mask | (1 << v);
                ll& dst = dp[size_t(next) * n + v];
                dst = min(dst, cur + w[u + 1][v + 1]);
            }
        }
    }
    return dp[size_t(states - 1) * n + n - 1];
}
// O(n^2*2^n) 时间、O(n*2^n) 空间；n=18 的dp约36MiB。
// 返回 HAMILTON_INF 表示不存在；求任意终点则改取最后一行最小值。
