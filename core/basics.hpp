#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 数组 a[1..n]；{} 或 {0} 表示 n=0。所有 ll 中间结果须不溢出。
inline string to_string_i128(__int128 x) {
    bool neg = x < 0;
    // 无符号取反能处理 -2^127，禁止直接对最小有符号数取负。
    __uint128_t u = neg ? -(__uint128_t)x : (__uint128_t)x;
    string s;
    do { s += char('0' + u % 10); u /= 10; } while (u);
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    return s;
}
struct PrefixSum {
    int n;
    vector<ll> pre;
    explicit PrefixSum(const vector<ll>& a)
        : n(max(0, int(a.size()) - 1)), pre(n + 1) {
        for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] + a[i];
    }
    ll sum(int l, int r) const {
        if (l > r) return 0; // 空区间；非空时 1<=l<=r<=n。
        assert(1 <= l && r <= n);
        return pre[r] - pre[l - 1];
    }
};
// 离线区间加；初值全 0。values() 得到 1 下标数组，可重复调用。
struct Difference {
    int n;
    vector<ll> d;
    explicit Difference(int size = 0) : n(size), d(size + 1) {
        assert(size >= 0);
    }
    void add(int l, int r, ll x) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        d[l] += x;
        if (r < n) d[r + 1] -= x;
    }
    vector<ll> values() const {
        vector<ll> a(n + 1);
        for (int i = 1; i <= n; ++i) a[i] = a[i - 1] + d[i];
        return a;
    }
};
// 离散化保序不保距离。rank(x) 只接收初始化时存在的值。
struct Compression {
    vector<ll> values; // values[rank] 为原值；values[0] 不使用。
    explicit Compression(const vector<ll>& a) {
        values.assign(a.begin() + min<size_t>(1, a.size()), a.end());
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        values.insert(values.begin(), 0);
    }
    int rank(ll x) const {
        auto it = lower_bound(values.begin() + 1, values.end(), x);
        assert(it != values.end() && *it == x);
        return int(it - values.begin());
    }
};
