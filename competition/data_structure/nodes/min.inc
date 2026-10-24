struct Node { ll mn; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {min(x.mn, y.mn)}; }
