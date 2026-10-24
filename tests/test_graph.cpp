#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#if __has_include("../graph/dsu.hpp") && \
    __has_include("../graph/bfs_path.hpp") && \
    __has_include("../graph/dfs_forest.hpp") && \
    __has_include("../graph/topological_sort.hpp") && \
    __has_include("../graph/unicyclic_graph.hpp")
#define GRAPH_P0_READY
#include "../graph/dsu.hpp"
#include "../graph/bfs_path.hpp"
#include "../graph/dfs_forest.hpp"
#include "../graph/topological_sort.hpp"
#include "../graph/unicyclic_graph.hpp"
#endif

#if !defined(P0_ONLY) && __has_include("../graph/dijkstra.hpp") && \
    __has_include("../graph/kruskal.hpp") && \
    __has_include("../graph/prim.hpp") && \
    __has_include("../graph/lca.hpp") && \
    __has_include("../graph/euler_subtree.hpp")
#define GRAPH_P1_READY
#include "../graph/dijkstra.hpp"
#include "../graph/kruskal.hpp"
#include "../graph/prim.hpp"
#include "../graph/lca.hpp"
#include "../graph/euler_subtree.hpp"
#endif

#if !defined(P0_ONLY) && __has_include("../advanced/scc.hpp") && \
    __has_include("../advanced/bridges.hpp")
#define GRAPH_P2_READY
#include "../advanced/scc.hpp"
#include "../advanced/bridges.hpp"
#endif

void check(bool ok, const string& why) {
    if (!ok) { cerr << "FAIL: " << why << '\n'; exit(1); }
}
mt19937 rng(20261024);
int rnd(int lo, int hi) { return uniform_int_distribution<>(lo, hi)(rng); }
using Graph = vector<vector<int>>;
void edge(Graph& g, int u, int v) {
    g[u].push_back(v); g[v].push_back(u);
}
bool reachable(const Graph& g, int s, int t) {
    vector<int> seen(g.size());
    queue<int> q; q.push(s); seen[s] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) if (!seen[v]) seen[v] = 1, q.push(v);
    }
    return seen[t];
}

