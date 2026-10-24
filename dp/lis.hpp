#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// a 为 1-indexed；仅求长度。strict=true 严格递增，否则非递减。
// 最直观状态：dp[i] 为以 a[i] 结尾的最优长度，O(n^2)。
inline int lis_quadratic(const vector<ll>& a, bool strict = true) {
    assert(!a.empty());
    int n = int(a.size()) - 1, ans = 0;
    vector<int> dp(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j < i; ++j) {
            bool can = strict ? a[j] < a[i] : a[j] <= a[i];
            if (can) dp[i] = max(dp[i], dp[j] + 1);
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}

// tails[k]：长度 k+1 的序列中，最小的末尾值；它不是答案序列。
// 严格递增用 lower_bound，非递减用 upper_bound；O(n log n)。
inline int lis_length(const vector<ll>& a, bool strict = true) {
    assert(!a.empty());
    vector<ll> tails;
    for (int i = 1; i < int(a.size()); ++i) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), a[i])
                         : upper_bound(tails.begin(), tails.end(), a[i]);
        if (it == tails.end()) tails.push_back(a[i]);
        else *it = a[i];
    }
    return int(tails.size());
}
// vector<ll> a(n+1); 读入 a[1..n]；cout << lis_length(a);
