struct Node { ll sum; };
Node make_node(ll val) { return {val}; }
Node merge(const Node& x, const Node& y) { return {x.sum + y.sum}; }
