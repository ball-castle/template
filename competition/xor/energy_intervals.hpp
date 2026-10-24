#pragma once
#include <bits/stdc++.h>
using namespace std;

// 2268C / 2269E。延续原前缀p思路，补全原本为空的solve。
// a[1..n]，n>=2，0<=a[i]<2^width，1<=width<=18。
vector<int> a, p, s, L, R, cnt;
vector<array<int, 2>> ch;
int root = 0;
bool energy_check(int mask) {
    int n = int(a.size()) - 1;
    for(int i = 0; i <= n; i++) s[i] = p[i] & mask;
    fill(cnt.begin(), cnt.end(), 0);
    // {结点,阶段,是否保留频次}；显式栈避免单调数组递归n层。
    vector<array<int, 3>> st{{root, 0, 1}};
    while(!st.empty()) {
        auto [v, stage, keep] = st.back(); st.pop_back();
        int left = ch[v][0], right = ch[v][1];
        bool heavy_right = R[v] - v > v - L[v];
        int heavy = heavy_right ? right : left;
        int light = heavy_right ? left : right;
        if(stage == 0) {
            st.push_back({v, 1, keep});
            if(heavy) st.push_back({heavy, 0, 1});
            if(light) st.push_back({light, 0, 0});
            continue;
        }
        bool allowed = (a[v] & mask) == mask;
        if(heavy_right) {
            cnt[s[v]]++; // 对侧端点为[v..R]
            if(allowed) for(int x = L[v] - 1; x < v; x++) {
                int need = s[x] ^ mask;
                int forbidden = (x == v - 1 && need == s[v]);
                if(cnt[need] > forbidden) return true;
            }
            for(int x = L[v]; x < v; x++) cnt[s[x]]++;
        } else {
            cnt[s[L[v] - 1]]++; // 对侧端点为[L-1..v-1]
            if(allowed) for(int y = v; y <= R[v]; y++) {
                int need = s[y] ^ mask;
                int forbidden = (y == v && need == s[v - 1]);
                if(cnt[need] > forbidden) return true;
            }
            cnt[s[L[v] - 1]]--;
            for(int y = v; y <= R[v]; y++) cnt[s[y]]++;
        }
        // 此时恰好保存前缀下标[L[v]..R[v]]。
        if(!keep) for(int i = L[v]; i <= R[v]; i++) cnt[s[i]]--;
    }
    return false;
}
int energy_max(const vector<int>& input, int width = 18) {
    a = input;
    int n = int(a.size()) - 1;
    if(n < 2) return 0;
    p.assign(n + 1, 0); s.resize(n + 1);
    L.resize(n + 1); R.resize(n + 1); ch.assign(n + 1, {0, 0});
    cnt.assign(1 << width, 0);
    vector<int> st;
    for(int i = 1; i <= n; i++) {
        p[i] = p[i - 1] ^ a[i];
        int last = 0;
        while(!st.empty() && a[st.back()] < a[i]) {
            last = st.back(); st.pop_back();
        }
        if(!st.empty()) ch[st.back()][1] = i;
        ch[i][0] = last; st.push_back(i);
    }
    root = st.front();
    vector<int> order{root};
    for(size_t i = 0; i < order.size(); i++)
        for(int v : ch[order[i]]) if(v) order.push_back(v);
    reverse(order.begin(), order.end());
    for(int v : order) {
        L[v] = ch[v][0] ? L[ch[v][0]] : v;
        R[v] = ch[v][1] ? R[ch[v][1]] : v;
    }
    int ans = 0;
    for(int k = width - 1; k >= 0; k--)
        if(energy_check(ans | (1 << k))) ans |= 1 << k;
    return ans;
}
