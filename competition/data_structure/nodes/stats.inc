// 一起维护的字段须都能从左右子段独立合并；禁止 LLONG_MIN。
struct Node { ll sum, mn, mx, g; };
Node make_node(ll val) {
    assert(val != LLONG_MIN);
    return {val, val, val, val < 0 ? -val : val};
}
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum, min(x.mn, y.mn), max(x.mx, y.mx), gcd(x.g, y.g)};
}