#ifdef GRAPH_P0_READY
void test_p0_boundaries() {
    cp::DSU empty(0), d(4);
    check(empty.components == 0, "DSU empty component count");
    check(d.components == 4, "DSU initial component count");
    d.unite(1, 2); d.unite(2, 1); d.unite(3, 4);
    check(d.components == 2, "DSU redundant unite preserves component count");
    d.unite(2, 3);
    check(d.components == 1, "DSU final component count");
    Graph g(1); auto f = cp::dfs_forest(g);
    check(f.components == 0 && f.order.empty(), "DFS empty graph");
    Graph directed(5); directed[1] = {2}; directed[2] = {3}; directed[4] = {4};
    auto from_one = cp::bfs(directed, 1), from_three = cp::bfs(directed, 3);
    check(from_one.path(3) == vector<int>({1, 2, 3}), "BFS directed path");
    check(from_three.dist[1] == -1 && from_three.path(1).empty(),
          "BFS respects edge direction");
    check(from_one.path(4).empty(), "BFS isolated self-loop unreachable");
}
void test_dsu() {
    cp::DSU one(1);
    check(one.size(1) == 1 && !one.unite(1, 1), "DSU singleton");
    for (int rep = 0; rep < 120; ++rep) {
        int n = rnd(2, 24); Graph g(n + 1); cp::DSU d(n);
        for (int k = 0; k < 100; ++k) {
            int u = rnd(1, n), v = rnd(1, n);
            bool was = reachable(g, u, v);
            check(d.unite(u, v) == !was, "DSU unite return");
            edge(g, u, v);
            for (int x = 1; x <= n; ++x) {
                int count = 0;
                for (int y = 1; y <= n; ++y) {
                    bool r = reachable(g, x, y); count += r;
                    check(d.same(x, y) == r, "DSU vs BFS");
                }
                check(d.size(x) == count, "DSU component size");
            }
        }
    }
}
void test_traversal() {
    for (int rep = 0; rep < 200; ++rep) {
        int n = rnd(1, 25); Graph g(n + 1);
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, 1000));
        for (int u = 1; u <= n; ++u) dist[u][u] = 0;
        for (int k = 0, m = rnd(0, n * 2); k < m; ++k) {
            int u = rnd(1, n), v = rnd(1, n); edge(g, u, v);
            if (u != v) dist[u][v] = dist[v][u] = 1;
        }
        for (int k = 1; k <= n; ++k)
            for (int u = 1; u <= n; ++u)
                for (int v = 1; v <= n; ++v)
                    dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
        int s = rnd(1, n); auto b = cp::bfs(g, s);
        auto f = cp::dfs_forest(g);
        for (int u = 1; u <= n; ++u) {
            int want = dist[s][u] == 1000 ? -1 : dist[s][u];
            check(b.dist[u] == want, "BFS vs Floyd distance");
            auto p = b.path(u);
            if (want == -1) check(p.empty(), "BFS unreachable path");
            else {
                check((int)p.size() == want + 1, "BFS path length");
                check(p.front() == s && p.back() == u, "BFS endpoints");
                for (int i = 1; i < (int)p.size(); ++i)
                    check(dist[p[i - 1]][p[i]] == 1, "BFS path edge");
            }
            for (int v = 1; v <= n; ++v)
                check((f.comp[u] == f.comp[v]) == (dist[u][v] < 1000),
                      "DFS components vs Floyd");
            if (f.parent[u])
                check(dist[u][f.parent[u]] == 1, "DFS parent edge");
        }
        set<int> seen(f.order.begin(), f.order.end());
        check((int)seen.size() == n, "DFS visits each vertex once");
        check(b.path(0).empty() && b.path(n + 1).empty(), "BFS bad target");
    }
}
void check_topo(const Graph& g, const vector<int>& order) {
    int n = (int)g.size() - 1;
    check((int)order.size() == n, "topological order size");
    vector<int> pos(n + 1, -1);
    for (int i = 0; i < n; ++i) {
        check(order[i] >= 1 && order[i] <= n, "topological vertex");
        check(pos[order[i]] == -1, "topological no duplicates");
        pos[order[i]] = i;
    }
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) check(pos[u] < pos[v], "topological edge order");
}
void test_topological() {
    for (int rep = 0; rep < 250; ++rep) {
        int n = rnd(1, 40); Graph g(n + 1);
        vector<int> perm(n); iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end(), rng);
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
            if (rnd(0, 4) == 0) {
                g[perm[i]].push_back(perm[j]);
                if (rnd(0, 3) == 0) g[perm[i]].push_back(perm[j]);
            }
        check_topo(g, cp::topological_sort(g));
        g[1].push_back(1);
        check(cp::topological_sort(g).empty(), "topological self cycle");
    }
    Graph g(5); g[1] = {2}; g[2] = {3}; g[3] = {1};
    check(cp::topological_sort(g).empty(), "topological 3-cycle");
}
void test_unicyclic() {
    for (int rep = 0; rep < 400; ++rep) {
        int n = rnd(3, 22); vector<pair<int, int>> e;
        set<pair<int, int>> have;
        for (int v = 2; v <= n; ++v) {
            int u = rnd(1, v - 1); e.push_back({u, v}); have.insert({u, v});
        }
        while (true) {
            int u = rnd(1, n), v = rnd(1, n); if (u > v) swap(u, v);
            if (u == v || have.count({u, v})) continue;
            e.push_back({u, v}); have.insert({u, v}); break;
        }
        shuffle(e.begin(), e.end(), rng);
        auto r = cp::unicyclic_graph(n, e);
        check(r.ok && r.cycle.size() >= 3, "unicycle accepts valid graph");
        set<int> want, got(r.cycle.begin(), r.cycle.end());
        for (int ban = 0; ban < n; ++ban) {
            Graph g(n + 1);
            for (int i = 0; i < n; ++i) if (i != ban)
                edge(g, e[i].first, e[i].second);
            if (reachable(g, e[ban].first, e[ban].second)) {
                want.insert(e[ban].first); want.insert(e[ban].second);
            }
        }
        check(got == want && got.size() == r.cycle.size(), "cycle members");
        for (int i = 0; i < (int)r.cycle.size(); ++i) {
            int u = r.cycle[i], v = r.cycle[(i + 1) % r.cycle.size()];
            if (u > v) swap(u, v);
            check(have.count({u, v}), "cycle order has each closing edge");
        }
    }
    vector<vector<pair<int, int>>> bad = {
        {{1, 2}, {2, 3}}, {{1, 2}, {1, 2}, {2, 3}},
        {{1, 1}, {1, 2}, {2, 3}}, {{1, 2}, {2, 3}, {3, 4}},
        {{0, 1}, {1, 2}, {2, 3}}
    };
    for (auto e : bad) {
        auto r = cp::unicyclic_graph(3, e);
        check(!r.ok && r.cycle.empty(), "unicycle rejects invalid input");
    }
    auto r = cp::unicyclic_graph(6,
        {{1, 2}, {2, 3}, {3, 1}, {4, 5}, {5, 6}, {6, 4}});
    check(!r.ok && r.cycle.empty(), "unicycle rejects disconnected cycles");
    check(!cp::unicyclic_graph(0, {}).ok, "unicycle rejects empty");
}
void test_p0_deep() {
    const int n = 200000; Graph g(n + 1); cp::DSU d(n);
    for (int v = 2; v <= n; ++v) { edge(g, v - 1, v); d.unite(v - 1, v); }
    check(d.size(n) == n, "DSU deep chain");
    auto b = cp::bfs(g, 1); auto f = cp::dfs_forest(g);
    check(b.dist[n] == n - 1 && (int)b.path(n).size() == n, "BFS deep chain");
    check(f.components == 1 && f.parent[n] == n - 1, "DFS deep chain");
}
#endif

