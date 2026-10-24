#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "nodes/frequency.inc"
#include "segment_point_base.inc"

// k 从 1 开始；返回第 k 个元素所在下标，非法 k 返回 -1。
// 必须保证所有频率非负，且总频率不超过 LLONG_MAX。
int kth(int node, int l, int r, ll k) {
    if (k <= 0 || k > tree[node].sum) return -1;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    if (k <= tree[node * 2].sum) return kth(node * 2, l, mid, k);
    return kth(node * 2 + 1, mid + 1, r, k - tree[node * 2].sum);
}
