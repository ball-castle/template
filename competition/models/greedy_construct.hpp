#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;

// seg[1..n]为非空半开区间[l,r)，l<r；端点相接允许同时选择。
// 返回数量最多的不相交区间原下标，按被选择的时间顺序排列。
vector<int> interval_schedule(const vector<pair<ll,ll>>& seg) {
    assert(!seg.empty());
    int n = (int)seg.size() - 1;
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    for(int i = 1; i <= n; i++) assert(seg[i].first < seg[i].second);
    sort(p.begin(), p.end(), [&](int x, int y) {
        return tie(seg[x].second, seg[x].first, x)
             < tie(seg[y].second, seg[y].first, y);
    });
    vector<int> ans;
    for(int id : p) {
        if(ans.empty() || seg[id].first >= seg[ans.back()].second)
            ans.push_back(id);
    }
    return ans;
}

// a[1..n]，n为偶数，所有元素恰好配对；最小化最大配对和。
// pairs输出数值对（重复值保留）；空数组约定答案为0。和用i128。
i128 minmax_pair_sum(vector<ll> a, vector<pair<ll,ll>>& pairs) {
    assert(!a.empty() && (a.size() - 1) % 2 == 0);
    sort(a.begin() + 1, a.end());
    pairs.clear();
    int n = (int)a.size() - 1;
    i128 ans = 0;
    for(int l = 1, r = n; l < r; l++, r--) {
        i128 sum = (i128)a[l] + a[r];
        if(pairs.empty() || sum > ans) ans = sum;
        pairs.push_back({a[l], a[r]});
    }
    return ans;
}

// 空串合法；只支持一类括号()，其他字符返回false。
bool regular_brackets(const string& s) {
    size_t balance = 0;
    for(char c : s) {
        if(c == '(') balance++;
        else if(c == ')') {
            if(balance == 0) return false;
            balance--;
        } else return false;
    }
    return balance == 0;
}
