# 现代 C++：本项目用到的子集

> 不需要学完《Effective Modern C++》。下面这些是写 ROS 2 节点时**每天都会用到**的部分。
> 目标：看懂骨架、写出不泄漏、不悬垂、能通过审查的代码。标准是 C++17。

## 1. 值、引用与 `const`

```cpp
int a = 1;
int& ref = a;               // 引用：a 的别名，必须初始化
const int& cref = a;        // 只读引用，常用于函数参数

void print(const std::string& text);   // 不拷贝，也不修改
```

规则：

- 函数参数：小类型按值；大对象（字符串、容器、消息）用 `const&`
- 不打算修改就加 `const`；成员函数不修改状态也加 `const`
- 引用不能为空、不能重新绑定；需要「可能为空」时用指针或 `std::optional`

## 2. `auto`：少写类型，但不滥用

```cpp
auto value = 3.14;                       // double
auto msg = std_msgs::msg::Float64();     // 类型很明显
for (const auto& item : items) { ... }
```

适合：类型名很长、从函数返回值、迭代器。
不适合：你想强调类型、或 `auto` 会隐式丢掉 `const`/引用时。审查时会被问「这里推导出什么类型」。

## 3. RAII 与智能指针

RAII：资源在构造时获取、在析构时释放。C++ 靠它避免手动 `new/delete`。

```cpp
// 独占所有权
auto node = std::make_unique<MyNode>();

// 共享所有权
auto session = std::make_shared<Session>();

// 只借用，不拥有
void use(Session& session);
```

规则：

- **禁止裸 `new`/`delete`**；用 `std::make_unique` / `std::make_shared`
- 默认用值或 `unique_ptr`；确实需要共享时才 `shared_ptr`
- 只借用时用引用或裸指针，并保证生命周期长于使用者
- ROS 2 的消息回调常见参数是 `const Msg::SharedPtr` 或 `const Msg &`，不要额外再包一层

## 4. 移动语义（先知道，不急着用）

```cpp
std::string a = "hello";
std::string b = std::move(a);   // 把 a 的资源转给 b，a 之后不要再用
```

- 移动避免大对象拷贝；但**不要对 `const` 对象 `std::move`**（会退化成拷贝）
- 返回局部对象时直接 `return obj;`，编译器会做返回值优化，不要画蛇添足 `std::move`
- 初学阶段：能读懂 `std::move`，不要求自己写移动构造/移动赋值

## 5. 时间：`std::chrono`

```cpp
using namespace std::chrono_literals;

auto period = 500ms;
timer_ = create_wall_timer(period, std::bind(&Node::on_timer, this));
```

- 不要用裸 `int` 表示毫秒/秒；用 `std::chrono` 的类型和字面量
- ROS 消息里的时间戳用 `rclcpp::Time` / `builtin_interfaces::msg::Time`
- 两者不要混着算；需要转换时显式转换并写清单位

## 6. `enum class`

```cpp
enum class Status : std::uint8_t {
    kOk = 0,
    kWarn = 1,
    kError = 2,
};
```

- 用 `enum class`，不要用裸 `enum`（避免名字污染和隐式转 int）
- 需要和消息字段对应时，显式指定底层类型
- 打印时自己写转换函数，不要依赖隐式转换

## 7. Lambda

```cpp
auto callback = [this](const Msg& msg) {
    handle(msg);
};
```

- 捕获列表越窄越好：`[this]`、`[&]`、`[=]` 要能说清生命周期
- 异步/定时器回调里捕获 `this` 时，确认对象活得比回调久
- 能用成员函数就用成员函数；lambda 适合短小的局部逻辑

## 8. `std::optional` 与 `std::variant`（了解）

```cpp
std::optional<double> maybe_value;   // 可能没有值
if (maybe_value) { use(*maybe_value); }

std::variant<int, std::string> v;    // 二选一
```

- `optional` 适合表达「可能没有」，比魔法值（-1、空字符串）清晰
- `variant` 适合有限类型集合；初学阶段知道有这两个工具即可

## 9. 常用标注

```cpp
[[nodiscard]] bool try_start();      // 返回值不能忽略
void run() override;                 // 明确重写虚函数
class Foo final { ... };             // 禁止再被继承
```

- `[[nodiscard]]` 用在「失败必须处理」的函数上
- `override` 能帮你发现签名写错
- 不要给所有东西都加标注，按语义来

## 10. 错误处理

- 构造函数里无法完成初始化时抛异常是合理的
- **不要让异常逃出 ROS 回调**：在回调边界捕获并转成日志/状态
- 能用返回值表达失败时，优先返回 `bool` + 输出参数，或 `std::optional`
- 日志里写清上下文，不要只写 `error`

## 11. 暂时不要用的东西

- 模板元编程、SFINAE、C++20 concepts/ranges
- 手写 placement new、手动管理内存、复杂宏
- 无锁并发、`std::atomic` 的细节（先把单线程 executor 用明白）
- 过度使用 `std::function` 和层层 lambda

## 12. 自检

1. 什么时候用 `const&`，什么时候按值传递？
2. `unique_ptr` 和 `shared_ptr` 的所有权区别是什么？
3. 为什么回调里捕获 `this` 要小心？
4. `std::chrono` 相比 `int` 表示时间解决了什么问题？
5. 为什么 `enum class` 比裸 `enum` 好？

能用自己的话答出来，再开始写第一课。
