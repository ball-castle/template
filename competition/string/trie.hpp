#pragma once
#include <bits/stdc++.h>
using namespace std;

// 源自 cf/ICPC/2026ICPCWeb1/L.cpp 的 tr/cnt/dep/tot 写法。
// 根为0，0号儿子表示不存在；最多TRIE_N-1个非根结点。
// 每次trie_init后，所有新插入串总长<TRIE_N是充分容量条件。
const int TRIE_N = 500005;
inline int tr[TRIE_N][26], cnt[TRIE_N], dep[TRIE_N];
inline int word_end[TRIE_N], tot;

inline void trie_init() {
    for (int u = 0; u <= tot; ++u) {
        fill(tr[u], tr[u] + 26, 0);
        cnt[u] = dep[u] = word_end[u] = 0;
    }
    tot = 0;
}
// 只接收'a'..'z'；重复串重复计数；空串在根处结束。
inline void trie_insert(const string& s) {
    int u = 0;
    ++cnt[0];
    for (char ch : s) {
        int c = ch - 'a';
        assert(0 <= c && c < 26);
        if (!tr[u][c]) {
            assert(tot + 1 < TRIE_N);
            tr[u][c] = ++tot;
            dep[tot] = dep[u] + 1;
        }
        u = tr[u][c];
        ++cnt[u];
    }
    ++word_end[u];
}
inline int trie_node(const string& s) {
    int u = 0;
    for (char ch : s) {
        int c = ch - 'a';
        assert(0 <= c && c < 26);
        if (!tr[u][c]) return -1;
        u = tr[u][c];
    }
    return u;
}
inline int trie_prefix(const string& s) {
    int u = trie_node(s);
    return u == -1 ? 0 : cnt[u];
}
inline int trie_count(const string& s) {
    int u = trie_node(s);
    return u == -1 ? 0 : word_end[u];
}
