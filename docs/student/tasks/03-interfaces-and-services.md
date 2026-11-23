# Task 03：自定义 msg/srv 与服务

| 模块 | 预计时间 | 前置 | 分支 |
|---|---|---|---|
| M3 | 2 天 | Task 02 已合并 | `task/03-interfaces-services` |

## 1. 学习目标

- 自己定义 `.msg` / `.srv`，理解 IDL 到 C++ 结构体的映射
- 理解接口包为什么单独存在、依赖方向为什么重要
- 会写服务端并处理请求/响应
- 顺带练习值语义、`const&` 传参、`[[nodiscard]]`

## 2. 交付物

### 2.1 `src/training_interfaces`

把占位字段替换成下面的定义（文件里已有规格注释）：

`msg/SensorReading.msg`：

```text
builtin_interfaces/Time stamp
string sensor_id
float64 value
uint8 status

uint8 STATUS_OK=0
uint8 STATUS_WARN=1
uint8 STATUS_ERROR=2
```

`srv/ResetCounter.srv`：

```text
string sensor_id
---
bool success
uint64 reset_count
```

### 2.2 `src/training_sensor`

| 项 | 内容 |
|---|---|
| 节点 | `sensor_node`，发布 `/sensor_reading`（`training_interfaces/msg/SensorReading`） |
| 参数 | `sensor_id`（默认 `sensor_0`）、`publish_rate_hz`（默认 `2.0`）、`alarm_threshold`（默认 `30.0`） |
| 服务 | `/reset_counter`（`training_interfaces/srv/ResetCounter`），返回重置前的发布计数 |
| 行为 | `value` 超过阈值时 `status=STATUS_WARN`，否则 `STATUS_OK`；`stamp` 用当前 ROS 时间 |
| 服务校验 | 请求 `sensor_id` 与节点参数不一致时 `success=false`，计数不变 |

## 3. 骨架说明

- `training_interfaces`：只改 `msg/`、`srv/` 两个文件，接口包的 `CMakeLists.txt` / `package.xml` 已配好
- `training_sensor`：改 `src/sensor_node.cpp`、`launch/sensor.launch.py` 的 `TODO`
- 不要改两个包里的 `CMakeLists.txt` / `package.xml`（本课已预置依赖与链接）
- 先构建接口包，再构建节点包

## 4. 验收

```bash
colcon build --symlink-install --packages-select training_interfaces training_sensor
source install/setup.bash

ros2 interface show training_interfaces/msg/SensorReading
ros2 interface show training_interfaces/srv/ResetCounter
```

预期：字段与第 2 节完全一致，没有 `placeholder`。

启动节点：

```bash
ros2 run training_sensor sensor_node
```

另开终端：

```bash
ros2 topic info /sensor_reading --verbose
ros2 topic echo /sensor_reading --once
# 预期：stamp / sensor_id / value / status 都有值

ros2 service list | grep reset_counter
ros2 service call /reset_counter training_interfaces/srv/ResetCounter "{sensor_id: 'sensor_0'}"
# 预期：success=True，reset_count 为当前已发布条数

sleep 2
ros2 service call /reset_counter training_interfaces/srv/ResetCounter "{sensor_id: 'sensor_0'}"
# 预期：reset_count 重新从少量开始增长

ros2 service call /reset_counter training_interfaces/srv/ResetCounter "{sensor_id: 'other'}"
# 预期：success=False，计数不变
```

## 5. 约束

- 接口字段严格按第 2 节，不改名、不加多余字段
- 服务返回前不要打印与请求无关的噪声日志
- 计数用整数类型，避免浮点误差
- 回调里不阻塞、不让异常逃逸
- 代码通过 `clang-format`

## 6. 提示

**提示 1**：读官方教程的「Creating custom msg and srv files」和
「Writing a simple service and client (C++)」；重点看接口包结构和服务回调签名。

**提示 2**：接口包的 CMake 用 `rosidl_generate_interfaces`；节点包需要
`find_package(training_interfaces)`、`ament_target_dependencies` 和
`ament_target_dependencies`。服务回调参数是 request/response 的 `shared_ptr`。

**提示 3**：仅在 Issue 中说明已尝试超过 2 小时后，导师会在评论里给关键片段。

## 7. 自检清单

- [ ] `ros2 interface show` 两个接口都正确，无占位字段
- [ ] 先构建接口包再构建节点包，没有链接错误
- [ ] `/sensor_reading` 有数据，`stamp` 在更新
- [ ] 服务返回 `reset_count` 符合预期
- [ ] `sensor_id` 不匹配时 `success=False`
- [ ] 阈值前后 `status` 会变化
- [ ] 构建 0 warning，`clang-format` 干净
- [ ] M3 完成前跑过一次 sanitizer

## 8. 完成定义（DoD）

- 验收命令逐条跑过，输出贴在 PR
- CI 绿
- 无 `must-fix` 遗留
- 能解释：为什么接口要单独成包、改字段后为什么要先构建接口包

## 9. 提交要求

- 分支：`task/03-interfaces-services`
- 提交信息：`task-03: ...`
- PR 标题：`Task 03: 自定义 msg/srv 与服务`
- PR 描述包含：接口定义、服务行为、验收输出、sanitizer 结果
