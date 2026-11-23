# training_capstone（M4，可选）

Task 04 的旁路监控进程骨架：只订阅、不控制。

| 项 | 内容 |
|---|---|
| 可执行 | `monitor_node` |
| 订阅 | `/sensor_reading`（`training_interfaces/msg/SensorReading`） |
| 参数 | `topic`、`report_period_s` |
| 服务 | `~/reset_stats`（`std_srvs/srv/Empty`） |
| launch | `launch/capstone.launch.py`（同时启动 `training_sensor` 的 `sensor_node`） |

## 构建与运行

```bash
colcon build --symlink-install --packages-up-to training_capstone
source install/setup.bash

ros2 launch training_capstone capstone.launch.py
```

## 只改这里

- `src/monitor_node.cpp`
- `launch/capstone.launch.py`

`CMakeLists.txt` / `package.xml` 已配好，这一课不需要修改。
完整任务见 [`../../docs/student/tasks/04-capstone-monitor.md`](../../docs/student/tasks/04-capstone-monitor.md)。
