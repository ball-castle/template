#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../../graph/dsu.hpp"
#include "../../graph/bfs_path.hpp"
#include "../../graph/dfs_forest.hpp"
#include "../../graph/dijkstra.hpp"
#include "../../graph/lca.hpp"
#include "../../graph/euler_subtree.hpp"
#include "../../graph/topological_sort.hpp"
namespace pd {
#include "../../competition/graph/dsu.hpp"
}
namespace pb {
#include "../../competition/graph/multisource_bfs.hpp"
}
namespace pf {
#include "../../competition/graph/dfs.hpp"
}
namespace pt {
#include "../../competition/graph/lca.hpp"
#include "../../competition/graph/tree_diameter.hpp"
}
namespace pj {
#include "../../competition/graph/dijkstra.hpp"
}
namespace pg {
#include "../../competition/graph/grid_bfs.hpp"
}
namespace pz {
#include "../../competition/graph/zero_one_bfs.hpp"
}
namespace pw {
#include "../../competition/graph/floyd.hpp"
}
namespace pc {
#include "../../competition/graph/bipartite.hpp"
}
namespace py {
#include "../../competition/graph/cycle.hpp"
}
namespace pa {
#include "../../competition/graph/dag_dp.hpp"
}

mt19937 rng(20261024);
int rnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
void check(bool ok, const string& message) {
    if (!ok) { cerr << "FAIL: " << message << '\n'; exit(1); }
}
using Graph = vector<vector<int>>;
using Edge = tuple<int, int, ll>;
const ll INF = LLONG_MAX / 4;

// 独立 oracle：Bellman-Ford 每轮只读取上一轮距离。
vector<ll> oracle_dist(int n, const vector<Edge>& e, int s) {
    vector<ll> d(n + 1, INF); d[s] = 0;
    for (int t = 1; t < n; ++t) {
        auto next = d;
        for (auto [u, v, w] : e) if (d[u] != INF)
            next[v] = min(next[v], d[u] + w);
        d = next;
    }
    return d;
}

void test_dsu() {
    for (int rep = 0; rep < 120; ++rep) {
        int n = rnd(1, 16); pd::init(n); cp::DSU old(n);
        vector<int> label(n + 1); iota(label.begin(), label.end(), 0);
        for (int k = 0; k < 80; ++k) {
            int u = rnd(1, n), v = rnd(1, n);
            int a = label[u], b = label[v];
            pd::unite(u, v); old.unite(u, v);
            for (int x = 1; x <= n; ++x) if (label[x] == a) label[x] = b;
            for (int x = 1; x <= n; ++x) for (int y = 1; y <= n; ++y) {
                bool want = label[x] == label[y];
                check(pd::same(x, y) == want && old.same(x, y) == want,
                      "DSU personal/round01 vs relabel oracle");
            }
        }
    }
}

void check_cycle(const vector<pair<int, int>>& edges, bool directed) {
    check(py::cycle.front() == py::cycle.back(), "cycle closes");
    check(py::cycle.size() == py::cycle_edges.size() + 1, "cycle edge count");
    set<int> ids;
    for (int i = 0; i < (int)py::cycle_edges.size(); ++i) {
        int id = py::cycle_edges[i];
        check(id >= 0 && id < (int)edges.size(), "cycle id range");
        check(ids.insert(id).second, "cycle does not reuse input edge");
        auto [u, v] = edges[id];
        int a = py::cycle[i], b = py::cycle[i + 1];
        check((u == a && v == b) || (!directed && u == b && v == a),
              "cycle follows edge orientation");
    }
}

