#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 200010;
ll a[N], b[N];
// a[1..n]，n<N；返回逆序对数，同时把a排序。
ll merge_count(int l, int r) {
    if(l >= r) return 0;
    int mid = (l + r) / 2;
    ll ans = merge_count(l, mid) + merge_count(mid + 1, r);
    int i = l, j = mid + 1, k = l;
    while(i <= mid && j <= r) {
        if(a[i] <= a[j]) b[k++] = a[i++];
        else { ans += mid - i + 1; b[k++] = a[j++]; }
    }
    while(i <= mid) b[k++] = a[i++];
    while(j <= r) b[k++] = a[j++];
    for(int p = l; p <= r; p++) a[p] = b[p];
    return ans;
}
// 折半搜索：下标1..n，n<=40；所有子集和以及target-s必须在ll范围。
// 统计按位置区分的子集个数，包含空集，不把相同和值去重。
ll subset_sum_count(const vector<ll>& v, ll target) {
    int n = (int)v.size() - 1;
    assert(0 <= n && n <= 40);
    auto sums = [&](int l, int r) {
        vector<ll> ans{0};
        for(int i = l; i <= r; i++) {
            int old = (int)ans.size();
            for(int j = 0; j < old; j++) ans.push_back(ans[j] + v[i]);
        }
        return ans;
    };
    int mid = n / 2;
    vector<ll> left = sums(1, mid), right = sums(mid + 1, n);
    sort(right.begin(), right.end());
    ll ans = 0;
    for(ll x : left) {
        auto range = equal_range(right.begin(), right.end(), target - x);
        ans += range.second - range.first;
    }
    return ans;
}
