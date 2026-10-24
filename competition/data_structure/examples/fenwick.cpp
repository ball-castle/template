#include "../fenwick.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    Fenwick fen(n);
    for (int i = 1; i <= n; ++i) { ll x; cin >> x; fen.add(i, x); }
    while (q--) {
        int op, l, r; ll val;
        cin >> op;
        if (op == 1) { cin >> l >> val; fen.add(l, val); }
        else if (op == 2) { cin >> l >> r; cout << fen.rangeSum(l, r) << '\n'; }
        else { cin >> r; cout << fen.query(r) << '\n'; }
    }
}
