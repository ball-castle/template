#include <bits/stdc++.h>
#define ll long long
using namespace std;

/*
线段树：单点修改 + 区间查询

适用：
1. 单点改值
2. 单点加值
3. 区间求和

不需要 lazy
*/

const int N = 2e5 + 5;

ll a[N];          // 原数组
ll tree[N * 4];   // 线段树数组

// 把左右儿子的信息往上传
void pushup(int node) {
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

// 建树
void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = a[l];
        return;
    }

    int mid = (l + r) >> 1;

    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);

    pushup(node);
}

// 单点修改：把 a[pos] 改成 val
void update(int node, int l, int r, int pos, ll val) {
    if (l == r) {
        tree[node] = val;
        return;
    }

    int mid = (l + r) >> 1;

    if (pos <= mid) {
        update(node * 2, l, mid, pos, val);
    } else {
        update(node * 2 + 1, mid + 1, r, pos, val);
    }

    pushup(node);
}

// 单点加：把 a[pos] 加上 val
void add(int node, int l, int r, int pos, ll val) {
    if (l == r) {
        tree[node] += val;
        return;
    }

    int mid = (l + r) >> 1;

    if (pos <= mid) {
        add(node * 2, l, mid, pos, val);
    } else {
        add(node * 2 + 1, mid + 1, r, pos, val);
    }

    pushup(node);
}

// 区间查询：查询 [L, R] 的和
ll query(int node, int l, int r, int L, int R) {
    if (R < l || r < L) return 0;

    if (L <= l && r <= R) {
        return tree[node];
    }

    int mid = (l + r) >> 1;

    return query(node * 2, l, mid, L, R)
         + query(node * 2 + 1, mid + 1, r, L, R);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    build(1, 1, n);

    while (q--) {
        int op;
        cin >> op;

        if (op == 1) {
            // 单点修改：a[pos] = val
            int pos;
            ll val;
            cin >> pos >> val;

            update(1, 1, n, pos, val);
        } 
        else if (op == 2) {
            // 单点加：a[pos] += val
            int pos;
            ll val;
            cin >> pos >> val;

            add(1, 1, n, pos, val);
        } 
        else if (op == 3) {
            // 区间查询：[L, R]
            int L, R;
            cin >> L >> R;

            cout << query(1, 1, n, L, R) << '\n';
        }
    }

    return 0;
}