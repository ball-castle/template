#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// 先递归处理儿子，再用儿子状态更新父亲

vector<int> g[200010];
ll dp[200010];



void dfs(int u, int fa) {
    for(int v : g[u]) {
        if(v == fa) continue;
        dfs(v, u);
        dp[u] += dp[v];
    }
}



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    dfs(1, 0);
    return 0;
}