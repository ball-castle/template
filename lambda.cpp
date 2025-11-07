#include <bits/stdc++.h>
#define ll long long 
using namespace std;

vector<ll> a;
int main() {
	auto sum = [&](vector<ll> &a) -> ll {
		ll ans = 0;
		for(auto &x : a) ans += x;
		return ans;
	};
	sort(a.begin(), a.end(), [](ll a, ll b) {return a > b;});
	auto add = [&](ll x, ll y) -> ll {return x + y;};
	auto print = [&](auto &self, ll n) -> void {
		if(n == 0) return;
		self(self, n - 1);
		cout << n << " ";
	};
	return 0;
}
