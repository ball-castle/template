#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 单点加、区间和。n>=0，1 下标；所有树节点/前缀/答案须在 ll 范围。
// 适用于可相减的前缀信息，不能直接把求和改成区间最小值。
struct Fenwick {
    int n;
    vector<ll> bit;
    explicit Fenwick(int size = 0) : n(size), bit(size + 1) {
        assert(size >= 0);
    }
    void add(int p, ll x) {
        assert(1 <= p && p <= n); // p=0 会使 lowbit 循环无法前进。
        while (p <= n) {
            bit[p] += x;
            int step = p & -p;
            if (step > n - p) break; // 避免索引加法越过 int 上界。
            p += step;
        }
    }
    ll sum(int p) const { // [1,p]，允许 p=0。
        assert(0 <= p && p <= n);
        ll ans = 0;
        for (; p; p -= p & -p) ans += bit[p];
        return ans;
    }
    ll range_sum(int l, int r) const { // [l,r]；空区间返回 0。
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        return sum(r) - sum(l - 1);
    }
};
