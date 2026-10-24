# 2026-10-24 成都铜牌模板重建

从旧提交 `8c9f89adb51980acd6f16b8da3e2e9c344130423` 建立分支 `codex/icpc-chengdu-bronze-template-20261024`。旧 32 份根目录 cpp 全部保留，以哈希清单和 legacy 索引追溯。

## 本次交付

- 建立 32 个正式 GNU++20 模块（17 P0、13 P1、2 P2），每个模块包含识别信号、调用约定、复杂度、错误、例题、测试入口；代码和打印稿共用唯一来源。
- 修复旧 Fenwick、Lazy 线段树、模幂、乘积递推及残缺 ST 草稿对应的正式实现；保留失败反例，不静默覆盖旧文件。
- 建立独立编译、随机差分、边界、20 万深链和 ASan/UBSan 测试；两套完整配置各 39 项检查通过。
- 交付可编辑的 24 页成都急用 DOCX/PDF、41 页长期完整 DOCX/PDF。含前置思维卡片、排错、索引、页码；已核对源码与文档，并完成全页渲染检查。
- 完成全部 32 份旧模板审计、2024/2025 成都全部 26 题官方题解分析、奖牌线证据、能力画像和 4–8 周训练安排。
- 新增 README、分类与个人使用成本索引、文档生成脚本、导出哈希配对、过期 PDF 回归门禁及 GitHub Actions。
- 新增 .gitignore 和新文件 LF 规范，阻止未来构建产物入库；不删除历史训练记录或已跟踪 exe。

## 范围与限制

当前正式源码均在明确契约内通过本地验证；证据见 [VALIDATION_REPORT.md](VALIDATION_REPORT.md)。旧目录仍含故障/残缺代码，其状态见 [TEMPLATE_AUDIT.md](TEMPLATE_AUDIT.md)，不可按“所有 cpp 都已修好”理解。

P2 仅选 SCC 和桥；其他高级候选未实施。未向线上评测站提交这些新模块。QOJ 提交记录无法公开核验；CF 文件存在不等于 AC。本届纸质资料的数量与规格仍待赛站确认，两年样本也不能预测 2026 命题或固定铜牌过题数。

本机通过 Word 导出 PDF、Poppler 渲染；Linux 的 LibreOffice 复现路径已提供，但本次没有在缺少 LibreOffice 的环境中假称完成该路径验证。其他机器必须重查字体与分页。

## 两仓库发布与合并顺序

1. 先提交、推送此独立 template 分支并创建 PR。
2. cf 的 `codex/icpc-chengdu-submodule-20261024` 分支补齐 `.gitmodules`，以 mode 160000 指向已在远端可获取的 template 提交。
3. 先合并 template PR，再合并 cf PR。建议使用保留所引用提交的 merge commit；若 squash/rebase 产生新提交，应将 cf gitlink 更新到最终 main 提交并重新验证后再合并 cf。
4. 新环境以 `git clone --recurse-submodules` 获取；已有克隆执行 `git submodule update --init --recursive`。

本轮只创建分支与 PR，不直接写 main，不 force push，不改写历史。实际提交链接和新克隆证据见 cf 的 `DELIVERY_20261024.md`。
