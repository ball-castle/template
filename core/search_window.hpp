#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 整数闭区间 [lo,hi]；空区间或无解返回 nullopt。
// ok 确定且单调；check 内部的乘加仍需自行防溢出。
template<class F>
optional<ll> first_true(ll lo, ll hi, F ok) {
    if (lo > hi || !ok(hi)) return nullopt; // false...false,true...true
    while (lo < hi) {
        ll mid = midpoint(lo, hi); // 向 lo 取整，全 ll 范围不溢出。
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
template<class F>
optional<ll> last_true(ll lo, ll hi, F ok) {
    if (lo > hi || !ok(lo)) return nullopt; // true...true,false...false
    while (lo < hi) {
        ll mid = midpoint(hi, lo); // 向 hi 取整，避免死循环。
        if (ok(mid)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}
// a[1..n] 非降序，找不同位置 i<j 且 a[i]+a[j]=target；无解 {0,0}。
inline pair<int, int> two_sum_sorted(const vector<ll>& a, ll target) {
    int l = 1, r = max(0, int(a.size()) - 1);
    while (l < r) {
        __int128 s = (__int128)a[l] + a[r];
        if (s == target) return {l, r};
        if (s < target) ++l;
        else --r;
    }
    return {0, 0};
}
// a[1..n] 必须非负；返回和<=limit 的最长连续段长度，可为 0。
// 负数会破坏窗口单调性；{}、{0} 均返回 0。和用 i128 防溢出。
inline int longest_sum_at_most(const vector<ll>& a, ll limit) {
    if (limit < 0) return 0;
    int n = max(0, int(a.size()) - 1), l = 1, best = 0;
    __int128 sum = 0;
    for (int r = 1; r <= n; ++r) {
        assert(a[r] >= 0);
        sum += a[r];
        while (sum > limit) sum -= a[l++];
        best = max(best, r - l + 1);
    }
    return best;
}
