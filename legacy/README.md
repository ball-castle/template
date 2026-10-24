# 旧源码索引（不复制）

原32份源码保留在template根目录，基线提交为`8c9f89adb51980acd6f16b8da3e2e9c344130423`。本目录只索引，不搬移、不复制、不覆盖。比赛复制代码请走正式模块；旧源状态见[TEMPLATE_AUDIT.md](../TEMPLATE_AUDIT.md)。

- [逐文件SHA256与诊断](../notes/legacy_manifest.json)：同时记录Windows原字节与Git LF blob哈希。
- [审计复现脚本](../tests/legacy_audit.py)：编译与行为验证输出到忽略目录`_audit/`。
- 旧源统计：14 VERIFIED（限定契约）、10 REFERENCE_ONLY、7 INCOMPLETE、1 UNVERIFIED。5个正式替代FIXED不会改变原文件。

| 原源码 | 原状态 |
|---|---|
| [01bag.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/01bag.cpp) | VERIFIED |
| [bfs.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/bfs.cpp) | VERIFIED |
| [binary_lifting.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/binary_lifting.cpp) | INCOMPLETE |
| [binary_search_num.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/binary_search_num.cpp) | VERIFIED |
| [data_sturcture.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/data_sturcture.cpp) | REFERENCE_ONLY |
| [dfs.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/dfs.cpp) | VERIFIED |
| [DSU.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/DSU.cpp) | VERIFIED |
| [Fenwick_tree.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Fenwick_tree.cpp) | INCOMPLETE |
| [gcd.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/gcd.cpp) | VERIFIED |
| [graph_paint.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/graph_paint.cpp) | INCOMPLETE |
| [interaction.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/interaction.cpp) | INCOMPLETE |
| [KMP.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/KMP.cpp) | VERIFIED |
| [kruskal MST.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/kruskal%20MST.cpp) | VERIFIED |
| [ksm.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/ksm.cpp) | REFERENCE_ONLY |
| [lambda.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/lambda.cpp) | REFERENCE_ONLY |
| [linklist.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/linklist.cpp) | REFERENCE_ONLY |
| [lower_bound.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/lower_bound.cpp) | REFERENCE_ONLY |
| [matrix_ksm.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/matrix_ksm.cpp) | REFERENCE_ONLY |
| [mex.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/mex.cpp) | INCOMPLETE |
| [Mobius_inversion.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Mobius_inversion.cpp) | REFERENCE_ONLY |
| [prim MST.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/prim%20MST.cpp) | VERIFIED |
| [read.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/read.cpp) | REFERENCE_ONLY |
| [segmen tree1.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/segmen%20tree1.cpp) | VERIFIED |
| [Segment tree2.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Segment%20tree2.cpp) | INCOMPLETE |
| [sort.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/sort.cpp) | REFERENCE_ONLY |
| [ST表.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/ST%E8%A1%A8.cpp) | VERIFIED |
| [Ternary_Search.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/Ternary_Search.cpp) | UNVERIFIED |
| [tree_dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/tree_dp.cpp) | INCOMPLETE |
| [tree_storage.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/tree_storage.cpp) | REFERENCE_ONLY |
| [区间dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E5%8C%BA%E9%97%B4dp.cpp) | VERIFIED |
| [完全背包dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E5%AE%8C%E5%85%A8%E8%83%8C%E5%8C%85dp.cpp) | VERIFIED |
| [线性dp.cpp](https://github.com/ball-castle/template/blob/8c9f89adb51980acd6f16b8da3e2e9c344130423/%E7%BA%BF%E6%80%A7dp.cpp) | VERIFIED |
