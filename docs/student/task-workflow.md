# 任务流程

> 一个任务 = 一个 Issue = 一个分支 = 一个 PR。任务要求以 `tasks/` 下的文档和 Issue 为准。
> 文档写法见 [`doc-style.md`](doc-style.md)。

## 1. 开始前

- [ ] 环境可用：`colcon build` 能通过，`source install/setup.bash` 后 `ros2 run` 正常
- [ ] 已读对应任务文档
- [ ] 已认领 Issue（导师 assign 给你）
- [ ] `dev` 是最新的

`dev` 是日常任务分支；`main` 由导师维护，不要直接往 `main` 提 PR 或 push。

## 2. 流程总览

```text
认领 Issue
  → 同步 dev
  → 开分支
  → 实现 TODO
  → colcon build + 跑验收命令
  → clang-format + sanitizer
  → 自查
  → 提 PR（附命令输出）
  → CI 绿
  → 审查 / 修改
  → squash merge
  → 删除分支
```

## 3. 同步 dev、开分支、提交

```bash
git checkout dev
git pull origin dev

git checkout -b task/01-nodes-topics
```

分支命名：`task/<两位编号>-<简短英文描述>`。

提交信息：

```text
task-01: 实现温度发布者
task-01: 增加阈值告警
```

一次提交只做一件事；不要提交 `build/`、`install/`、`log/`。

不要在一个分支里做下一课的内容。

## 4. 提 PR 前自测

逐条执行任务文档里的验收命令，并把**关键输出**贴进 PR：

- `colcon build` 结果（无 warning）
- 话题频率 / `ros2 topic echo`
- 服务调用返回
- `clang-format --dry-run --Werror`（见下）
- sanitizer 结果（M2、M3）
- 改动 diff 自查：没有调试残留、没有注释掉的代码

```bash
# 格式检查（与 CI 一致）
git ls-files -z '*.cpp' '*.hpp' '*.h' | xargs -0 -r clang-format --dry-run --Werror
```

格式以仓库根目录 `.clang-format` 为准，不要手改格式规则。

## 5. PR 描述

- 关联 Issue：`Closes #12`
- 目标分支：`dev`
- 改了什么（3 行以内）
- 数据流：谁发布 → 哪个话题/服务 → 谁订阅/调用
- 自测命令与输出
- PR 模板里的自检清单全部勾选
- 仍未解决的问题（没有就写「无」）

## 6. CI 失败怎么办

1. 点开失败 job，只读第一条错误
2. 本地复现同一条命令
3. 修完 push，CI 自动重跑
4. 连续两次修不好，在 PR 里贴错误和已尝试的命令，@ 导师

## 7. 卡住时怎么问

在 Issue/PR 评论里给三样东西：

```text
我执行了：<命令>
我期望：<结果>
我实际看到：<完整报错>
我猜的原因是：<一句话>
```

只发「跑不起来」会被要求补信息。

## 8. 审查与合并

- 审查意见分 `must-fix` / `should-fix` / `nit` / `question`
- `must-fix` 改完回复「已修改 + 新 commit」，不要 force push 覆盖审查历史
- PR 合入 `dev`；通过后由导师 squash merge，你删除远程分支
- `main` 由导师定期从 `dev` 合并，不需要你操作
- 前两个 PR 建议和导师共享屏幕过一遍

## 9. 什么情况不能合并

- CI 红
- 构建有 warning
- 验收命令没跑或输出对不上
- 改了任务文档里禁止修改的文件（`CMakeLists.txt` / `package.xml`，任务特别说明除外）
- 有未解决的 `must-fix`
- PR 超过 300 行（拆小）
