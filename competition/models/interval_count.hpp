#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;
// a[1..n]，a[0]=0。中间值和操作次数用 i128。
// mode=1 只加1；-1 只减1；0 两者均可。无其他中间状态限制。
optional<i128> unit_intervals(const vector<ll>& a, int mode = 0) {
    assert(!a.empty() && -1 <= mode && mode <= 1);
    i128 ans = 0, pre = 0;
    for(int i = 1; i < (int)a.size(); i++) {
        if((mode == 1 && a[i] < 0) || (mode == -1 && a[i] > 0))
            return nullopt;
        i128 d = (i128)a[i] - pre;
        if(d > 0) ans += d;
        pre = a[i];
    }
    if(pre < 0) ans -= pre; // 补上最后边界 d[n+1]=-a[n]
    return ans;
}
// 只能操作前缀/后缀，每次可加1或减1。
i128 prefix_units(const vector<ll>& a) {
    i128 ans = 0, next = 0;
    for(int i = (int)a.size() - 1; i >= 1; i--) {
        i128 d = (i128)a[i] - next;
        ans += d < 0 ? -d : d;
        next = a[i];
    }
    return ans;
}
i128 suffix_units(const vector<ll>& a) {
    i128 ans = 0, pre = 0;
    for(int i = 1; i < (int)a.size(); i++) {
        i128 d = (i128)a[i] - pre;
        ans += d < 0 ? -d : d;
        pre = a[i];
    }
    return ans;
}
// 从全0二进制数组出发，任意区间按位翻转：答案是连续1段数。
int binary_interval_flips(const vector<int>& a) {
    int ans = 0, pre = 0;
    for(int i = 1; i < (int)a.size(); i++) {
        assert(a[i] == 0 || a[i] == 1);
        if(a[i] == 1 && pre == 0) ans++;
        pre = a[i];
    }
    return ans;
}
// 相邻位置转移1单位；允许两个方向。总和相同才可达。
optional<i128> adjacent_moves(const vector<ll>& a, const vector<ll>& b) {
    assert(a.size() == b.size());
    i128 pre = 0, ans = 0;
    for(int i = 1; i < (int)a.size(); i++) {
        pre += (i128)a[i] - b[i];
        if(i + 1 < (int)a.size()) ans += pre < 0 ? -pre : pre;
    }
    if(pre != 0) return nullopt;
    return ans;
}
