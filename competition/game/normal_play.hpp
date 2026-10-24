#pragma once
#include <bits/stdc++.h>
using namespace std;

// 正常 Nim：每步选择一堆，取走任意正数；无棋可走者输。
inline unsigned long long nim_xor(const vector<unsigned long long>& a) {
    unsigned long long sum = 0;
    for (auto x : a) sum ^= x;
    return sum;
}
// 返回 {0-based 堆下标, 操作后堆大小}；必败态返回 {-1,0}。
inline pair<int, unsigned long long>
nim_winning_move(const vector<unsigned long long>& a) {
    auto sum = nim_xor(a);
    if (!sum) return {-1, 0};
    for (int i = 0; i < int(a.size()); ++i)
        if ((a[i] ^ sum) < a[i]) return {i, a[i] ^ sum};
    return {-1, 0};
}

// 有限、无环、公平、无随机、完全信息、正常规则的有向状态图。
// 状态已按依赖编号：moves[v] 内每个 u 必须满足 0<=u<v。
// sg[v]=mex({sg[u]})；独立子游戏之和的 SG 才可异或。
inline vector<int> sg_ordered_dag(const vector<vector<int>>& moves) {
    int n = int(moves.size());
    vector<int> sg(n);
    for (int v = 0; v < n; ++v) {
        vector<bool> vis(moves[v].size() + 1);
        for (int u : moves[v]) {
            assert(0 <= u && u < v);
            if (sg[u] < int(vis.size())) vis[sg[u]] = true;
        }
        while (sg[v] < int(vis.size()) && vis[sg[v]]) ++sg[v];
    }
    return sg;
}

// 反常 Nim：拿最后一个的人输，仅限 Nim 取石规则。
// 若所有非空堆都是 1，偶数个 1 必胜；否则仍看 xor。
// 全空时返回 true：按反常约定，无棋可走者赢。
inline bool misere_nim_win(const vector<unsigned long long>& a) {
    int ones = 0;
    bool large = false;
    for (auto x : a) {
        ones += (x == 1);
        large |= (x > 1);
    }
    return large ? nim_xor(a) != 0 : ones % 2 == 0;
}
