#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 0-indexed字符串；get(l,r)取半开区间[l,r)，要求0<=l<=r<=n。
// 双模哈希仍可能碰撞，不能作为无碰撞证明；重要比较须直接核验。
// 比较前同时比较长度；不同对象使用相同base和模数才可比较。
struct DoubleHash {
    static constexpr ll mod1 = 1000000007, mod2 = 1000000009;
    static constexpr ll base = 911382323;
    vector<ll> h1, h2, p1, p2;
    explicit DoubleHash(const string& s) {
        int n = int(s.size());
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            // unsigned char避免高位字节被解释成负数；+1不把零字节当0。
            ll x = (unsigned char)s[i] + 1;
            h1[i + 1] = (h1[i] * base + x) % mod1;
            h2[i + 1] = (h2[i] * base + x) % mod2;
            p1[i + 1] = p1[i] * base % mod1;
            p2[i + 1] = p2[i] * base % mod2;
        }
    }
    pair<ll, ll> get(int l, int r) const {
        assert(0 <= l && l <= r && r < int(h1.size()));
        ll a = (h1[r] - h1[l] * p1[r - l] % mod1 + mod1) % mod1;
        ll b = (h2[r] - h2[l] * p2[r - l] % mod2 + mod2) % mod2;
        return {a, b};
    }
};
// DoubleHash h(s); auto key=h.get(l,r); 空区间返回(0,0)。
// 固定base方便复现，但可被针对构造；需要确定性匹配优先用KMP。