#ifdef GRAPH_P1_READY
using WeightedGraph = vector<vector<pair<int, ll>>>;
void test_dijkstra() {
    for (int rep = 0; rep < 350; ++rep) {
        int n = rnd(1, 18); WeightedGraph g(n + 1);
        vector<vector<ll>> f(n + 1, vector<ll>(n + 1, cp::DIJK_INF));
        for (int u = 1; u <= n; ++u) f[u][u] = 0;
        for (int k = 0, m = rnd(0, n * 4); k < m; ++k) {
            int u = rnd(1, n), v = rnd(1, n); ll w = rnd(0, 50);
            g[u].push_back({v, w}); f[u][v] = min(f[u][v], w);
        }
        for (int k = 1; k <= n; ++k)
            for (int u = 1; u <= n; ++u)
                for (int v = 1; v <= n; ++v)
                    f[u][v] = min(f[u][v], f[u][k] + f[k][v]);
        vector<int> sources;
        for (int k = 0, count = rnd(0, n + 2); k < count; ++k)
            sources.push_back(rnd(1, n));
        auto r = cp::dijkstra(g, sources);
        check(r.ok, "Dijkstra accepts nonnegative graph");
        for (int v = 1; v <= n; ++v) {
            ll want = cp::DIJK_INF;
            for (int s : sources) want = min(want, f[s][v]);
            check(r.dist[v] == want, "multi-source Dijkstra vs Floyd");
            auto p = r.path(v);
            if (want == cp::DIJK_INF) check(p.empty(), "Dijkstra unreachable");
            else {
                check(!p.empty() && p.back() == v, "Dijkstra path target");
                check(find(sources.begin(), sources.end(), p[0]) != sources.end(),
                      "Dijkstra path source");
                ll sum = 0;
                for (int i = 1; i < (int)p.size(); ++i) {
                    ll w = cp::DIJK_INF;
                    for (auto [to, cost] : g[p[i - 1]]) if (to == p[i])
                        w = min(w, cost);
                    check(w != cp::DIJK_INF, "Dijkstra path edge"); sum += w;
                }
                check(sum == want, "Dijkstra restored path cost");
            }
        }
        auto one = cp::dijkstra(g, 1);
        for (int v = 1; v <= n; ++v)
            check(one.dist[v] == f[1][v], "single-source Dijkstra");
    }
    WeightedGraph g(4); g[1] = {{2, cp::DIJK_INF - 2}};
    g[2] = {{3, 9}};
    auto r = cp::dijkstra(g, 1);
    check(r.ok && r.dist[2] == cp::DIJK_INF - 2 &&
          r.dist[3] == cp::DIJK_INF, "Dijkstra safe distance saturation");
    g[3] = {{3, -1}};
    check(!cp::dijkstra(g, 1).ok, "Dijkstra rejects unreachable negative edge");
    g[3].clear(); g[1][0].second = cp::DIJK_INF;
    check(!cp::dijkstra(g, 1).ok, "Dijkstra rejects sentinel-sized edge");
    g[1].clear();
    check(!cp::dijkstra(g, 0).ok, "Dijkstra rejects invalid source");
}
ll brute_msf(int n, const vector<cp::KruskalEdge>& edges, int& components) {
    Graph all(n + 1);
    for (auto e : edges) edge(all, e.u, e.v);
    components = 0;
    for (int u = 1; u <= n; ++u) {
        bool first = true;
        for (int v = 1; v < u; ++v) if (reachable(all, u, v)) first = false;
        components += first;
    }
    ll best = LLONG_MAX; int m = (int)edges.size();
    for (int mask = 0; mask < (1 << m); ++mask) {
        if (__builtin_popcount((unsigned)mask) != n - components) continue;
        Graph g(n + 1); ll cost = 0;
        for (int i = 0; i < m; ++i) if (mask >> i & 1) {
            edge(g, edges[i].u, edges[i].v); cost += edges[i].w;
        }
        bool valid = true;
        for (int u = 1; u <= n; ++u) for (int v = 1; v <= n; ++v)
            if (reachable(g, u, v) != reachable(all, u, v)) valid = false;
        if (valid) best = min(best, cost);
    }
    return best;
}
void test_mst() {
    for (int rep = 0; rep < 300; ++rep) {
        int n = rnd(1, 6), m = rnd(0, 10); WeightedGraph g(n + 1);
        vector<cp::KruskalEdge> e;
        for (int i = 0; i < m; ++i) {
            int u = rnd(1, n), v = rnd(1, n); ll w = rnd(-10, 20);
            e.push_back({u, v, w});
            g[u].push_back({v, w}); g[v].push_back({u, w});
        }
        int components; ll want = brute_msf(n, e, components);
        auto k = cp::kruskal(n, e); auto p = cp::prim(g);
        check(k.weight == want && p.weight == want, "MST vs exhaustive forests");
        check(k.components == components && p.components == components,
              "MST reports component count");
        check(k.connected == (components == 1) &&
              p.connected == (components == 1), "MST reports disconnected");
        check((int)k.edges.size() == n - components, "Kruskal chosen edges");
        Graph chosen(n + 1); ll cost = 0;
        for (int i : k.edges) {
            check(i >= 0 && i < m, "Kruskal original edge index");
            check(!reachable(chosen, e[i].u, e[i].v), "Kruskal chosen acyclic");
            edge(chosen, e[i].u, e[i].v); cost += e[i].w;
        }
        check(cost == want, "Kruskal chosen weight");
        Graph ptree(n + 1); ll pcost = 0; int roots = 0;
        for (int u = 1; u <= n; ++u) {
            int parent = p.parent[u];
            if (!parent) { ++roots; continue; }
            ll cheapest = LLONG_MAX;
            for (auto [v, w] : g[u]) if (v == parent) cheapest = min(cheapest, w);
            check(cheapest != LLONG_MAX, "Prim parent is an input edge");
            check(!reachable(ptree, u, parent), "Prim parent forest acyclic");
            edge(ptree, u, parent); pcost += cheapest;
        }
        check(pcost == want && roots == components, "Prim parent forest cost");
    }
    vector<cp::KruskalEdge> e = {{1, 2, 3000000000LL}, {2, 3, 3000000000LL}};
    check(cp::kruskal(3, e).weight == 6000000000LL, "MST 64-bit total");
    WeightedGraph g(4);
    g[1] = {{2, 3000000000LL}}; g[2] = {{1, 3000000000LL}, {3, 3000000000LL}};
    g[3] = {{2, 3000000000LL}};
    check(cp::prim(g).weight == 6000000000LL, "Prim 64-bit total");
    auto k0 = cp::kruskal(0, {}); auto p0 = cp::prim(WeightedGraph(1));
    check(!k0.connected && !p0.connected && k0.components == 0 &&
          p0.components == 0 && k0.weight == 0 && p0.weight == 0, "MST empty graph");
}
void test_tree_queries() {
    for (int rep = 0; rep < 250; ++rep) {
        int n = rnd(1, 120), root = rnd(1, n); Graph g(n + 1);
        for (int v = 2; v <= n; ++v) edge(g, rnd(1, v - 1), v);
        vector<int> par(n + 1), depth(n + 1, -1); depth[root] = 0;
        queue<int> q; q.push(root);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (depth[v] < 0) {
                depth[v] = depth[u] + 1; par[v] = u; q.push(v);
            }
        }
        cp::LCA lca(g, root); cp::EulerTree et(g, root);
        check(lca.ok && et.ok, "tree preprocessing accepts tree");
        vector<ll> val(n + 1); for (int u = 1; u <= n; ++u) val[u] = rnd(-50, 50);
        auto sum = et.subtree_sum(val);
        for (int u = 1; u <= n; ++u) {
            int p = u;
            for (int k = 0; k <= depth[u] + 1; ++k) {
                check(lca.jump(u, k) == p, "binary jump vs parent walking");
                p = par[p];
            }
            check(lca.jump(u, LLONG_MAX) == 0, "jump past root");
            int count = 0; ll total = 0;
            for (int v = 1; v <= n; ++v) {
                int x = u, y = v;
                while (depth[x] > depth[y]) x = par[x];
                while (depth[y] > depth[x]) y = par[y];
                while (x != y) { x = par[x]; y = par[y]; }
                check(lca.lca(u, v) == x, "LCA vs parent walking");
                check(lca.distance(u, v) == depth[u] + depth[v] - 2 * depth[x],
                      "LCA tree distance");
                bool inside = x == u;
                check((et.tin[u] <= et.tin[v] && et.tin[v] <= et.tout[u])
                      == inside, "Euler closed subtree interval");
                if (inside) ++count, total += val[v];
            }
            check(et.subtree_size(u) == count, "Euler subtree size");
            check(sum[u] == total, "bottom-up tree DP subtree sum");
            check(et.order[et.tin[u]] == u, "Euler inverse order is 1-indexed");
        }
    }
    Graph bad(5); edge(bad, 1, 2); edge(bad, 2, 3); edge(bad, 3, 1);
    check(!cp::LCA(bad).ok && !cp::EulerTree(bad).ok,
          "tree preprocessing rejects disconnected cyclic graph");
    Graph singleton(2);
    check(!cp::LCA(singleton, 0).ok && !cp::EulerTree(singleton, 2).ok,
          "tree preprocessing rejects invalid root");
    cp::LCA one(singleton); cp::EulerTree eo(singleton);
    check(one.jump(1, -1) == 0 && one.lca(0, 1) == 0 &&
          one.distance(0, 1) == -1, "LCA invalid queries");
    check(eo.subtree_sum({}).empty() && eo.subtree_size(0) == 0,
          "Euler invalid size/query");
    const int n = 200000; Graph chain(n + 1);
    for (int v = 2; v <= n; ++v) edge(chain, v - 1, v);
    cp::LCA lca(chain); cp::EulerTree et(chain);
    check(lca.ok && et.ok && lca.lca(n, n / 2) == n / 2,
          "LCA deep chain stack safety");
    vector<ll> val(n + 1, 1); val[0] = 0;
    auto sum = et.subtree_sum(val);
    check(sum[1] == n && sum[n] == 1 && et.tout[1] == n,
          "Euler and tree DP deep chain stack safety");
}
#endif

