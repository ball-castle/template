#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 保留旧 Fenwick 的 n、tr、add 和 lowbit 循环。
// 1 下标；add 是增量，query(x) 是前缀，rangeSum(l,r) 是闭区间。
// 0<=n<2^30 且内存足够；所有节点、前缀及中间值须在 ll 范围。
struct Fenwick {
    int n;
    vector<ll> tr;
    explicit Fenwick(int size) : n(size), tr(size + 1) {
        assert(size >= 0);
    }
    void add(int x, ll val) {
        assert(1 <= x && x <= n);
        for (; x <= n; x += x & -x) tr[x] += val;
    }
    ll query(int x) const {
        assert(0 <= x && x <= n);
        ll res = 0;
        for (; x; x -= x & -x) res += tr[x];
        return res;
    }
    ll rangeSum(int l, int r) const {
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        return query(r) - query(l - 1);
    }
};
