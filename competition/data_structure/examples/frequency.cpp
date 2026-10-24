#include "../segment_frequency.hpp"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    while (q--) {
        int op; cin >> op;
        if (op == 1) {
            int pos; ll val;
            cin >> pos >> val;
            modify(1, 1, n, pos, val);
        } else {
            ll k; cin >> k;
            cout << kth(1, 1, n, k) << '\n';
        }
    }
}