void test_graphs() {
    for (int rep = 0; rep < 260; ++rep) {
        int n = rnd(1, 9), m = rnd(0, 22);
        Graph g(n + 1); vector<Edge> weighted;
        vector<pair<int, int>> edges;
        pb::init(n); pf::init(n); pc::init(n); py::init(n);
        for (int id = 0; id < m; ++id) {
            int u = rnd(1, n), v = rnd(1, n);
            g[u].push_back(v); g[v].push_back(u);
            edges.push_back({u, v});
            weighted.push_back({u, v, 1}); weighted.push_back({v, u, 1});
            py::g[u].push_back({v, id}); py::g[v].push_back({u, id});
        }
        for (int u = 1; u <= n; ++u) pb::g[u] = pf::g[u] = pc::g[u] = g[u];
        vector<vector<ll>> d(n + 1);
        for (int s = 1; s <= n; ++s) d[s] = oracle_dist(n, weighted, s);
        int s = rnd(1, n); pb::bfs(s); auto old = cp::bfs(g, s);
        for (int u = 1; u <= n; ++u) {
            ll want = d[s][u] == INF ? -1 : d[s][u];
            check(pb::dist[u] == want && old.dist[u] == want, "BFS three-way");
            check(pb::path(u) == old.path(u), "BFS same parent convention");
        }
        vector<int> sources;
        for (int k = 0, count = rnd(0, n + 2); k < count; ++k)
            sources.push_back(rnd(1, n));
        pb::bfs(sources);
        for (int u = 1; u <= n; ++u) {
            ll want = INF;
            for (int start : sources) want = min(want, d[start][u]);
            check(pb::dist[u] == (want == INF ? -1 : want), "multi-source BFS");
        }
        pf::dfs_forest(); auto forest = cp::dfs_forest(g);
        check(pf::order == forest.order, "DFS preserves old adjacency order");
        int components = 0;
        for (int u = 1; u <= n; ++u) {
            bool first = true;
            for (int v = 1; v < u; ++v) if (d[u][v] != INF) first = false;
            components += first;
            check(pf::fa[u] == forest.parent[u], "DFS same parents");
            for (int v = 1; v <= n; ++v)
                check((pf::comp[u] == pf::comp[v]) == (d[u][v] != INF),
                      "DFS connectivity oracle");
        }
        check(pf::cnt == components, "DFS disconnected components");
        bool can_color = false;
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool ok = true;
            for (auto [u, v] : edges)
                if (((mask >> (u - 1)) & 1) == ((mask >> (v - 1)) & 1)) ok = false;
            can_color |= ok;
        }
        check(pc::bipartite() == can_color, "bipartite vs all 2^n colorings");
        bool has_cycle = py::find_cycle(false);
        check(has_cycle == (m > n - components), "multigraph cycle rank oracle");
        if (has_cycle) check_cycle(edges, false);
    }
    for (int rep = 0; rep < 200; ++rep) {
        int n = rnd(1, 12), m = rnd(0, 35); py::init(n);
        vector<pair<int, int>> edges;
        vector<vector<bool>> reach(n + 1, vector<bool>(n + 1));
        for (int id = 0; id < m; ++id) {
            int u = rnd(1, n), v = rnd(1, n);
            py::g[u].push_back({v, id}); edges.push_back({u, v}); reach[u][v] = true;
        }
        for (int k = 1; k <= n; ++k) for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j) reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
        bool want = false;
        for (int i = 1; i <= n; ++i) want = want || reach[i][i];
        check(py::find_cycle(true) == want, "directed cycle vs transitive closure");
        if (want) check_cycle(edges, true);
    }
}

void test_shortest_paths() {
    for (int rep = 0; rep < 250; ++rep) {
        int n = rnd(1, 18), m = rnd(0, 60), binary = rep % 2;
        vector<Edge> e;
        vector<vector<pair<int, ll>>> g(n + 1);
        pj::init(n); pz::init(n); pw::init(n);
        for (int k = 0; k < m; ++k) {
            int u = rnd(1, n), v = rnd(1, n), w = rnd(0, binary ? 1 : 50);
            e.push_back({u, v, w}); g[u].push_back({v, w});
            pj::g[u].push_back({v, w}); pz::g[u].push_back({v, w});
            pw::add_edge(u, v, w);
        }
        check(pw::floyd(), "Floyd nonnegative accepts");
        int s = rnd(1, n); auto want = oracle_dist(n, e, s);
        auto old = cp::dijkstra(g, s); check(pj::dijkstra(s) && old.ok, "Dijkstra accepts");
        if (binary) pz::bfs01(s);
        for (int v = 1; v <= n; ++v) {
            check(pj::dist[v] == want[v] && old.dist[v] == want[v], "Dijkstra three-way");
            check(pw::dist[s][v] == want[v], "Floyd vs Bellman-Ford");
            check(pj::path(v) == old.path(v), "Dijkstra restored path convention");
            if (binary) check(pz::dist[v] == (want[v] == INF ? pz::INF : want[v]), "01BFS");
        }
        vector<int> sources{rnd(1, n), rnd(1, n), rnd(1, n)};
        old = cp::dijkstra(g, sources); check(pj::dijkstra(sources), "multisource Dijkstra");
        for (int v = 1; v <= n; ++v) {
            ll best = INF;
            for (int start : sources) best = min(best, pw::dist[start][v]);
            check(pj::dist[v] == best && old.dist[v] == best, "multisource three-way");
        }
    }
    for (int rep = 0; rep < 80; ++rep) {
        int n = rnd(1, 15); pw::init(n); vector<Edge> e;
        vector<ll> potential(n + 1);
        for (int i = 1; i <= n; ++i) potential[i] = rnd(-100, 100);
        for (int k = 0; k < 35; ++k) {
            int u = rnd(1, n), v = rnd(1, n);
            ll w = rnd(0, 50) + potential[v] - potential[u];
            e.push_back({u, v, w}); pw::add_edge(u, v, w);
        }
        check(pw::floyd(), "Floyd signed weights no negative cycle");
        for (int s = 1; s <= n; ++s) {
            auto d = oracle_dist(n, e, s);
            for (int v = 1; v <= n; ++v) check(pw::dist[s][v] == d[v], "signed Floyd");
        }
    }
    pj::init(3); pj::g[1] = {{2, INF - 2}}; pj::g[2] = {{3, 9}};
    check(pj::dijkstra(1) && pj::dist[2] == INF - 2 && pj::dist[3] == INF, "Dijkstra saturation");
    pj::g[3] = {{3, -1}}; check(!pj::dijkstra(1), "Dijkstra rejects negative edge");
    pw::init(3); pw::add_edge(2, 2, -1); check(!pw::floyd(), "disconnected negative cycle");
    pw::init(30);
    for (int i = 1; i <= 30; ++i) for (int j = 1; j <= 30; ++j) pw::add_edge(i, j, -1000000000000LL);
    check(!pw::floyd(), "negative saturation avoids overflow");
}

