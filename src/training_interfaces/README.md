# training_interfaces（M3）

Task 03 的接口包骨架。两个文件目前是占位字段，按任务文档替换：

- `msg/SensorReading.msg`
- `srv/ResetCounter.srv`

## 构建

```bash
colcon build --symlink-install --packages-select training_interfaces
source install/setup.bash
ros2 interface show training_interfaces/msg/SensorReading
ros2 interface show training_interfaces/srv/ResetCounter
```

`CMakeLists.txt` 与 `package.xml` 已配好，不需要修改。
完整任务见 [`../../docs/student/tasks/03-interfaces-and-services.md`](../../docs/student/tasks/03-interfaces-and-services.md)。
