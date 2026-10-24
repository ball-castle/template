#pragma once
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

vector<vector<pair<int, ull>>> g;
vector<ull> value;
vector<int> comp;
int components = 0;
void xor_graph_init(int n) {
    g.assign(n + 1, {}); value.assign(n + 1, 0);
    comp.assign(n + 1, 0); components = 0;
}
void xor_add_edge(int u, int v, ull w) {
    g[u].push_back({v, w}); g[v].push_back({u, w});
}
bool xor_assign() {
    fill(comp.begin(), comp.end(), 0);
    components = 0;
    for(int s = 1; s < int(g.size()); s++) {
        if(comp[s]) continue;
        queue<int> q;
        comp[s] = ++components; value[s] = 0; q.push(s);
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for(auto [v, w] : g[u]) {
                if(!comp[v]) {
                    comp[v] = components; value[v] = value[u] ^ w;
                    q.push(v);
                } else if((value[u] ^ value[v]) != w) return false;
            }
        }
    }
    return true; // 仅true时value为完整解；每分量可整体再异或任意t
}
