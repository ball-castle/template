#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 区间加、区间和；a[1..n]，{} 或 {0} 为空；n<=INT_MAX/4。
// 节点和、标记累加、长度*x 及所有中间值必须在 ll 范围。
struct LazySegmentTree {
    int n;
    vector<ll> tr, tag;
    explicit LazySegmentTree(const vector<ll>& a)
        : n(max(0, int(a.size()) - 1)), tr(4ULL * max(1, n)), tag(tr.size()) {
        if (n) build(1, 1, n, a);
    }
    void add(int l, int r, ll x) {
        if (l > r) return; // 空区间不修改，包括 n=0。
        assert(1 <= l && r <= n);
        modify(1, 1, n, l, r, x);
    }
    ll sum(int l, int r) {
        if (l > r) return 0;
        assert(1 <= l && r <= n);
        return query(1, 1, n, l, r);
    }
private:
    void pull(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }
    void apply(int p, int l, int r, ll x) {
        tr[p] += (r - l + 1LL) * x;
        tag[p] += x;
    }
    void push(int p, int l, int r) {
        if (!tag[p] || l == r) return;
        int m = l + (r - l) / 2;
        apply(p * 2, l, m, tag[p]);
        apply(p * 2 + 1, m + 1, r, tag[p]);
        tag[p] = 0;
    }
    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int m = l + (r - l) / 2;
        build(p * 2, l, m, a);
        build(p * 2 + 1, m + 1, r, a);
        pull(p);
    }
    void modify(int p, int l, int r, int L, int R, ll x) {
        if (L <= l && r <= R) { apply(p, l, r, x); return; }
        push(p, l, r);
        int m = l + (r - l) / 2;
        if (L <= m) modify(p * 2, l, m, L, R, x);
        if (R > m) modify(p * 2 + 1, m + 1, r, L, R, x);
        pull(p);
    }
    ll query(int p, int l, int r, int L, int R) {
        if (L <= l && r <= R) return tr[p];
        push(p, l, r); // 查询也必须下传，否则子节点仍是旧值。
        int m = l + (r - l) / 2;
        ll ans = 0;
        if (L <= m) ans += query(p * 2, l, m, L, R);
        if (R > m) ans += query(p * 2 + 1, m + 1, r, L, R);
        return ans;
    }
};
