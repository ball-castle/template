#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 字符串统一0-indexed。pi[i]是s[0..i]最长相等真前后缀的长度。
inline vector<int> prefix_function(const string& s) {
    vector<int> pi(s.size());
    for (int i = 1; i < int(s.size()); ++i) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}
// 返回p在s中所有起点，允许重叠；空p匹配0..s.size()共n+1处。
inline vector<int> kmp_find_all(const string& s, const string& p) {
    vector<int> ans;
    if (p.empty()) {
        ans.resize(s.size() + 1); iota(ans.begin(), ans.end(), 0);
        return ans;
    }
    auto pi = prefix_function(p);
    for (int i = 0, j = 0; i < int(s.size()); ++i) {
        while (j && s[i] != p[j]) j = pi[j - 1];
        if (s[i] == p[j]) ++j;
        if (j == int(p.size())) {
            ans.push_back(i - j + 1);
            j = pi[j - 1];
        }
    }
    return ans;
}
// z[i]是s与s[i..]的最长公共前缀长度；本实现约定z[0]=0。
inline vector<int> z_function(const string& s) {
    int n = int(s.size());
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        // [l,r) 是已知最靠右的匹配区间。
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
// kmp_find_all(text,pattern)无需选分隔符，适合直接查找。
