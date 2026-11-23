# Task 04（可选）：旁路监控进程

| 模块 | 预计时间 | 前置 | 分支 |
|---|---|---|---|
| M4 | 1–1.5 天 | Task 03 已合并 | `task/04-capstone-monitor` |

## 1. 学习目标

- 练习「旁路进程」模式：只订阅、不控制，出问题不影响主链路
- 处理「没有数据」「数据忽快忽慢」等真实情况
- 为进入 `RUS_Engine` 的 topic inspector 任务做准备

## 2. 交付物

在 `src/training_capstone` 中完成 `monitor_node`：

| 项 | 内容 |
|---|---|
| 订阅 | `/sensor_reading`（`training_interfaces/msg/SensorReading`） |
| 参数 | `topic`（默认 `/sensor_reading`）、`report_period_s`（默认 `5.0`） |
| 统计 | 窗口内的消息数、频率、`value` 的 min/max/avg |
| 服务 | `~/reset_stats`（`std_srvs/srv/Empty`），清空统计并返回 |
| 日志 | 每个报告周期输出一次统计；没有数据时输出「等待数据」，不要刷屏 |
| 稳定性 | 发布者不存在、中途退出、恢复，都不能崩溃或无限刷日志 |

launch `capstone.launch.py` 同时启动 `training_sensor` 的 `sensor_node` 和本节点。

## 3. 骨架说明

- 可以改：`src/monitor_node.cpp`、`launch/capstone.launch.py` 的 `TODO`
- 不要改：`CMakeLists.txt`、`package.xml`
- 依赖 Task 03 的接口；先确保 Task 03 已合并

## 4. 验收

```bash
colcon build --symlink-install --packages-up-to training_capstone
source install/setup.bash

ros2 launch training_capstone capstone.launch.py
```

另开终端：

```bash
ros2 node list
ros2 topic hz /sensor_reading
# 每 5 秒左右能在 launch 输出里看到统计：count / hz / min / max / avg

ros2 service call /monitor/reset_stats std_srvs/srv/Empty
# 预期：成功，之后统计从 0 重新开始

# 停掉 sensor（Ctrl+C 只停 sensor，或单独启动 sensor 方便控制），观察 monitor：
# 预期：输出「等待数据」，不崩溃、不刷屏；重新启动 sensor 后统计恢复
```

## 5. 约束

- 只订阅，不发布控制类话题，不调用运动相关服务
- 统计更新和日志不要在回调里做重活
- 没有数据时不能忙等（不要空转循环）
- 服务清空统计后，频率统计要重新开始
- 代码通过 `clang-format`

## 6. 提示

**提示 1**：参考 Task 03 的订阅写法；统计周期用定时器，不要在消息回调里每帧打印。

**提示 2**：`std_srvs/srv/Empty` 的服务回调只有 request/response；用
`RCLCPP_INFO_THROTTLE` 或按 `report_period_s` 控制日志；用 `std::optional` 或计数
区分「还没有数据」。

**提示 3**：仅在 Issue 中说明已尝试超过 2 小时后，导师会在评论里给关键片段。

## 7. 自检清单

- [ ] 有数据时统计值与 `ros2 topic hz` 基本一致
- [ ] `reset_stats` 后统计归零并重新增长
- [ ] 发布者不存在时节点不崩溃、不刷屏
- [ ] 发布者恢复后统计继续，无需重启 monitor
- [ ] 构建 0 warning，`clang-format` 干净
- [ ] 跑过一次 sanitizer

## 8. 完成定义（DoD）

- 验收命令逐条跑过，输出贴在 PR
- CI 绿
- 无 `must-fix` 遗留
- 能解释：为什么旁路进程比直接改主链路更安全

## 9. 提交要求

- 分支：`task/04-capstone-monitor`
- 提交信息：`task-04: ...`
- PR 标题：`Task 04: 旁路监控进程`
- PR 描述包含：统计口径、异常场景处理、验收输出
