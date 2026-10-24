#include "../segment_max_count.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            int l, r;
            cin >> l >> r;
            auto ans = query(1, 1, n, l, r);
            cout << ans.mx << ' ' << ans.cnt << '\n';
        }
    }
}
