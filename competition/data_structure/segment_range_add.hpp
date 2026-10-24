#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 继承旧 Segment tree2 / 6.9 的全局树与递归写法。
// 非空树，1<=n<N；修改/查询须为 [1,n] 内非空闭区间。
// 所有和、标记、长度*增量及中间值须在 ll 范围。
const int N = 200000 + 5;
struct Node { ll sum, mn, mx; };
ll a[N], lazy[N * 4];
Node tree[N * 4];
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum, min(x.mn, y.mn), max(x.mx, y.mx)};
}
void pushup(int node) {
    tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
}
void apply(int node, int l, int r, ll val) {
    tree[node].sum += (r - l + 1LL) * val;
    tree[node].mn += val;
    tree[node].mx += val;
    lazy[node] += val;
}
void pushdown(int node, int l, int r) {
    if (lazy[node] == 0 || l == r) return;
    int mid = (l + r) >> 1;
    apply(node * 2, l, mid, lazy[node]);
    apply(node * 2 + 1, mid + 1, r, lazy[node]);
    lazy[node] = 0;
}
void build(int node, int l, int r) {
    lazy[node] = 0;
    if (l == r) {
        tree[node] = {a[l], a[l], a[l]};
        return;
    }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void modify(int node, int l, int r, int L, int R, ll val) {
    if (L <= l && r <= R) {
        apply(node, l, r, val);
        return;
    }
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(node * 2, l, mid, L, R, val);
    if (R > mid) modify(node * 2 + 1, mid + 1, r, L, R, val);
    pushup(node);
}
Node query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    pushdown(node, l, r);
    int mid = (l + r) >> 1;
    if (R <= mid) return query(node * 2, l, mid, L, R);
    if (L > mid) return query(node * 2 + 1, mid + 1, r, L, R);
    return merge(query(node * 2, l, mid, L, R),
                 query(node * 2 + 1, mid + 1, r, L, R));
}