void test_trees() {
    for (int rep = 0; rep < 120; ++rep) {
        int n = rnd(1, 70), root = rnd(1, n); pt::init(n); Graph g(n + 1);
        vector<Edge> e;
        for (int v = 2; v <= n; ++v) {
            int u = rnd(1, v - 1);
            g[u].push_back(v); g[v].push_back(u);
            pt::g[u].push_back(v); pt::g[v].push_back(u);
            e.push_back({u, v, 1}); e.push_back({v, u, 1});
        }
        check(pt::build_lca(root), "personal tree accepts"); cp::LCA old(g, root);
        cp::EulerTree euler(g, root);
        auto droot = oracle_dist(n, e, root);
        for (int u = 1; u <= n; ++u) {
            check(pt::dep[u] == droot[u], "tree depth oracle");
            ll size = 0;
            for (int v = 1; v <= n; ++v) {
                int x = v;
                while (x && x != u) x = (int)pt::fa[x];
                size += x == u;
                int a = u, b = v;
                while (pt::dep[a] > pt::dep[b]) a = (int)pt::fa[a];
                while (pt::dep[b] > pt::dep[a]) b = (int)pt::fa[b];
                while (a != b) { a = (int)pt::fa[a]; b = (int)pt::fa[b]; }
                check(pt::lca(u, v) == a && old.lca(u, v) == a, "LCA parent-walk oracle");
            }
            check(pt::sz[u] == size && euler.subtree_size(u) == size,
                  "tree subtree size personal/round01/oracle");
            int a = u;
            for (int k = 0; k <= pt::dep[u] + 1; ++k) {
                check(pt::jump(u, k) == a && old.jump(u, k) == a, "jump ancestor walk");
                a = (int)pt::fa[a];
            }
        }
        ll diameter = 0;
        for (int s = 1; s <= n; ++s) {
            auto d = oracle_dist(n, e, s);
            for (int v = 1; v <= n; ++v) {
                check(pt::distance(s, v) == d[v] && old.distance(s, v) == d[v], "tree distances");
                diameter = max(diameter, d[v]);
            }
        }
        check(pt::tree_diameter() == diameter, "diameter vs all-pair oracle");
        check((ll)pt::diameter_path.size() == diameter + 1, "diameter path length");
        check(pt::dep[root] == 0 && pt::fa[root] == 0 && pt::lca(root, n) == root,
              "diameter preserves chosen root and LCA arrays");
    }
    istringstream in("4 1 2 2 3 2 4"); pt::read_tree(in);
    check(pt::root_tree(1) && pt::sz[2] == 3, "read n-1 edges and root");
    pt::init(4); pt::g[1] = {2, 3}; pt::g[2] = {1, 3}; pt::g[3] = {1, 2};
    check(!pt::build_lca(1) && pt::tree_diameter() == -1, "disconnected cycle is not tree");
}

