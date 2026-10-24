#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;
struct IntervalOp { int l, r, sign; i128 cnt; };
// 输出压缩操作：[l,r] 加 sign，重复cnt次；至多n组，不逐次展开。
// a[0]=0。mode=1只加，-1只减，0任意；空结果也可能是合法全0目标。
optional<vector<IntervalOp>> construct_unit_intervals(
        const vector<ll>& a, int mode = 0) {
    assert(!a.empty() && -1 <= mode && mode <= 1);
    vector<pair<int,i128>> pos, neg;
    int n = (int)a.size() - 1;
    for(int i = 1; i <= n; i++) {
        if((mode == 1 && a[i] < 0) || (mode == -1 && a[i] > 0))
            return nullopt;
    }
    for(int i = 1; i <= n + 1; i++) {
        i128 d = (i <= n ? (i128)a[i] : 0) - a[i - 1];
        if(d > 0) pos.push_back({i,d});
        if(d < 0) neg.push_back({i,-d});
    }
    vector<IntervalOp> ans;
    int p = 0, q = 0;
    while(p < (int)pos.size()) {
        auto &x = pos[p]; auto &y = neg[q];
        i128 cnt = min(x.second, y.second);
        if(x.first < y.first) ans.push_back({x.first,y.first-1,1,cnt});
        else ans.push_back({y.first,x.first-1,-1,cnt});
        x.second -= cnt; y.second -= cnt;
        if(x.second == 0) p++;
        if(y.second == 0) q++;
    }
    return ans;
}
// 每次区间加任意整数，费用固定为1：不能套单位操作公式。
// 仅用于非零差分个数m<=20：精确子集DP，O(m*2^m)，空间O(2^m)。
int arbitrary_intervals_small(const vector<ll>& a) {
    assert(!a.empty());
    vector<i128> d;
    int n = (int)a.size() - 1;
    for(int i = 1; i <= n + 1; i++) {
        i128 x = (i <= n ? (i128)a[i] : 0) - a[i - 1];
        if(x != 0) d.push_back(x);
    }
    int m = (int)d.size(); assert(m <= 20);
    int all = 1 << m;
    vector<i128> sum(all);
    vector<int> dp(all);
    for(int mask = 1; mask < all; mask++) {
        int bit = __builtin_ctz((unsigned)mask);
        sum[mask] = sum[mask ^ (1 << bit)] + d[bit];
        for(int i = 0; i < m; i++) if(mask >> i & 1)
            dp[mask] = max(dp[mask], dp[mask ^ (1 << i)]);
        if(sum[mask] == 0) dp[mask]++;
    }
    return m - dp.back();
}
