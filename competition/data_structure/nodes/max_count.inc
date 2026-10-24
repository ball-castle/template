struct Node { ll mx; int cnt; };
Node make_node(ll val) { return {val, 1}; }
Node merge(const Node& x, const Node& y) {
    if (x.mx > y.mx) return x;
    if (x.mx < y.mx) return y;
    return {x.mx, x.cnt + y.cnt};
}
