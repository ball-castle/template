#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
// 输入 a[1..n]；{}、{0} 均为空数组；返回值也保留 0 号占位。
// 每个位置左侧最近的严格更小值的下标；不存在为 0。
inline vector<int> previous_less(const vector<ll>& a) {
    int n = max(0, int(a.size()) - 1);
    vector<int> ans(n + 1), st;
    for (int i = 1; i <= n; ++i) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        ans[i] = st.empty() ? 0 : st.back();
        st.push_back(i);
    }
    return ans;
}
// 固定长度 k 的窗口最小值；ans[l]=min(a[l..l+k-1])。
// k<=0 或 k>n 返回 {0}。相等时保留较新的下标。
inline vector<ll> window_min(const vector<ll>& a, int k) {
    int n = max(0, int(a.size()) - 1);
    vector<ll> ans(1);
    if (k <= 0 || k > n) return ans;
    deque<int> q;
    for (int i = 1; i <= n; ++i) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] >= a[i]) q.pop_back();
        q.push_back(i);
        if (i >= k) ans.push_back(a[q.front()]);
    }
    return ans;
}
