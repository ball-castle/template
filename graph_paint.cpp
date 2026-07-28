#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll col[200010];
queue<ll> q;

int main() {
	ll start = 1;
	col[start] = 1;
	q.push(start);
	vector<ll> adj[200010];
	while(!q.empty()) {
		ll u = q.front();
		q.pop();

		for(ll v : adj[u]) {
			if(!col[v]) {
				col[v] = 3 - col[u];
				q.push(v);
			}
		}
	}
}


