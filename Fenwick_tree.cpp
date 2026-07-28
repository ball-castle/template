#include <bits/stdc++.h>
using namespace std;
using ll = long long;




struct Fenwick { 
    int n;
    vector<long long> tr;

    Fenwick(int n) : n(n), tr(n + 1) {}

    void add(int x, long long v) {
        for(; x <= n; x += x & -x) {
            tr[x] += v;
        }
    }

    long long sum(int x) {
        long long res = 0;
        for(; x; x -= x & -x) {
            res += tr[x];
        }
        return res;
    }

    long long query(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Fenwick fen(n);

    for (int i = 1; i <= n; i++) {
        long long x;
        cin >> x;
        fen.add(i, x);
    }

    while(q--) {
        int op;
        cin >> op;

        if(op == 1) {
            int x;
            long long v;
            cin >> x >> v;
            fen.add(x, v);
        } 
        else {
            int l, r;
            cin >> l >> r;
            cout << fen.rangeSum(l, r) << '\n';
        }
    }

    return 0;
}