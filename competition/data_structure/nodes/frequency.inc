// 每个位置存非负频率；修改是把该位置频率改成 val。
struct Node { ll sum; };
Node make_node(ll val) { assert(val >= 0); return {val}; }
Node merge(const Node& x, const Node& y) { return {x.sum + y.sum}; }
