// gcd 统一非负；禁止 LLONG_MIN，因为其绝对值不能用 ll 表示。
struct Node { ll g; };
Node make_node(ll val) {
    assert(val != LLONG_MIN);
    return {val < 0 ? -val : val};
}
Node merge(const Node& x, const Node& y) { return {gcd(x.g, y.g)}; }
