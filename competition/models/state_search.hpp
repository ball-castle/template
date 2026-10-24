#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;

struct IntegerPath {
    vector<int> states;
    vector<char> operations; // '+'为+1，'-'为-1，'*'为乘2。
};

// 整数局面x直接编码为数组下标x；0<=start,target<=limit<=1e6。
// 每次+1/-1/乘2，结果必须仍在[0,limit]；每步费用1。
IntegerPath integer_state_bfs(int start, int target, int limit) {
    assert(0 <= limit && limit <= 1000000);
    assert(0 <= start && start <= limit && 0 <= target && target <= limit);
    vector<int> dist(limit + 1, -1), fa(limit + 1, -1);
    vector<char> from(limit + 1);
    queue<int> q;
    dist[start] = 0; q.push(start);
    while(!q.empty()) {
        int x = q.front(); q.pop();
        if(x == target) break;
        int next[3] = {x + 1, x - 1, x * 2};
        const char op[3] = {'+', '-', '*'};
        for(int k = 0; k < 3; k++) {
            int y = next[k];
            if(y < 0 || y > limit || dist[y] != -1) continue;
            dist[y] = dist[x] + 1; fa[y] = x; from[y] = op[k];
            q.push(y);
        }
    }
    IntegerPath ans;
    for(int x = target; x != start; x = fa[x]) {
        ans.states.push_back(x); ans.operations.push_back(from[x]);
    }
    ans.states.push_back(start);
    reverse(ans.states.begin(), ans.states.end());
    reverse(ans.operations.begin(), ans.operations.end());
    return ans; // +/-1保证边界内任意两状态可达。
}

// a[1..n]非负、n<=40、target>=0；返回一个子集的原下标或nullopt。
// 重复值按不同位置保留；target=0允许空集。最坏仍O(2^n)。
optional<vector<int>> subset_target_nonnegative(const vector<ll>& a, ll target) {
    assert(!a.empty() && a.size() <= 41 && target >= 0);
    int n = (int)a.size() - 1;
    vector<i128> remain(n + 2);
    for(int i = n; i >= 1; i--) {
        assert(a[i] >= 0);
        remain[i] = remain[i + 1] + a[i];
    }
    vector<int> chosen;
    auto dfs = [&](auto&& self, int i, i128 sum) -> bool {
        if(sum > target || sum + remain[i] < target) return false;
        if(sum == target) return true;
        if(i > n) return false;
        chosen.push_back(i);
        if(self(self, i + 1, sum + a[i])) return true;
        chosen.pop_back();
        return self(self, i + 1, sum);
    };
    if(dfs(dfs, 1, 0)) return chosen;
    return nullopt;
}
