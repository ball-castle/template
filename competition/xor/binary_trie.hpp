#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// 数组节点：0为空指针，1为根。容量包含根，不含0号哨兵。
vector<array<int, 2>> tree;
vector<ll> cnt;
int bits = 64;
size_t capacity = 1;
bool trie_init(int width, size_t max_nodes) {
    if(width < 1 || width > 64 || max_nodes < 1 ||
       max_nodes > size_t(INT_MAX)) return false;
    bits = width; capacity = max_nodes;
    tree.assign(2, {0, 0}); cnt.assign(2, 0);
    return true;
}
bool trie_fits(ull x) { return bits == 64 || (x >> bits) == 0; }
bool trie_insert(ull x) {
    if(!trie_fits(x) || tree.empty()) return false;
    int p = 1;
    // 先检查新增节点数，容量不足时不留下半次插入。
    for(int k = bits - 1; k >= 0; k--) {
        int c = int((x >> k) & 1);
        if(!tree[p][c]) {
            if(size_t(k + 1) > capacity - (tree.size() - 1)) return false;
            break;
        }
        p = tree[p][c];
    }
    p = 1; cnt[p]++;
    for(int k = bits - 1; k >= 0; k--) {
        int c = int((x >> k) & 1);
        if(!tree[p][c]) {
            tree[p][c] = int(tree.size());
            tree.push_back({0, 0}); cnt.push_back(0);
        }
        p = tree[p][c]; cnt[p]++;
    }
    return true;
}
bool trie_erase(ull x) {
    if(!trie_fits(x) || tree.empty() || !cnt[1]) return false;
    int p = 1, path[65], len = 0;
    path[len++] = p;
    for(int k = bits - 1; k >= 0; k--) {
        p = tree[p][(x >> k) & 1];
        if(!p || !cnt[p]) return false;
        path[len++] = p;
    }
    for(int i = 0; i < len; i++) cnt[path[i]]--;
    return true; // 不回收节点；相同路径可复用，换批次 init()
}
// 返回 XOR 值而非被选元素。空树/超位宽返回false，不修改ans。
bool trie_query(ull x, ull& ans, bool maximum = true) {
    if(!trie_fits(x) || tree.empty() || !cnt[1]) return false;
    int p = 1;
    ans = 0;
    for(int k = bits - 1; k >= 0; k--) {
        int c = int((x >> k) & 1), want = c ^ int(maximum);
        int q = tree[p][want];
        if(!q || !cnt[q]) want ^= 1;
        if(c != want) ans |= 1ULL << k;
        p = tree[p][want];
    }
    return true;
}
// a[1..n]，n<=1e6；无数对/空数组按0返回；会重置全局Trie。
ull max_xor_pair(const vector<ull>& a) {
    trie_init(64, 1 + 64 * (a.size() - 1));
    ull ans = 0, value = 0;
    for(size_t i = 1; i < a.size(); i++) {
        if(trie_query(a[i], value)) ans = max(ans, value);
        trie_insert(a[i]); // 先查后插，保证不同下标
    }
    return ans;
}
ull max_xor_subarray(const vector<ull>& a) {
    trie_init(64, 1 + 64 * a.size());
    trie_insert(0); // 空前缀，允许子数组从1开始
    ull p = 0, ans = 0, value = 0;
    for(size_t i = 1; i < a.size(); i++) {
        p ^= a[i];
        trie_query(p, value); ans = max(ans, value);
        trie_insert(p);
    }
    return ans;
}
