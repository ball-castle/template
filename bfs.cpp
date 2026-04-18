#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 10;
vector<int> g[N];
bool visited[N];

void bfs(int start) {
    queue<int> q;
    visited[start] = true;
    q.push(start); // 起点入队
    
    while (!q.empty()) {
        int u = q.front(); // 取出队首
        q.pop();
        
        // the operation for every point
        
        for (int v : g[u]) {
            if (!visited[v]) {
                visited[v] = true; // 标记访问并入队
                q.push(v);
            }
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
        g[b].push_back(a); // 无向图
    }
    bfs(1); // 从点1开始搜索
    return 0;
}