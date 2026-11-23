# Task 02：参数与 launch

| 模块 | 预计时间 | 前置 | 分支 |
|---|---|---|---|
| M2 | 1.5 天 | Task 01 已合并 | `task/02-params-launch` |

## 1. 学习目标

- 用参数把「行为」从代码里拿出来，不重编译就能改
- 用 launch 编排多个进程与 namespace
- 理解参数校验、动态更新、定时器重建
- 养成「默认值 + 校验 + 无魔法数字」的习惯

## 2. 交付物

在 `src/training_params` 中完成：

| 项 | 内容 |
|---|---|
| 节点 `sensor` | 发布 `/temperature`（`std_msgs/msg/Float64`），周期与告警阈值由参数控制 |
| 参数 | `sensor_id`（string，默认 `sensor_0`）、`publish_rate_hz`（double，默认 `2.0`）、`alarm_threshold`（double，默认 `30.0`） |
| launch | `launch/sensor.launch.py` 一次启动 `sensor_a`、`sensor_b` 两份实例，参数不同 |
| 配置 | `config/sensor.yaml` 作为参数文件示例 |

要求：

- 启动后从参数读取配置；`ros2 param set` 修改 `publish_rate_hz` 或 `alarm_threshold` 立即生效
- `publish_rate_hz` 必须 `> 0` 且 `<= 100`；非法值拒绝并保留旧值，日志说明原因
- 修改频率时先取消旧定时器，再创建新定时器
- 节点名固定为 `sensor`，靠 namespace 区分实例

## 3. 骨架说明

- 可以改：`src/sensor_node.cpp`、`launch/sensor.launch.py` 的 `TODO`
- 可以按需调整 `config/sensor.yaml`
- 不要改：`CMakeLists.txt`、`package.xml`
- 不要引入新的第三方库

## 4. 验收

```bash
colcon build --symlink-install --packages-select training_params
source install/setup.bash

ros2 launch training_params sensor.launch.py
```

另开终端：

```bash
ros2 node list
# 预期包含 /sensor_a/sensor 和 /sensor_b/sensor

ros2 param list /sensor_a/sensor
# 预期有 sensor_id / publish_rate_hz / alarm_threshold

timeout 5 ros2 topic hz /sensor_a/temperature
# 预期约 2 Hz

ros2 param set /sensor_a/sensor publish_rate_hz 5.0
timeout 5 ros2 topic hz /sensor_a/temperature
# 预期约 5 Hz，且不需要重新编译

ros2 param set /sensor_a/sensor publish_rate_hz -1.0
# 预期：拒绝并保留旧值（CLI 可能提示失败，属预期）

ros2 param get /sensor_a/sensor publish_rate_hz
# 预期仍是 5.0

ros2 param dump /sensor_a/sensor
# 预期能看到当前参数值
```

再用 `ros2 topic echo /sensor_b/temperature --once` 确认两份实例互不影响。

## 5. 约束

- 参数必须声明并给默认值，禁止把默认值散落在回调里
- 非法参数不能导致节点退出或崩溃
- 定时器重建顺序必须是「先 cancel 旧的，再创建新的」
- 不修改构建文件
- 代码通过 `clang-format`

## 6. 提示

**提示 1**：读官方教程的「Using parameters in a class (C++)」和
「Launching nodes」两节；重点看 `declare_parameter`、`add_on_set_parameters_callback`。

**提示 2**：参数回调返回 `rcl_interfaces::msg::SetParametersResult`；频率变化时用
`rclcpp::create_wall_timer` 重建定时器；launch 里用 `namespace=` 和 `parameters=`。

**提示 3**：仅在 Issue 中说明已尝试超过 2 小时后，导师会在评论里给关键片段。

## 7. 自检清单

- [ ] 两个 namespace 的实例都在运行且互不影响
- [ ] 参数有默认值，`ros2 param list` 能看到
- [ ] `param set` 改变频率后 `topic hz` 立即变化
- [ ] 非法频率被拒绝，旧值保留
- [ ] 没有出现两个定时器同时发布（频率翻倍）
- [ ] 构建 0 warning，`clang-format` 干净
- [ ] PR 描述里贴了 `param set` 前后的 `topic hz` 输出

## 8. 完成定义（DoD）

- 验收命令逐条跑过，输出贴在 PR
- 验收命令本地通过（导师会抽查复跑）
- 无 `must-fix` 遗留
- 能解释：参数、launch、namespace 各自解决什么问题

## 9. 提交要求

- 分支：`task/02-params-launch`
- 提交信息：`task-02: ...`
- PR 标题：`Task 02: 参数与 launch`
- PR 描述包含：参数表、launch 结构、验收输出
