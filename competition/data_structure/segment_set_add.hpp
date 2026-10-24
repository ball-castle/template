#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 在旧区间加树上扩展赋值；true=赋值，false=加。
// 非空树，1<=n<N；区间须合法；所有中间值须在 ll 范围。
const int N = 200000 + 5;
struct Tag { bool has_set; ll set, add; };
ll a[N], tree[N * 4];
Tag lazy[N * 4];
void pushup(int node) {
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}
void apply(int node, int l, int r, ll val, bool is_set) {
    if (is_set) {
        tree[node] = (r - l + 1LL) * val;
        lazy[node] = {true, val, 0}; // 新赋值覆盖之前的赋值和增量。
    } else {
        tree[node] += (r - l + 1LL) * val;
        lazy[node].add += val;
    }
}
void pushdown(int node, int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (lazy[node].has_set) { // 下传必须先 set，再 add。
        apply(node * 2, l, mid, lazy[node].set, true);
        apply(node * 2 + 1, mid + 1, r, lazy[node].set, true);
    }
    if (lazy[node].add != 0) {
        apply(node * 2, l, mid, lazy[node].add, false);
        apply(node * 2 + 1, mid + 1, r, lazy[node].add, false);
    }
    lazy[node] = {};
}
void build(int node, int l, int r) {
    lazy[node] = {};
    if (l == r) { tree[node] = a[l]; return; }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void modify(int node, int l, int r, int L, int R, ll val, bool is_set) {
    if (L <= l && r <= R) {
        apply(node, l, r, val, is_set);
        return;
    }
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(node * 2, l, mid, L, R, val, is_set);
    if (R > mid) modify(node * 2 + 1, mid + 1, r, L, R, val, is_set);
    pushup(node);
}
ll query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    ll ans = 0;
    if (L <= mid) ans += query(node * 2, l, mid, L, R);
    if (R > mid) ans += query(node * 2 + 1, mid + 1, r, L, R);
    return ans;
}
