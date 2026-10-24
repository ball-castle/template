# 图模板回归记录

旧根目录源码保持原样。以下为 2026-10-24 的真实复现与新模块检查。

## 旧 binary_lifting.cpp：无法编译

命令（WSL Ubuntu，g++ 13.3.0）：

```sh
g++ -std=gnu++20 -Wall -Wextra -fsyntax-only binary_lifting.cpp
```

第 13 行引用未定义的 `log`（应为 `logn`）；第 26 行 `for(ll j = 1; j  )`
缺少分号、循环条件和循环体，编译失败。此文件实际写的是残缺 ST 表，
并非可用的树上倍增 LCA，不能只改拼写后当作 LCA 使用。

## P0 先写测试

`tests/test_graph.cpp` 先存在，正式头文件尚未存在时应以退出码 1 输出：

```text
FAIL: P0 graph implementations missing (expected RED)
```

测试用例针对：按大小合并与连通性、BFS 距离及路径、迭代 DFS 森林、
拓扑顺序/环、自环重边拒绝、基环图环成员和闭环顺序，以及 20 万深链。
随机基环图使用删边后可达性作为独立环判定，避免与实现共用剥叶逻辑。

实际结果：缺少实现时测试退出码为 1，输出与上述一致。
补齐 5 个 P0 头文件后，`-std=gnu++20 -O2 -Wall -Wextra -Wshadow`
编译零警告，测试退出码 0，输出：

```text
PASS graph P0: seed=20261024; deep_chain=200000
```

## P1/P2 的 RED 与 GREEN

新增 P1 测试、尚无 P1 头文件时，P0 通过后测试退出码 1：
`FAIL: P1 graph implementations missing (expected RED)`。
新增 P2 测试、尚无 P2 头文件时，P0/P1 通过后测试退出码 1：
`FAIL: P2 graph implementations missing (expected RED)`。
测试先固定接口与行为，再补齐实现；没有使用替身实现。

全部模块完成后，以下两种配置在 WSL Ubuntu g++ 13.3.0 上均退出 0，
编译无警告，ASan/UBSan 无报错，输出均为：

```text
PASS graph P0: seed=20261024; deep_chain=200000
PASS graph P1: Dijkstra/MST/LCA/Euler; deep_chain=200000
PASS graph P2: SCC/bridges; deep_chain=200000
```

```sh
g++ -std=gnu++20 -O2 -Wall -Wextra -Wshadow -Werror \
  tests/test_graph.cpp -o /tmp/test_graph_full
/tmp/test_graph_full

g++ -std=gnu++20 -O1 -g -Wall -Wextra -Wshadow \
  -fsanitize=address,undefined -fno-omit-frame-pointer \
  tests/test_graph.cpp -o /tmp/test_graph_san
ASAN_OPTIONS=detect_leaks=1 /tmp/test_graph_san
```

P0 可单独编译 `-DP0_ONLY` 或运行参数 `--p0`；`--p1` 检查 P0/P1。

## 独立对照与边界

| 模块 | 独立对照/边界 |
|---|---|
| DSU | 120 轮，随机连边对 BFS 可达性；块大小、重复合并、components 精确值、n=0 |
| BFS/DFS | 200 轮，对 Floyd 距离/连通性；路径逐边验证、单点、自环、重边、断图、有向 BFS、DFS 空图 |
| 拓扑 | 250 轮随机 DAG，逐边检查先后关系；重边、自环与三点有向环 |
| 基环图 | 400 轮，删边后可达性判定环成员；逐边检查环序包括首尾闭边；拒绝重边/自环/越界/不连通/错误边数 |
| Dijkstra | 350 轮单源/多源对 Floyd；恢复路径权重；零权、重边、空源、不可达、负边拒绝、INF 饱和 |
| Kruskal/Prim | 300 轮，n<=6,m<=10 枚举边子集对最小生成森林；负权/重边/自环/断图；父边/选边无环与权重；60 亿总权；n=0 |
| LCA/Euler/树 DP | 250 轮随机树、随机根；父亲逐级行走对 LCA/jump；朴素枚举对子树闭区间、大小、含负权子树和；非法根/非树/非法查询 |
| SCC | 350 轮，对每对点双向可达性；每点恰好属于一个分量；自环重边 |
| 桥 | 400 轮，对逐条删边后的连通块数；自环、重边、断图；桥 ID 按输入顺序 |

深链压力均为 200000 点：DSU、BFS、DFS、LCA、Euler/树 DP、SCC、桥；
SCC/桥还检查 200000 点大环。所有遍历实现不使用递归。

## 可复制性与打印检查

12 份头文件分别使用 `-x c++ -fsyntax-only -include <单份头文件> /dev/null`
及 `-std=gnu++20 -Wall -Wextra -Wshadow -Werror` 独立编译；不依赖其他模块。
完整测试同一翻译单元包含全部 12 份头文件，检查命名兼容。
正式源码每份 22–56 行；中文按双宽列计算，最长行 77 列，无 main。