#ifdef GRAPH_P2_READY
void test_scc() {
    for (int rep = 0; rep < 350; ++rep) {
        int n = rnd(1, 22); Graph g(n + 1);
        for (int k = 0, m = rnd(0, n * 4); k < m; ++k)
            g[rnd(1, n)].push_back(rnd(1, n));
        auto r = cp::scc(g);
        set<int> ids; vector<int> seen(n + 1);
        for (int u = 1; u <= n; ++u) {
            ids.insert(r.comp[u]);
            for (int v = 1; v <= n; ++v)
                check((r.comp[u] == r.comp[v]) ==
                      (reachable(g, u, v) && reachable(g, v, u)),
                      "SCC vs pairwise reachability");
        }
        check((int)ids.size() == r.count && (int)r.groups.size() == r.count + 1,
              "SCC group count");
        for (int c = 1; c <= r.count; ++c) for (int u : r.groups[c]) {
            check(u >= 1 && u <= n && r.comp[u] == c, "SCC group membership");
            ++seen[u];
        }
        for (int u = 1; u <= n; ++u) check(seen[u] == 1, "SCC unique groups");
    }
}
int component_count(int n, const vector<pair<int, int>>& e, int ban) {
    Graph g(n + 1);
    for (int i = 0; i < (int)e.size(); ++i) if (i != ban)
        edge(g, e[i].first, e[i].second);
    int count = 0;
    for (int u = 1; u <= n; ++u) {
        bool first = true;
        for (int v = 1; v < u; ++v) if (reachable(g, u, v)) first = false;
        count += first;
    }
    return count;
}
void test_bridges() {
    for (int rep = 0; rep < 400; ++rep) {
        int n = rnd(1, 16); vector<pair<int, int>> e;
        for (int k = 0, m = rnd(0, 40); k < m; ++k)
            e.push_back({rnd(1, n), rnd(1, n)});
        int base = component_count(n, e, -1); vector<int> want;
        for (int ban = 0; ban < (int)e.size(); ++ban)
            if (component_count(n, e, ban) > base) want.push_back(ban);
        check(cp::bridges(n, e) == want, "bridges vs edge-removal components");
    }
    check(cp::bridges(2, {{1, 2}, {1, 2}}).empty(), "parallel edges are not bridges");
    check(cp::bridges(2, {{1, 1}, {1, 2}}) == vector<int>{1},
          "self-loop is not a bridge");
}
void test_p2_deep() {
    const int n = 200000; Graph g(n + 1); vector<pair<int, int>> e;
    for (int v = 2; v <= n; ++v) { g[v - 1].push_back(v); e.push_back({v - 1, v}); }
    auto s = cp::scc(g); auto b = cp::bridges(n, e);
    check(s.count == n && (int)b.size() == n - 1, "SCC/bridges deep chain");
    g[n].push_back(1); e.push_back({n, 1});
    check(cp::scc(g).count == 1 && cp::bridges(n, e).empty(), "large cycle");
}
#endif

int main(int argc, char** argv) {
#ifndef GRAPH_P0_READY
    check(false, "P0 graph implementations missing (expected RED)");
#else
    test_p0_boundaries(); test_dsu(); test_traversal();
    test_topological(); test_unicyclic();
    test_p0_deep();
    cout << "PASS graph P0: seed=20261024; deep_chain=200000\n";
#endif
    if (argc > 1 && string(argv[1]) == "--p0") return 0;
#ifdef P0_ONLY
    return 0;
#else
#ifndef GRAPH_P1_READY
    check(false, "P1 graph implementations missing (expected RED)");
#else
    test_dijkstra(); test_mst(); test_tree_queries();
    cout << "PASS graph P1: Dijkstra/MST/LCA/Euler; deep_chain=200000\n";
#endif
    if (argc > 1 && string(argv[1]) == "--p1") return 0;
#ifndef GRAPH_P2_READY
    check(false, "P2 graph implementations missing (expected RED)");
#else
    test_scc(); test_bridges(); test_p2_deep();
    cout << "PASS graph P2: SCC/bridges; deep_chain=200000\n";
#endif
#endif
}
