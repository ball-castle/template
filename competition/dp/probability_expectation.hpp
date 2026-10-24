#pragma once
#include <bits/stdc++.h>
using namespace std;

// p[1..n]是相互独立的伯努利试验成功概率，0<=p[i]<=1且有限。
// 返回dp[k]：恰有k次成功的概率；空试验dp[0]=1。
inline vector<long double> coin_heads_probability(
        const vector<long double>& p) {
    assert(!p.empty());
    int n = int(p.size()) - 1;
    vector<long double> dp(n + 1);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) {
        assert(isfinite(p[i]) && 0 <= p[i] && p[i] <= 1);
        for (int j = i; j >= 0; --j)
            dp[j] = dp[j] * (1 - p[i]) + (j ? dp[j - 1] * p[i] : 0);
    }
    return dp;
}

// 按顺序完成n阶段。阶段i每次独立尝试成功概率固定为p[i]，
// 成功去i+1，失败留i，每次耗时1。要求0<p[i]<=1，sum(1/p)有限。
// E[i]=1+(1-p[i])*E[i]+p[i]*E[i+1]，移项得到下式。
inline long double expected_stage_attempts(const vector<long double>& p) {
    assert(!p.empty());
    int n = int(p.size()) - 1;
    vector<long double> dp(n + 2);
    for (int i = n; i >= 1; --i) {
        assert(isfinite(p[i]) && 0 < p[i] && p[i] <= 1);
        dp[i] = dp[i + 1] + 1 / p[i];
    }
    return dp[1];
}

