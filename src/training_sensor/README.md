# training_sensor（M3）

Task 03 的节点骨架：发布自定义消息 + 提供重置服务。

| 项 | 内容 |
|---|---|
| 可执行 | `sensor_node` |
| 发布 | `/sensor_reading`（`training_interfaces/msg/SensorReading`） |
| 服务 | `/reset_counter`（`training_interfaces/srv/ResetCounter`） |
| 参数 | `sensor_id`、`publish_rate_hz`、`alarm_threshold` |

## 构建与运行

先确保 `training_interfaces` 已按任务文档填写并构建，然后：

```bash
colcon build --symlink-install --packages-select training_interfaces training_sensor
source install/setup.bash

ros2 run training_sensor sensor_node
```

## 只改这里

- `src/sensor_node.cpp`
- `launch/sensor.launch.py`

`CMakeLists.txt` / `package.xml` 已配好依赖与链接，这一课不需要修改。
完整任务见 [`../../docs/student/tasks/03-interfaces-and-services.md`](../../docs/student/tasks/03-interfaces-and-services.md)。
