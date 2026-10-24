#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 单点赋值、区间求和；a[1..n]，{} 或 {0} 表示空树。
// 任意区间和及中间值须在 ll 范围；n<=INT_MAX/4 且内存足够。
struct SegmentTree {
    int n;
    vector<ll> tr;
    explicit SegmentTree(const vector<ll>& a)
        : n(max(0, int(a.size()) - 1)), tr(4ULL * max(1, n)) {
        if (n) build(1, 1, n, a);
    }
    void set(int pos, ll x) { // a[pos]=x；不是单点加。
        assert(1 <= pos && pos <= n);
        update(1, 1, n, pos, x);
    }
    ll sum(int l, int r) const {
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        return query(1, 1, n, l, r);
    }
private:
    void pull(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }
    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int m = l + (r - l) / 2;
        build(p * 2, l, m, a);
        build(p * 2 + 1, m + 1, r, a);
        pull(p);
    }
    void update(int p, int l, int r, int pos, ll x) {
        if (l == r) { tr[p] = x; return; }
        int m = l + (r - l) / 2;
        if (pos <= m) update(p * 2, l, m, pos, x);
        else update(p * 2 + 1, m + 1, r, pos, x);
        pull(p);
    }
    ll query(int p, int l, int r, int L, int R) const {
        if (L <= l && r <= R) return tr[p];
        int m = l + (r - l) / 2;
        ll ans = 0;
        if (L <= m) ans += query(p * 2, l, m, L, R);
        if (R > m) ans += query(p * 2 + 1, m + 1, r, L, R);
        return ans;
    }
};
