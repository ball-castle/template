#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

inline constexpr ll GRID_INF = 1LL << 60;

// a[1..n]，n>=1；选非空连续子段，任意子段和绝对值<2^60。
inline ll max_subarray(const vector<ll>& a) {
    assert(a.size() >= 2);
    ll dp = a[1], ans = a[1];
    for (int i = 2; i < int(a.size()); ++i) {
        // dp：必须以当前位置结尾；只能自己开头或接前一段。
        dp = max(a[i], dp + a[i]);
        ans = max(ans, dp);
    }
    return ans;
}

// g[1..n][1..m]，行列均从1开始；'#'不能走，只能向右/下。
// n,m>=1；mod>=1。dp[j]更新前是上方，dp[j-1]是左方。
inline int grid_paths(const vector<string>& g, int mod = 1000000007) {
    assert(g.size() >= 2 && g[1].size() >= 2 && mod >= 1);
    int n = int(g.size()) - 1, m = int(g[1].size()) - 1;
    vector<ll> dp(m + 1);
    dp[1] = 1 % mod;
    for (int i = 1; i <= n; ++i) {
        assert(int(g[i].size()) == m + 1);
        for (int j = 1; j <= m; ++j) {
            if (g[i][j] == '#') dp[j] = 0;
            else dp[j] = (dp[j] + dp[j - 1]) % mod;
        }
    }
    return int(dp[m]);
}

// 路径代价含起终点；a也1-indexed，可有负数。
// 任意路径和绝对值<2^60；无路返回GRID_INF。
inline ll grid_min_path(const vector<vector<ll>>& a,
                        const vector<string>& g) {
    assert(g.size() >= 2 && g[1].size() >= 2 && a.size() == g.size());
    int n = int(g.size()) - 1, m = int(g[1].size()) - 1;
    vector<ll> dp(m + 1, GRID_INF);
    dp[1] = 0;
    for (int i = 1; i <= n; ++i) {
        assert(int(g[i].size()) == m + 1 && int(a[i].size()) == m + 1);
        for (int j = 1; j <= m; ++j) {
            ll best = min(dp[j], dp[j - 1]);
            dp[j] = g[i][j] == '#' || best == GRID_INF
                  ? GRID_INF : best + a[i][j];
        }
    }
    return dp[m];
}

