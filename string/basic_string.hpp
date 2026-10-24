#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 基础字符串均按字节处理；0-indexed，区间[l,r)，不是Unicode码点。
inline bool is_palindrome(const string& s, int l, int r) {
    assert(0 <= l && l <= r && r <= int(s.size()));
    for (--r; l < r; ++l, --r) if (s[l] != s[r]) return false;
    return true;
}
// 按单个字符分割，保留首尾及连续分隔符产生的空串。
inline vector<string> split_char(const string& s, char sep) {
    vector<string> parts;
    size_t start = 0;
    for (size_t i = 0; i <= s.size(); ++i)
        if (i == s.size() || s[i] == sep) {
            parts.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    return parts;
}
inline string ascii_lower(string s) {
    for (char& c : s) if ('A' <= c && c <= 'Z') c = char(c - 'A' + 'a');
    return s;
}
inline vector<pair<char, int>> run_length(const string& s) {
    vector<pair<char, int>> groups;
    for (char c : s) {
        if (groups.empty() || groups.back().first != c)
            groups.push_back({c, 1});
        else ++groups.back().second;
    }
    return groups;
}
// 读含空格整行：getline(cin,s)；刚用cin>>后先丢弃残留换行。
// cin.ignore(numeric_limits<streamsize>::max(),'\n');
// cin>>ws会跳过全部前导空白；若空白有意义，不要使用它。
