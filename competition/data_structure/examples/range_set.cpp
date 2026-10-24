#include "../segment_set_add.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            ll val; cin >> val;
            modify(1, 1, n, l, r, val, true);
        } else cout << query(1, 1, n, l, r) << '\n';
    }
}