void test_grid() {
    for (int rep = 0; rep < 150; ++rep) {
        pg::n = rnd(1, 8); pg::m = rnd(1, 8);
        int n = pg::n, m = pg::m;
        for (int i = 1; i <= n; ++i) {
            pg::g[i] = " ";
            for (int j = 1; j <= m; ++j) pg::g[i] += (rnd(0, 3) ? '.' : '#');
        }
        vector<Edge> e;
        auto id = [m](int x, int y) { return (x - 1) * m + y; };
        for (int x = 1; x <= n; ++x) for (int y = 1; y <= m; ++y)
            if (pg::g[x][y] == '.') for (int nx = 1; nx <= n; ++nx)
                for (int ny = 1; ny <= m; ++ny)
                    if (pg::g[nx][ny] == '.' && abs(x - nx) + abs(y - ny) == 1)
                        e.push_back({id(x, y), id(nx, ny), 1});
        int sx = rnd(1, n), sy = rnd(1, m); pg::bfs(sx, sy);
        auto d = oracle_dist(n * m, e, id(sx, sy));
        for (int x = 1; x <= n; ++x) for (int y = 1; y <= m; ++y) {
            ll want = pg::g[sx][sy] == '#' || d[id(x, y)] == INF ? -1 : d[id(x, y)];
            check(pg::dist[x][y] == want, "grid BFS vs explicit graph Bellman-Ford");
        }
    }
}

void test_dag() {
    for (int rep = 0; rep < 200; ++rep) {
        int n = rnd(1, 9); pa::init(n); Graph g(n + 1);
        vector<int> p(n); iota(p.begin(), p.end(), 1); shuffle(p.begin(), p.end(), rng);
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
            for (int k = 0, copies = rnd(0, 2); k < copies; ++k) g[p[i]].push_back(p[j]);
        for (int i = 1; i <= n; ++i) pa::g[i] = g[i];
        int s = rnd(1, n); ll mod = rep % 3 == 0 ? LLONG_MAX : rnd(1, 1000000);
        check(pa::dag_dp(s, mod) && !cp::topological_sort(g).empty(), "DAG accepts");
        vector<ll> count(n + 1); vector<int> longest(n + 1, -1);
        function<void(int, int)> enumerate = [&](int u, int len) {
            ++count[u]; longest[u] = max(longest[u], len);
            for (int v : g[u]) enumerate(v, len + 1);
        };
        enumerate(s, 0);
        for (int i = 1; i <= n; ++i)
            check(pa::ways[i] == count[i] % mod && pa::dist[i] == longest[i], "DAG enumerate all paths");
        pa::g[1].push_back(1); check(!pa::dag_dp(s, mod), "DAG rejects self cycle");
    }
    pa::init(4); pa::g[1] = {2, 3}; pa::g[2] = {4}; pa::g[3] = {4};
    check(pa::dag_dp(1, 1000000007) && pa::dist[4] == 2 && pa::ways[4] == 2, "DAG manual example");
    pa::init(64);
    for (int i = 1; i < 64; ++i) pa::g[i] = {i + 1, i + 1};
    check(pa::dag_dp(1, LLONG_MAX) && pa::ways[64] == 1,
          "DAG 2^63 paths mod 2^63-1 uses wide addition");
}

void test_deep() {
    const int n = 200000;
    pd::init(n); pb::init(n); pf::init(n); pt::init(n); py::init(n); pa::init(n);
    Graph g(n + 1); cp::DSU old(n);
    for (int u = 1; u < n; ++u) {
        g[u].push_back(u + 1); g[u + 1].push_back(u);
        pd::unite(u, u + 1); old.unite(u, u + 1);
        pt::g[u].push_back(u + 1); pt::g[u + 1].push_back(u);
        py::g[u].push_back({u + 1, u}); py::g[u + 1].push_back({u, u});
        pa::g[u].push_back(u + 1);
    }
    for (int i = 1; i <= n; ++i) pb::g[i] = pf::g[i] = g[i];
    check(pd::same(1, n) && old.same(1, n), "deep DSU");
    pb::bfs(1); auto b = cp::bfs(g, 1);
    check(pb::dist[n] == n - 1 && b.dist[n] == n - 1, "deep BFS");
    pf::dfs_forest(); auto f = cp::dfs_forest(g);
    check(pf::fa[n] == n - 1 && pf::order == f.order, "deep DFS");
    check(pt::build_lca(1), "deep LCA build"); cp::LCA lca(g);
    check(pt::lca(n, n / 2) == n / 2 && lca.lca(n, n / 2) == n / 2, "deep LCA");
    check(pt::sz[1] == n && pt::tree_diameter() == n - 1, "deep subtree and diameter");
    check(!py::find_cycle(false), "deep acyclic");
    py::g[1].push_back({n, n}); py::g[n].push_back({1, n});
    check(py::find_cycle(false) && py::cycle_edges.size() == (size_t)n, "deep cycle");
    check(pa::dag_dp(1, 7) && pa::dist[n] == n - 1 && pa::ways[n] == 1, "deep DAG");
}

