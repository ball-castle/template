// 公共单点树骨架：先放一个 Node / make_node / merge，再接本文件。
// 每个 segment_point_*.hpp 均已准确包含所需节点与本骨架，可直接 include。
// 保留旧代码 a/tree/node/l/r/mid 和递归结构；update 改名 modify。
// 非空树，1<=n<N；1<=pos<=n；查询必须 1<=L<=R<=n。
// a 仅为建树输入缓冲；modify 后当前值以 tree 为准。
const int N = 200000 + 5;
ll a[N];
Node tree[N * 4];

void pushup(int node) {
    tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
}
void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = make_node(a[l]);
        return;
    }
    int mid = (l + r) >> 1;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node);
}
void apply(int node, ll val) {
    tree[node] = make_node(val);
}
void modify(int node, int l, int r, int pos, ll val) {
    if (l == r) {
        apply(node, val);
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) modify(node * 2, l, mid, pos, val);
    else modify(node * 2 + 1, mid + 1, r, pos, val);
    pushup(node);
}
Node query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    int mid = (l + r) >> 1;
    if (R <= mid) return query(node * 2, l, mid, L, R);
    if (L > mid) return query(node * 2 + 1, mid + 1, r, L, R);
    return merge(query(node * 2, l, mid, L, R),
                 query(node * 2 + 1, mid + 1, r, L, R));
}
