#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// a[1..n]；a[0] 不使用。来源：cf/1/1124/C.cpp 的前缀抵消观察。
vector<ull> xor_prefix(const vector<ull>& a) {
    vector<ull> p(a.size());
    for(size_t i = 1; i < a.size(); i++) p[i] = p[i - 1] ^ a[i];
    return p;
}
ull range_xor(const vector<ull>& p, int l, int r) {
    return p[r] ^ p[l - 1]; // 1 <= l <= r <= n
}
ll count_subarray_xor(const vector<ull>& a, ull k) {
    unordered_map<ull, ll> cnt;
    cnt[0] = 1;
    ull p = 0;
    ll ans = 0;
    for(size_t i = 1; i < a.size(); i++) {
        p ^= a[i];
        auto it = cnt.find(p ^ k);
        if(it != cnt.end()) ans += it->second;
        cnt[p]++; // 先查询，避免 k=0 时计入空区间
    }
    return ans;
}
// 操作 {l,r,x}：将 a[l..r] 都 ^= x。仅需最终数组时使用。
vector<ull> xor_offline_update(vector<ull> a,
                              const vector<array<ull, 3>>& q) {
    vector<ull> d(a.size() + 1);
    for(auto [l, r, x] : q) {
        d[l] ^= x;
        d[r + 1] ^= x;
    }
    ull tag = 0;
    for(size_t i = 1; i < a.size(); i++) {
        tag ^= d[i];
        a[i] ^= tag;
    }
    return a;
}
