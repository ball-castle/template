struct Node { ll mx; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {max(x.mx, y.mx)}; }
