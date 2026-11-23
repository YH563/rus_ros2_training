# C++ 约定

目标：写出能被审查、能被维护的 ROS 2 节点。以下规则不是风格洁癖，都是审查时会看重的点。
格式以仓库根目录 `.clang-format` 为准（当前：基于 LLVM，4 空格缩进，100 列，左指针，
`public:` / `private:` 顶格）。不要手写与它冲突的格式。

## 1. 标准与基本习惯

- 使用 C++17，不引入 C++20 特性
- 优先值语义和 RAII：资源在构造时获取、析构时释放
- **禁止裸 `new/delete`**；需要共享所有权时用 `std::shared_ptr`，独占用 `std::unique_ptr`，
  只借用时用引用或裸指针（并保证生命周期）
- 不使用 C 风格数组，用 `std::array` / `std::vector`
- 不使用 C 风格强制转换，用 `static_cast` 等
- 能用 `const` 就 `const`：成员函数、局部变量、引用参数

## 2. 命名

| 对象 | 规则 | 示例 |
|---|---|---|
| 类型 / 类 | `CamelCase` | `TemperaturePublisher` |
| 函数 / 方法 | `snake_case` | `on_timer()` |
| 变量 | `snake_case` | `publish_rate_hz` |
| 成员变量 | `snake_case_` | `publisher_`、`timer_` |
| 常量 | `kCamelCase` 或 `UPPER_SNAKE`（消息常量按 IDL） | `kDefaultThreshold` |
| 消息字段 | IDL 定义的 `snake_case` | `sensor_id` |

## 3. 时间与数值

- 时间间隔用 `std::chrono`，不要用裸 `int` 毫秒：

```cpp
using namespace std::chrono_literals;
timer_ = create_wall_timer(1s, std::bind(&TemperaturePublisher::on_timer, this));
```

- ROS 时间（消息时间戳）用 `rclcpp::Time` / `builtin_interfaces::msg::Time`，
  不要和 `std::chrono` 混着算
- 参数不写魔法数字，默认值集中声明
- 从参数读出的数值要校验（频率 > 0、上限合理、阈值有限）

## 4. 回调与线程

- 回调里不做长时间阻塞（`sleep`、同步网络、重计算）
- 回调里不要让异常逃出去；ROS 2 不会替你处理
- 默认单线程 executor；需要并发时先说明理由，再用 callback group
- 修改共享状态时，先问「这个回调会不会和另一个回调同时跑」

## 5. 日志

- 使用 `RCLCPP_INFO` / `WARN` / `ERROR` / `DEBUG`
- 高频日志加节流：`RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000, "...")`
- 日志里带上关键上下文（传感器 id、频率、数值），不要只写「error」

## 6. 参数

```cpp
sensor_id_ = declare_parameter<std::string>("sensor_id", "sensor_0");
publish_rate_hz_ = declare_parameter<double>("publish_rate_hz", 2.0);
```

- 声明时给默认值
- 动态修改要校验；非法值拒绝并保留旧值
- 参数名与任务文档一致，不要自创缩写

## 7. 接口与依赖

- 节点包依赖接口包：`package.xml` 加 `<depend>`，CMake `find_package` + `ament_target_dependencies`
- 不在两个包里重复定义同一条消息
- 接口字段只增不改语义；改字段要同步发布端、订阅端和文档

## 8. 头文件与 include

顺序：

```cpp
// 1. 对应的头文件（如果有）
// 2. C++ 标准库
// 3. 第三方库
// 4. ROS 2 与项目内头文件
```

用 `#pragma once` 或 include guard（二者选一，仓库内保持一致）。

## 9. 提交前检查

```bash
colcon build --symlink-install            # 0 warning
clang-format -i <改过的文件>
git ls-files -z '*.cpp' '*.hpp' '*.h' | xargs -0 -r clang-format --dry-run --Werror
```

构建出现 warning 也要修，不要留到 review。