void test_printed_usage() {
    { // PG01
        using namespace pt;
        istringstream in("4 1 2 2 3 2 4"); read_tree(in); bool ok=root_tree(1);
        check(ok && sz[2] == 3, "PG01 usage");
    }
    { // PG02
        using namespace pd;
        init(4); unite(1,2); unite(2,1); bool ok=same(1,2);
        check(ok && !same(1,3), "PG02 usage");
    }
    { // PG03
        using namespace pb;
        init(4); g[1]={2}; g[2]={3}; bfs(1); auto p=path(3);
        check(p == vector<int>({1,2,3}) && dist[4] == -1, "PG03 usage");
    }
    { // PG04
        using namespace pf;
        init(4); g[1]={2}; g[2]={1}; g[3]={4}; g[4]={3}; dfs_forest();
        check(cnt == 2 && comp[1] != comp[3], "PG04 usage");
    }
    { // PG05
        using namespace pj;
        init(3); g[1]={{2,3},{3,1}}; g[3]={{2,1}};
        bool ok=dijkstra(1); auto p=path(2);
        check(ok && dist[2] == 2 && p == vector<int>({1,3,2}), "PG05 usage");
    }
    { // PG06
        using namespace pt;
        init(4); g[1]={2}; g[2]={1,3,4}; g[3]={2}; g[4]={2};
        bool ok=build_lca(1); int a=lca(3,4);
        check(ok && a == 2 && jump(3,2) == 1, "PG06 usage");
    }
    { // PG07
        using namespace pg;
        n=2; m=3; g[1]=" ..."; g[2]=" .#."; bfs(1,1);
        check(dist[2][3] == 3 && dist[2][2] == -1, "PG07 usage");
    }
    { // PG08
        using namespace pb;
        init(5);
        for(int i=1;i<5;++i){g[i].push_back(i+1);g[i+1].push_back(i);}
        bfs(vector<int>{1,5,1});
        check(vector<int>(dist+1,dist+6) == vector<int>({0,1,2,1,0}), "PG08 usage");
    }
    { // PG09
        using namespace pz;
        init(3); g[1]={{2,1},{3,0}}; g[3]={{2,0}}; bfs01(1);
        check(dist[2] == 0, "PG09 usage");
    }
    { // PG10
        using namespace pw;
        init(3); add_edge(1,2,4); add_edge(2,3,-2); add_edge(1,3,9); bool ok=floyd();
        check(ok && dist[1][3] == 2, "PG10 usage");
    }
    { // PG11
        using namespace pc;
        init(3); g[1]={2}; g[2]={1,3}; g[3]={2}; bool ok=bipartite();
        check(ok, "PG11 usage");
        g[1].push_back(3); g[3].push_back(1);
        check(!bipartite(), "PG11 triangle regression");
    }
    { // PG12
        using namespace py;
        init(2); g[1]={{2,0},{2,1}}; g[2]={{1,0},{1,1}}; bool ok=find_cycle(false);
        check(ok && cycle_edges.size() == 2, "PG12 usage");
    }
    { // PG13
        using namespace pt;
        init(4); g[1]={2,3,4}; g[2]={1}; g[3]={1}; g[4]={1}; int len=tree_diameter();
        check(len == 2, "PG13 usage");
    }
    { // PG15
        using namespace pa;
        init(4); g[1]={2,3}; g[2]={4}; g[3]={4}; bool ok=dag_dp(1,1000000007);
        check(ok && dist[4] == 2 && ways[4] == 2, "PG15 usage");
    }
}

int main() {
    test_printed_usage();
    test_dsu(); test_graphs(); test_shortest_paths(); test_trees();
    test_grid(); test_dag(); test_deep();
    cout << "PASS graph personal + round01 + independent oracles; seed=20261024; deep=200000\n";
}
