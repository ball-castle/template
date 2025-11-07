#include <bits/stdc++.h>
#define ll long long 
using namespace std;
/*
not form a circle
find min edge 
*/

int fa[1010];  
int n, m, k;

struct edge {
  int u, v, w;
};

int l;
edge g[10010];

void add(int u, int v, int w) {
  l++;
  g[l].u = u;
  g[l].v = v;
  g[l].w = w;
}

int findroot(int x) { return fa[x] == x ? x : fa[x] = findroot(fa[x]); }

void Merge(int x, int y) {
  x = findroot(x);
  y = findroot(y);
  fa[x] = y;
}

bool cmp(edge A, edge B) {return A.w < B.w;}


void kruskal() {
  int tot = 0; 
  int ans = 0; 
  for(int i = 1; i <= m; i++) {
    int xr = findroot(g[i].u), yr = findroot(g[i].v);
    if (xr != yr) {       
      Merge(xr, yr);       
      tot++;               
      ans += g[i].w;     
      if(tot == n - k) { // init connected component is n, link once minus 1;
        cout << ans << '\n';
        return;
      }
    }
  }
  cout << "No Answer\n";  
}
 
int main() {
	cin >> n >> m >> k;
	if (n == k) {  
		cout << "0\n";
    	return 0;
  	}
	for(int i = 1; i <= n; i++) {  
		fa[i] = i;
	}
	for(int i = 1; i <= m; i++) {
		int u, v, w;
    	cin >> u >> v >> w;
    	add(u, v, w);  
    }
	sort(g + 1, g + m + 1, cmp);  
	kruskal();
	return 0;
}
