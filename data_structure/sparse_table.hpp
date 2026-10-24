#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 静态区间查询；op 必须满足结合律、幂等性，如 min/max/gcd。
// 不支持修改，也不能用两块重叠区间求和。GcdOp 要求输入非负。
struct MinOp {
    ll operator()(ll a, ll b) const { return min(a, b); }
};
struct GcdOp {
    ll operator()(ll a, ll b) const { return gcd(a, b); }
};
template<class Op>
struct SparseTable {
    int n;
    vector<int> lg;
    vector<vector<ll>> st;
    ll identity;
    Op op;
    // a[1..n]，{}、{0} 均为空；empty_value 是空区间返回值。
    SparseTable(const vector<ll>& a, ll empty_value, Op merge = Op{})
        : n(max(0, int(a.size()) - 1)), lg(n + 1),
          identity(empty_value), op(merge) {
        for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
        st.assign(lg[n] + 1, vector<ll>(n + 1));
        for (int i = 1; i <= n; ++i) st[0][i] = a[i];
        for (int j = 1; j <= lg[n]; ++j) {
            for (int i = 1; i <= n - (1 << j) + 1; ++i) {
                st[j][i] = op(st[j - 1][i],
                              st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    ll query(int l, int r) const {
        if (l > r) return identity;
        assert(1 <= l && r <= n);
        int k = lg[r - l + 1];
        return op(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
