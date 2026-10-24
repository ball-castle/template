#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// dp[i][j]：s的前i个字符、t的前j个字符的最长公共子序列长度。
inline int lcs_length(const string& s, const string& t) {
    int n = int(s.size()), m = int(t.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) {
            if (s[i - 1] == t[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    return dp[n][m];
}

// 插入、删除、替换各花1；dp[i][j]：s前i个变成t前j个的最少操作。
inline int edit_distance(const string& s, const string& t) {
    int n = int(s.size()), m = int(t.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1,
                dp[i - 1][j - 1] + (s[i - 1] != t[j - 1])});
    return dp[n][m];
}

// 从s任意位置开始，只能先向右、再向左，最多转向一次，匹配t。
// 保留cf/date/7/7.9/1_interesting_dp.cpp的dp0/dp1递推。
// 空t约定可匹配；字符串下标遵从原稿0-indexed。
inline bool right_then_left_match(const string& s, const string& t) {
    if (t.empty()) return true;
    int n = int(s.size()), m = int(t.size());
    vector<int> dp0(n), dp1(n); // 尚未向左 / 已经向左
    for (int i = 0; i < n; ++i) dp0[i] = s[i] == t[0];
    for (int j = 1; j < m; ++j) {
        vector<int> ndp0(n), ndp1(n);
        for (int i = 0; i < n; ++i) {
            if (dp0[i]) {
                if (i + 1 < n && s[i + 1] == t[j]) ndp0[i + 1] = 1;
                if (i > 0 && s[i - 1] == t[j]) ndp1[i - 1] = 1;
            }
            if (dp1[i] && i > 0 && s[i - 1] == t[j]) ndp1[i - 1] = 1;
        }
        dp0 = move(ndp0); dp1 = move(ndp1);
    }
    for (int i = 0; i < n; ++i) if (dp0[i] || dp1[i]) return true;
    return false;
}

// 保留JAKARTA_2018/J.cpp：非空子序列排序去重 + 前缀最大值。
// 单个字符串长度<=15；每个候选按字典序严格递增，最大化总长度。
inline vector<string> nonempty_subsequences(const string& s) {
    int n = int(s.size());
    assert(n <= 15);
    vector<string> v;
    for (int mask = 1; mask < (1 << n); ++mask) {
        string t;
        for (int i = 0; i < n; ++i) if (mask >> i & 1) t += s[i];
        v.push_back(t);
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v;
}
inline ll lex_subsequence_chain(const vector<string>& a) {
    assert(!a.empty()); // a[1..n]；无对象传{""}。
    if (a.size() == 1) return 0;
    const ll NEG = -(1LL << 60);
    auto pre = nonempty_subsequences(a[1]);
    vector<ll> dp(pre.size());
    for (int i = 0; i < int(pre.size()); ++i) dp[i] = pre[i].size();
    for (int i = 2; i < int(a.size()); ++i) {
        auto cur = nonempty_subsequences(a[i]);
        vector<ll> ndp(cur.size(), NEG);
        int p = 0;
        ll best = NEG;
        for (int j = 0; j < int(cur.size()); ++j) {
            while (p < int(pre.size()) && pre[p] < cur[j]) {
                best = max(best, dp[p]);
                ++p;
            }
            if (best != NEG) ndp[j] = best + ll(cur[j].size());
        }
        pre = move(cur); dp = move(ndp);
    }
    ll ans = NEG;
    for (ll x : dp) ans = max(ans, x);
    return ans == NEG ? -1 : ans;
}

