#include <bits/stdc++.h>
#define ll long long 
using namespace std;


ll gcd(ll a, ll b) {
	return b ? gcd(b, a % b) : a;
}

ll a, b;

int main() {
	scanf("%lld%lld", &a, &b);
	printf("%lld\n", gcd(a, b));
	return 0;
}
