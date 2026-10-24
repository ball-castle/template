# 个人铜牌手册 V2 验证报告

2026-10-25。主线是用户已经使用验证过的代码；普通整数直接采用原 `read.cpp`，没有改函数、接口或正文。树的存储、DSU、Fenwick、图遍历、数位 DP 等均有具体旧源路径；新写内容单独注明。在线提交状态未核验的仍为 UNKNOWN。

## 统一代码验证

100 个模块编号（32 个保留、68 个本轮条目）全部有当前源码/测试/本地 include 依赖的 LF 归一化 SHA256。每个展开后的打印模块独立编译；16 组功能测试、16 个模型卡完整 main、12 个模块完整例程运行通过。两套配置各 188 项检查，编译诊断均为空。

| 配置 | 环境 | 结果 | 完整证据 |
|---|---|---|---|
| release | Windows GNU g++ 15.2，GNU++20，O2，Wall/Wextra/Wshadow | PASS | [release.json](notes/round02/validation/release.json) |
| ASan + UBSan | WSL Ubuntu GNU g++ 13.3，O1，地址/未定义行为检查，遇错停止 | PASS | [sanitizers.json](notes/round02/validation/sanitizers.json) |

这些是独立预期值、朴素枚举或实际操作搜索的对照，不只比较两个相似实现。包括区间操作 BFS 与区间构造重放、混合 lazy 的赋值0和顺序、最大子段全负、频率非负约束、DP 状态/初值/遍历、Trie 计数/删除/容量、堆与有序集合随机操作、20万点深链、多组清空。保留的新旧核心实现共用随机输入并各自对照独立 oracle。具体种子、规模、命令和输出见报告及专题 `*_sources.json`。

独立审查另检查了任意整数每次费用1的零和分组证明、二进制 Trie、XOR 图约束、X11 的笛卡尔树与小并大；补充299584个 X11 数组直接枚举、155个区间操作 BFS 目标和729个约束图，无需修改的高/中风险问题。证据：[independent_review.json](notes/round02/validation/independent_review.json)。DP 独立审查发现的新计数函数循环边界已加回归并修复，个人数位原稿未改。

统一验证期间曾编译到正在修复前的 DP 文件；已终止旧进程、重新编译并完成全部配置。证据绑定的是最终源码。新的测试入口会比较编译前后依赖哈希，拒绝把并发改动期间的测试结果当作当前结果。新增 i128 测试去除了本机不存在的 Boost 依赖，使用原已验证的除10输出和明确数值边界；不修改原 ll 快读。

## 打印产物与一致性

- [赛前小技巧与题型补丁 DOCX](板子打印版/成都站_赛前小技巧与题型补丁_2026.docx) / [PDF](板子打印版/成都站_赛前小技巧与题型补丁_2026.pdf)：25页，586行规范代码。
- [算法与模型 V2 DOCX](板子打印版/ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2.docx) / [PDF](板子打印版/ACM_ICPC_铜牌稳妥覆盖_算法与模型_V2.pdf)：280页，4775行规范代码，含130张模型卡、个人比赛源码及40项逐条STL API教学。

Word 中每个 Code 段落与规范源码递归展开或实际受测 main 逐行相同。本地 include 被展开，不能只抄六行包装头。实际 PDF 页数、A4尺寸、正文、每页页码、目录 PAGEREF 和所有模块位置均核对；Word/PDF导出哈希成对绑定。附录条目在文末集中检索，覆盖表记录其真实页码。

采用现有 Microsoft Word `ExportAsFixedFormat` 导出和 Poppler 130dpi 全页渲染。此环境缺少 LibreOffice；没有将其未执行的渲染路径说成已验证。初稿全页实看后修复孤立函数尾行和样例输出页。最终补丁全部页有原尺寸视觉证据；V2 首稿276页全页实看，最终246页正文像素与已检页面完全相同，34个新增/变化页再逐页实看，另复核页脚样本与全部数字页码。证据：[补丁QA](notes/round02/validation/visual_patch_final.json)、[V2 QA](notes/round02/validation/visual_full_final.json)、[像素映射](notes/round02/validation/visual_full_pixel_mapping.json)。

检查程序也用故意破坏的导出绑定、代码文本、源码哈希和索引书签验证拒绝机制。`scripts/test_round02_print_gate.py` 通过。旧24页/41页两册的四个文件哈希完全不变，原32正式模块和32历史源文件保持原内容。

## 覆盖与范围

[铜牌覆盖矩阵](BRONZE_COVERAGE_MATRIX.md)按319个知识点检查，其中310项必需内容就绪、9项OPTIONAL；259项有代码、51项为明确的概念/证明框架。模型框架没有被冒充成通用求解器，例如任意指定位置操作的不变量、通用构造思路和区间MEX的延伸。

[STL与数据结构验收](STL_AND_DATA_STRUCTURE_COVERAGE.md)的90项分别核验场景、调用、复杂度、错误、测试、真实打印位置。保留“用户使用验证”和“公开线上AC”两种不同证据；未进行新的线上提交。

## 复现

```sh
python3 scripts/run_round02_tests.py
python3 scripts/run_round02_tests.py --sanitize
python3 scripts/build_round02_books.py --book patch
python3 scripts/build_round02_books.py --book full
```

随后用 `scripts/export_word.ps1` 对两份DOCX导出同名PDF，运行 `scripts/check_round02_print.py`、`scripts/test_round02_print_gate.py`，并渲染全页重新检查。不要把其他机器的字体和分页假定为相同。GitHub Actions运行同一统一代码入口；最终提交和新克隆记录在父仓库 `DELIVERY_ROUND02.md`，两个PR保持待审，不自动合并。
