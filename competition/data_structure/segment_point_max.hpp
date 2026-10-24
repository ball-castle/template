#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "nodes/max.inc"
#include "segment_point_base.inc"

// 在闭区间 [L,R] 中找第一个 a[pos]>=val；不存在或 L>R 返回 -1。
int first_ge(int node, int l, int r, int L, int R, ll val) {
    if (L > R || R < l || r < L || tree[node].mx < val) return -1;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    int pos = first_ge(node * 2, l, mid, L, R, val);
    if (pos != -1) return pos;
    return first_ge(node * 2 + 1, mid + 1, r, L, R, val);
}
