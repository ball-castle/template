#include <iostream>
#include <vector>
using namespace std;

const int N = 1e5 + 10;
vector<int> g[N];
bool visited[N];

void dfs(int u) {
    visited[u] = true;
    // the operation for every point
    for(int v : g[u]) {
        if (!visited[v]) {
            dfs(v);
        }
    }
}

int main() {
    int n, m; // n个点，m条边
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a); // 如果是无向图
    }
    dfs(1); // 从点1开始搜索
    return 0;
}
