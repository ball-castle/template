#pragma once
#include <bits/stdc++.h>
#include "../../string/basic_string.hpp" // 复用S03的双指针判定。
using namespace std;

struct PalindromeInfo {
    long long count; // 非空回文子串个数：按位置计数，不按内容去重。
    int l, r;       // 最长回文子串的0-based半开区间[l,r)。
};
// 奇中心(i,i)与偶中心(i,i+1)分别扩展；空串返回{0,0,0}。
inline PalindromeInfo palindrome_centers(const string& s) {
    PalindromeInfo ans{0, 0, 0};
    int n = int(s.size());
    auto extend = [&](int l, int r) {
        while (l >= 0 && r < n && s[l] == s[r]) {
            ++ans.count;
            if (r - l + 1 > ans.r - ans.l) { ans.l = l; ans.r = r + 1; }
            --l; ++r;
        }
    };
    for (int i = 0; i < n; ++i) { extend(i, i); extend(i, i + 1); }
    return ans;
}
// 重排成回文：最多一种字符出现奇数次；按字节，不按Unicode字符。
inline optional<string> palindrome_permutation(const string& s) {
    array<int, 256> freq{};
    for (unsigned char c : s) ++freq[c];
    string left, middle;
    for (int c = 0; c < 256; ++c) {
        left.append(freq[c] / 2, char(c));
        if (freq[c] % 2) middle += char(c);
    }
    if (middle.size() > 1) return nullopt;
    string right = left; reverse(right.begin(), right.end());
    return left + middle + right;
}
// 非负整数的标准十进制表示：单独0允许，其他前导零不允许。
inline bool is_decimal_palindrome(const string& s) {
    if (s.empty() || (s.size() > 1 && s[0] == '0')) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return is_palindrome(s, 0, int(s.size()));
}

// 直接保留 cf/construction/1.cpp：镜像传播、扣已用计数、填中心/配对。
// s仅含'0'/'1'/'?'，要恰好a个0、b个1；失败返回nullopt。
inline optional<string> binary_palindrome_fill(string s, int a, int b) {
    if (a < 0 || b < 0 || (long long)a + b != (long long)s.size())
        return nullopt;
    for (int times = 0; times < 2; ++times) {
        for (int i = 0; i < int(s.size()); ++i) {
            int j = int(s.size()) - i - 1;
            if (s[i] != '?') {
                if (s[j] == '?') s[j] = s[i];
                else if (s[i] != s[j]) return nullopt;
            }
        }
        reverse(s.begin(), s.end());
    }
    a -= int(count(s.begin(), s.end(), '0'));
    b -= int(count(s.begin(), s.end(), '1'));
    int ques = int(count(s.begin(), s.end(), '?'));
    bool empty_mid = s.size() % 2 == 1 && s[s.size() / 2] == '?';
    if (a < 0 || b < 0 || a + b != ques ||
        (empty_mid && a % 2 == 0 && b % 2 == 0)) return nullopt;
    if (a % 2 == 1 || b % 2 == 1) {
        int i = int(s.size()) / 2;
        if (s[i] != '?') return nullopt;
        s[i] = (a % 2 == 1 ? '0' : '1');
        if (a % 2 == 1) --a;
        else --b;
    }
    if (a % 2 == 1 || b % 2 == 1) return nullopt;
    for (int i = 0; i < int(s.size()); ++i) {
        if (s[i] == '?') {
            int j = int(s.size()) - i - 1;
            if (a > 0) { a -= 2; s[i] = s[j] = '0'; }
            else { b -= 2; s[i] = s[j] = '1'; }
        }
    }
    return s;
}
