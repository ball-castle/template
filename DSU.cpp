#include <bits/stdc++.h>
#define ll long long 
using namespace std;
/*
attach to the parent node
*/
const ll N = 1e5 + 5;
ll fa[N], rk[N];


void init(ll n) {
	for(ll i = 1; i <= n; i++) {
		fa[i] = i;
		rk[i] = 1;
	}
}

int find(ll x) {
	return fa[x] == x ? x : fa[x] = find(fa[x]);
}

void unite(ll x, ll y) {
	ll fx = find(x), fy = find(y);
	if(fx == fy) return;
	if(rk[fx] < rk[fy]) fa[fx] = fy;
	else {
		fa[fy] = fx;
		if(rk[fx] == rk[fy]) rk[fx]++;
	}
}

bool same(ll x, ll y) {
	return find(x) == find(y);
}


