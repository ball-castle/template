// 非空连续子段：全负时返回最大的那个负数。
struct Node { ll sum, pref, suff, best; };
Node make_node(ll val) { return {val, val, val, val}; }
Node merge(const Node& x, const Node& y) {
    return {x.sum + y.sum,
            max(x.pref, x.sum + y.pref),
            max(y.suff, y.sum + x.suff),
            max({x.best, y.best, x.suff + y.pref})};
}
